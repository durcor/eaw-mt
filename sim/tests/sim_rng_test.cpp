// Host-side validation for the lifted global sim RNG (Phase-3 determinism primitive).
// Cross-checks sim/sim_rng.cpp (lifted from FUN_1401ffb40 / FUN_1401ffbb0 / FUN_1401ffdb0) against
// an INDEPENDENT reference LCG written straight from the recurrence, plus the structural invariants
// the engine relies on (range bounds, the a==b/pct boundary no-draw cases, and — critically — the
// exact number of state advances per call, which is the lockstep/replay contract). The end-to-end
// oracle is the in-game DTRNG capture (tools/analyze_rng_oracle.py) replayed bit-exact.
#include "../sim_rng.h"
#include <cstdio>
#include <cmath>

using namespace eaw;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

// Independent reference: the bare recurrence, written from the spec not the lifted code.
static u32 ref_step(u32& s) { s = s * 0x41c64e6du + 0xbdfu; return s; }

// Reference range_i, derived independently from the decompile semantics.
static int ref_range_i(u32& s, int a, int b, int& draws) {
    int hi = a, lo = b;
    if (a <= b) { hi = b; lo = a; }
    if (lo == hi) return lo;
    u32 range = (u32)hi - (u32)lo;
    u32 bit = 0x4000;
    if (range < 0x7fff) { u32 t = range & 0x4000u; while (!t) { bit >>= 1; t = range & bit; } }
    else range = 0x7ffe;
    u32 mask = bit * 2u - 1u;
    u32 r;
    do { u32 v = ref_step(s); ++draws; r = (v >> 10) & 0x7fffu & mask; } while ((int)range < (int)r);
    return (int)(r + (u32)lo);
}

int main() {
    // ---- 1. Core recurrence: lifted step() matches the reference for a run of seeds. ----
    {
        SimRng g(0x12345678u);
        u32 ref = 0x12345678u;
        for (int i = 0; i < 100000; ++i) {
            u32 a = g.step();
            u32 b = ref_step(ref);
            CHECK(a == b);
        }
    }

    // ---- 2. range_i: identical state evolution + return as the independent reference. ----
    {
        u32 seed = 0xDEADBEEFu;
        SimRng g(seed);
        u32 ref = seed;
        const int spans[][2] = {{0,0},{0,1},{0,5},{5,0},{0,30},{0,100},{-7,7},{10,10},
                                {0,0x7ffd},{0,0x7ffe},{0,0x7fff},{0,0x10000},{-1000,1000}};
        for (int iter = 0; iter < 4000; ++iter) {
            for (auto& sp : spans) {
                u32 before_lift = g.state, before_ref = ref;
                CHECK(before_lift == before_ref);
                int draws = 0;
                int rr = ref_range_i(ref, sp[0], sp[1], draws);
                int gr = g.range_i(sp[0], sp[1]);
                CHECK(gr == rr);
                CHECK(g.state == ref);                       // same number of advances
                int lo = sp[0] < sp[1] ? sp[0] : sp[1];
                int hi = sp[0] < sp[1] ? sp[1] : sp[0];
                CHECK(gr >= lo);
                // range is capped at 0x7ffe, so wide spans can fall short of hi — bound by lo+0x7ffe.
                int eff_hi = (hi - lo) >= 0x7fff ? lo + 0x7ffe : hi;
                CHECK(gr <= eff_hi);
            }
        }
    }

    // ---- 3. range_i edge: a == b returns the value WITHOUT advancing state. ----
    {
        SimRng g(99u);
        u32 before = g.state;
        CHECK(g.range_i(42, 42) == 42);
        CHECK(g.state == before);                            // no draw
        CHECK(g.range_i(-3, -3) == -3);
        CHECK(g.state == before);
    }

    // ---- 4. range_i over a tiny span [0,1] covers both values and stays in range. ----
    {
        SimRng g(7u);
        bool saw0 = false, saw1 = false;
        for (int i = 0; i < 1000; ++i) {
            int v = g.range_i(0, 1);
            CHECK(v == 0 || v == 1);
            saw0 |= (v == 0); saw1 |= (v == 1);
        }
        CHECK(saw0 && saw1);
    }

    // ---- 5. range_f: single draw, in [lo,hi], endpoints reachable, order-independent. ----
    {
        SimRng g(0x55AA55AAu);
        for (int i = 0; i < 20000; ++i) {
            u32 before = g.state;
            f32 v = g.range_f(-2.5f, 7.5f);
            (void)v;
            g.state = before;                                // rewind to check single-advance
            u32 expect = g.state * 0x41c64e6du + 0xbdfu;
            f32 v2 = g.range_f(-2.5f, 7.5f);
            CHECK(g.state == expect);                        // exactly ONE advance
            CHECK(v == v2);
            CHECK(v2 >= -2.5f - 1e-4f && v2 <= 7.5f + 1e-4f);
        }
        // swapped args give the same distribution range.
        SimRng a(1u), b(1u);
        for (int i = 0; i < 100; ++i) CHECK(a.range_f(3.0f, 9.0f) == b.range_f(9.0f, 3.0f));
        // field/32767 hits 1.0 exactly when the 15-bit field is max -> upper endpoint exact.
        SimRng e(0u);
        f32 lo = -4.0f, hi = 4.0f;
        for (int i = 0; i < 200000; ++i) {
            f32 v = e.range_f(lo, hi);
            CHECK(v >= lo - 1e-4f && v <= hi + 1e-4f);
        }
    }

    // ---- 6. percent: boundary no-draw cases + probability monotonicity. ----
    {
        SimRng g(123u);
        u32 before = g.state;
        CHECK(g.percent(0) == false);  CHECK(g.state == before);   // 0 -> never, no draw
        CHECK(g.percent(100) == true); CHECK(g.state == before);   // 100 -> always, no draw

        // Empirical rates should rise monotonically with pct and bracket the requested value.
        auto rate = [](int pct) {
            SimRng r(0xABCDEF01u);
            int hits = 0, N = 200000;
            for (int i = 0; i < N; ++i) hits += r.percent(pct) ? 1 : 0;
            return (double)hits / N;
        };
        double r25 = rate(25), r50 = rate(50), r75 = rate(75);
        CHECK(r25 < r50 && r50 < r75);
        CHECK(std::fabs(r50 - 50.0/101.0) < 0.01);                 // 101-bucket model
        CHECK(std::fabs(r25 - 25.0/101.0) < 0.01);
    }

    // ---- 7. Determinism: same seed -> identical mixed-call sequence. ----
    {
        SimRng a(0xC0FFEEu), b(0xC0FFEEu);
        for (int i = 0; i < 5000; ++i) {
            CHECK(a.range_i(0, 37) == b.range_i(0, 37));
            CHECK(a.range_f(0.f, 1.f) == b.range_f(0.f, 1.f));
            CHECK(a.percent(40) == b.percent(40));
            CHECK(a.state == b.state);
        }
    }

    if (g_fail == 0) std::printf("sim_rng_test: ALL PASS\n");
    else             std::printf("sim_rng_test: %d FAILURE(S)\n", g_fail);
    return g_fail ? 1 : 0;
}
