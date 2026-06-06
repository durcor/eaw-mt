// sim/tests/spatial_query_test.cpp — concurrency gate for the spatial-query short-lock+copy primitive
// (firing_body_lift_scope.md §8.6). Models the engine's INTRUSIVE result list (each candidate's +0x18
// field, head at a shared index) and proves:
//   1. CONTRACT: query_copy returns an INDEPENDENT snapshot — mutating the shared intrusive buffer after
//      the call does not change the returned copy (the copy-out decouples consumers from +0x18).
//   2. CONCURRENCY: with the guard, N threads hammering the shared grid each get the COMPLETE, CORRECT
//      candidate set every time (the atomic build+copy + frozen grid ⇒ no torn lists). corruption == 0.
//   3. NEGATIVE (informational, best-effort): the SAME shared intrusive buffer queried UNGUARDED from
//      concurrent threads DOES tear (a node's +0x18 overwritten mid-walk), proving the guard is
//      load-bearing — reported, not hard-asserted (races are timing-dependent).
#include "../spatial_query.h"
#include <cstdio>
#include <vector>
#include <thread>
#include <atomic>
#include <algorithm>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

// A candidate object with the engine's intrusive `+0x18` result-link. The grid is FROZEN (ids/positions
// const during the walk); only `next` (the result link) is mutated per query — the shared hazard.
struct Node {
    uint32_t id;
    int      next;        // intrusive result link (index into nodes; -1 = end) — the "+0x18" field
};

static const int NCAND = 25;   // Fix B2 caps consumption at 25

// The shared spatial index: candidate nodes + a shared head (the "index+8"). build() relinks ALL nodes
// into the result list (every candidate is "in range" for this fixed query) — overwriting each +0x18.
struct Grid {
    std::vector<Node> nodes;
    int head = -1;
    Grid() { for (int i = 0; i < NCAND; ++i) nodes.push_back({1000u + uint32_t(i), -1}); }
    void build() {                       // 20e780: thread the intrusive list through +0x18
        head = -1;
        for (int i = 0; i < NCAND; ++i) { nodes[i].next = head; head = i; }
    }
    // A query variant whose chain ORDER depends on `salt` — models two DIFFERENT live queries that share
    // candidates (the real hazard: query B overwrites a shared candidate's +0x18 to point into B's chain).
    void build_variant(int salt) {
        head = -1;
        if (salt & 1) for (int i = 0; i < NCAND; ++i)      { nodes[i].next = head; head = i; }  // 24→…→0
        else          for (int i = NCAND - 1; i >= 0; --i) { nodes[i].next = head; head = i; }  // 0→…→24
    }
    void collect(std::vector<uint32_t>& out) const {   // walk +0x18, copy ids out
        for (int i = head; i != -1; i = nodes[i].next) out.push_back(nodes[i].id);
    }
};

static std::vector<uint32_t> expected_set() {
    std::vector<uint32_t> e; for (int i = 0; i < NCAND; ++i) e.push_back(1000u + uint32_t(i));
    std::sort(e.begin(), e.end()); return e;
}

static bool is_full_set(std::vector<uint32_t> got) {
    std::sort(got.begin(), got.end());
    return got == expected_set();
}

int main() {
    // ── 1. CONTRACT: the copy is independent of later shared-buffer mutation. ────────────────────────
    {
        Grid g;
        SpatialQueryGuard<uint32_t> guard;
        std::vector<uint32_t> snap;
        guard.query_copy(snap, [&]{ g.build(); }, [&](std::vector<uint32_t>& o){ g.collect(o); });
        CHECK(is_full_set(snap));
        std::vector<uint32_t> before = snap;
        // simulate the NEXT query scribbling the shared intrusive buffer:
        g.build(); for (auto& n : g.nodes) n.next = -999;  g.head = -1;
        CHECK(snap == before);                 // the earlier snapshot is untouched (decoupled)
    }

    // ── 2. CONCURRENCY: guarded queries always yield the complete correct set. ───────────────────────
    {
        Grid g;
        SpatialQueryGuard<uint32_t> guard;
        std::atomic<int> corruption{0};
        const int NT = 8, ITERS = 4000;
        std::vector<std::thread> ts;
        for (int t = 0; t < NT; ++t) {
            ts.emplace_back([&]{
                std::vector<uint32_t> local;
                for (int k = 0; k < ITERS; ++k) {
                    guard.query_copy(local, [&]{ g.build(); }, [&](std::vector<uint32_t>& o){ g.collect(o); });
                    if (!is_full_set(local)) corruption.fetch_add(1);
                }
            });
        }
        for (auto& th : ts) th.join();
        CHECK(corruption.load() == 0);         // the guard makes build+copy atomic ⇒ never torn
        std::printf("  [guarded] %d threads x %d queries, corruption=%d\n", NT, ITERS, corruption.load());
    }

    // ── 3. NEGATIVE control (DETERMINISTIC interleaving — no timing dependence): the SAME shared buffer
    //    walked UNGUARDED while a different query rebuilds it produces a torn set. Models the exact +0x18
    //    hazard: A reads head + walks a few nodes; B (a different query) overwrites the chain; A continues
    //    following the NOW-CHANGED +0x18 links ⇒ the traversal skips/duplicates ⇒ NOT the full set. This is
    //    what the guard prevents by making build+copy atomic. (Reliable, not race-timing dependent.) ──────
    {
        Grid g;
        g.build_variant(0);                       // query A: chain 0→1→…→24 (head=0)
        std::vector<uint32_t> torn;
        int i = g.head, steps = 0;
        for (; i != -1 && steps < 4*NCAND; i = g.nodes[i].next, ++steps) {
            torn.push_back(g.nodes[i].id);
            if (steps == 3) g.build_variant(1);   // ← query B rebuilds the SHARED chain mid-walk (24→…→0)
        }
        CHECK(!is_full_set(torn));                 // unguarded interleaving ⇒ torn traversal, set is wrong
        std::printf("  [unguarded/interleaved] torn traversal collected %zu ids (full=%d) — guard is load-bearing\n",
                    torn.size(), NCAND);
    }

    if (g_fail == 0) std::printf("spatial_query_test: ALL PASS\n");
    else             std::printf("spatial_query_test: %d FAILURE(S)\n", g_fail);
    return g_fail ? 1 : 0;
}
