// sim/spatial_query.h — the short-lock + copy-out wrapper that makes the engine's spatial query safe to
// call from concurrent fire-pass workers (firing_body_lift_scope.md §8.6).
//
// THE HAZARD (decoded in §8.6): the per-team spatial index (FUN_1402acb60) is READ-ONLY during the Pass-A
// walk — FUN_1402be640 rebuilds every team's grid ONCE at tick start (the 20ed70 preamble) before the
// per-object walk, and the locomotor/transform/body never touch it, so the grid STRUCTURE is frozen
// (tick-start view, for free). Concurrent geometry READS are safe. The thread-hostile part is the query's
// OUTPUT: FUN_14020e780 builds an INTRUSIVE result list threaded through each result object's +0x18 field
// (head at the shared index+8, plus a benign global counter DAT_140a7c66c++). Two concurrent queries that
// share ANY candidate clobber that candidate's +0x18 ⇒ corruption. A per-thread INDEX would not fix it —
// the +0x18 lives on the shared objects.
//
// THE MITIGATION (clean, deterministic, low-effort): a SHORT LOCK around { run the query + copy the result
// pointers into a thread-local buffer }, then evaluate the candidates from the local copy OUTSIDE the lock,
// in parallel. The lock covers only the fast tree-walk + a bounded pointer copy (consumption is capped at
// 25 by Fix B2), so the EXPENSIVE per-candidate eval (fog/sensor/score — the parallel win) stays parallel.
// Determinism holds: the frozen grid returns the same candidate SET regardless of lock acquisition order;
// the shared +0x18 / index+8 / counter are written-then-copied under the lock and never read across units.
// No FUN_14020e780 rewrite is needed; a full non-intrusive rewrite is the fallback only if the 25-cap
// critical section ever shows contention.
//
// This header is the host realization of that wrapper: generic over the result type and the query/collect
// callables, so the host concurrency test and the eventual engine binding (lock around 2acb60→20e780 +
// copy the intrusive list off +0x18) share one mechanism. The grid-frozen invariant is the CALLER's
// obligation (upheld by the Pass-A sweep barrier — the same no-mutation contract FrozenSnapshot relies on).
#pragma once
#include <vector>
#include <mutex>

namespace sim {

template <class Result>
class SpatialQueryGuard {
public:
    // Atomically: run `query` (it populates the shared intrusive result buffer) then `collect` (it walks
    // that buffer and appends each result into `out`). On return `out` is an INDEPENDENT thread-local
    // snapshot — the caller evaluates it lock-free while other workers run their own guarded queries.
    //   query  : void()                      — the build into the shared buffer (2acb60→20e780).
    //   collect: void(std::vector<Result>&)  — copy result pointers off the shared +0x18 list into out.
    template <class QueryFn, class CollectFn>
    void query_copy(std::vector<Result>& out, QueryFn&& query, CollectFn&& collect) {
        out.clear();
        {
            std::lock_guard<std::mutex> lk(mu_);
            query();              // build the intrusive list into the shared index+8 / +0x18 chain
            collect(out);         // copy the result pointers OUT into the thread-local snapshot
        }                         // lock released here — `out` is decoupled from the shared buffer
        // (caller now runs the expensive per-candidate eval on `out` in parallel, no lock held)
    }

private:
    std::mutex mu_;
};

} // namespace sim
