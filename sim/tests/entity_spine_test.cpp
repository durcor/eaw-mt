// Host-side behavioral test for the lifted entity-update spine.
// Asserts the determinism-relevant control structure recovered from FUN_1402be640 / FUN_1403a6b80:
// reverse-order behavior dispatch, schedule + enable gates, the mode-flag fast path, timed-action
// fire-on-zero, hardpoint/regen flag gates, and GOM/sim-frame traversal order.
#include "../entity_spine.h"
#include <cstdio>
#include <string>
#include <vector>

using namespace eaw;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

// Records the global order in which things happen across an update.
static std::vector<std::string> g_log;

struct RecBehavior : Behavior {
    std::string name;
    bool en = true;
    RecBehavior(std::string n, u32 sched, bool e) : name(std::move(n)) {
        next_service_tick = sched; en = e;
    }
    bool enabled() const override { return en; }
    void update(GameObject&, u32 tick) override {
        g_log.push_back(name + "@" + std::to_string(tick));
    }
};

struct RecHooks : EntityUpdateHooks {
    void locomotor_prestep(GameObject&) override { g_log.push_back("loco"); }
    void fire_timed_action(GameObject&, const TimedAction& a) override {
        g_log.push_back("fire#" + std::to_string(a.id));
    }
    void hardpoint_service(GameObject&, u32) override { g_log.push_back("hardpoint"); }
    void regen_roll(GameObject&, u32) override { g_log.push_back("regen"); }
};

static void test_behavior_dispatch_reverse_order() {
    std::printf("test_behavior_dispatch_reverse_order\n");
    g_log.clear();
    GameObject e;
    RecBehavior b0("b0", 0, true), b1("b1", 0, true), b2("b2", 0, true);
    e.behaviors = { &b0, &b1, &b2 };  // array order 0,1,2 -> dispatched 2,1,0
    RecHooks h;
    update_game_object(e, 100, /*mode_flag=*/false, &h);
    // loco first (has_locomotor=false here so skipped), then behaviors reverse, then no hardpoints.
    CHECK(g_log.size() == 3);
    CHECK(g_log[0] == "b2@100");
    CHECK(g_log[1] == "b1@100");
    CHECK(g_log[2] == "b0@100");
}

static void test_schedule_and_enable_gates() {
    std::printf("test_schedule_and_enable_gates\n");
    g_log.clear();
    GameObject e;
    RecBehavior ready("ready", 50, true);     // scheduled <= tick -> runs
    RecBehavior future("future", 200, true);  // scheduled > tick  -> skipped
    RecBehavior disabled("disabled", 0, false);// enabled()==false -> skipped
    e.behaviors = { &ready, &future, &disabled };
    update_game_object(e, 100, false, nullptr);
    CHECK(g_log.size() == 1);
    CHECK(g_log[0] == "ready@100");
}

static void test_mode_flag_fast_path() {
    std::printf("test_mode_flag_fast_path\n");
    g_log.clear();
    GameObject e;
    e.has_locomotor = true;
    e.has_hardpoints = true;
    e.flags = GO_FLAG_REGEN_ELIGIBLE;  // eligible so regen would fire
    RecBehavior b("b", 0, true);
    e.behaviors = { &b };
    RecHooks h;
    // mode_flag=true => skip blocks 1 & 4 (no loco, no behaviors, no hardpoints); only tail regen.
    update_game_object(e, 5, /*mode_flag=*/true, &h);
    CHECK(g_log.size() == 1);
    CHECK(g_log[0] == "regen");
}

static void test_full_intra_tick_order() {
    std::printf("test_full_intra_tick_order\n");
    g_log.clear();
    GameObject e;
    e.has_locomotor = true;
    e.has_hardpoints = true;
    e.flags = GO_FLAG_REGEN_ELIGIBLE;
    e.timed_actions = { TimedAction{7, 1} };   // countdown 1 -> fires this tick
    RecBehavior b("b", 0, true);
    e.behaviors = { &b };
    RecHooks h;
    update_game_object(e, 10, false, &h);
    // Order: locomotor pre-step -> timed-action fire -> behaviors -> hardpoint -> regen.
    CHECK(g_log.size() == 5);
    CHECK(g_log[0] == "loco");
    CHECK(g_log[1] == "fire#7");
    CHECK(g_log[2] == "b@10");
    CHECK(g_log[3] == "hardpoint");
    CHECK(g_log[4] == "regen");
}

static void test_timed_action_countdown() {
    std::printf("test_timed_action_countdown\n");
    g_log.clear();
    GameObject e;
    e.timed_actions = { TimedAction{1, 3} };   // needs 3 ticks to fire (3->2->1->0)
    RecHooks h;
    for (u32 t = 1; t <= 2; ++t) update_game_object(e, t, false, &h);
    CHECK(g_log.empty());                        // not fired after 2 decrements (countdown==1)
    CHECK(e.timed_actions.size() == 1);
    update_game_object(e, 3, false, &h);         // third decrement -> 0 -> fire + erase
    CHECK(g_log.size() == 1);
    CHECK(g_log[0] == "fire#1");
    CHECK(e.timed_actions.empty());
}

static void test_hardpoints_off_flag() {
    std::printf("test_hardpoints_off_flag\n");
    g_log.clear();
    GameObject e;
    e.has_hardpoints = true;
    e.flags = GO_FLAG_HARDPOINTS_OFF | GO_FLAG_REGEN_ELIGIBLE;
    RecHooks h;
    update_game_object(e, 1, false, &h);
    // hardpoints-off also suppresses regen (both gated on !GO_FLAG_HARDPOINTS_OFF).
    CHECK(g_log.empty());
}

static void test_block1_transient_skip() {
    std::printf("test_block1_transient_skip\n");
    g_log.clear();
    GameObject e;
    e.flags = GO_FLAG_TRANSIENT_8;
    e.template_blocks_update = true;   // (flags&8) set AND template blocks -> skip block 1
    e.has_hardpoints = true;
    RecBehavior b("b", 0, true);
    e.behaviors = { &b };
    RecHooks h;
    update_game_object(e, 1, false, &h);
    // block 1 (behaviors) skipped, but block 4 (hardpoints) still runs.
    CHECK(g_log.size() == 1);
    CHECK(g_log[0] == "hardpoint");
}

static void test_gom_and_frame_traversal_order() {
    std::printf("test_gom_and_frame_traversal_order\n");
    g_log.clear();
    GameObject e0, e1, e2;
    RecBehavior b0("e0", 0, true), b1("e1", 0, true), b2("e2", 0, true);
    e0.behaviors = { &b0 }; e1.behaviors = { &b1 }; e2.behaviors = { &b2 };
    GameObjectManager gom_a, gom_b;
    gom_a.master_update_list = { &e0, &e1 };  // order e0, e1
    gom_b.master_update_list = { &e2 };
    RecHooks h;
    sim_frame_update(gom_a, gom_b, 42, &h);
    // GOM A in list order, then GOM B.
    CHECK(g_log.size() == 3);
    CHECK(g_log[0] == "e0@42");
    CHECK(g_log[1] == "e1@42");
    CHECK(g_log[2] == "e2@42");
}

static void test_gom_lightweight_mode() {
    std::printf("test_gom_lightweight_mode\n");
    g_log.clear();
    GameObject e;
    RecBehavior b("b", 0, true);
    e.behaviors = { &b };
    GameObjectManager gom;
    gom.master_update_list = { &e };
    gom.lightweight_mode = true;   // forwarded as mode_flag -> behaviors skipped
    RecHooks h;
    gom_update(gom, 1, &h);
    CHECK(g_log.empty());
}

int main() {
    std::printf("== entity_spine host validation ==\n");
    test_behavior_dispatch_reverse_order();
    test_schedule_and_enable_gates();
    test_mode_flag_fast_path();
    test_full_intra_tick_order();
    test_timed_action_countdown();
    test_hardpoints_off_flag();
    test_block1_transient_skip();
    test_gom_and_frame_traversal_order();
    test_gom_lightweight_mode();
    if (g_fail) { std::printf("\nFAILED: %d check(s)\n", g_fail); return 1; }
    std::printf("\nAll entity_spine checks passed.\n");
    return 0;
}
