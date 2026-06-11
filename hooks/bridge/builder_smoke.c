// §8.70 builder-API smoke: drives fc_bridge.dll's builder (reset → setters → decide) exactly as the hook
// will, validating the plumbing (setters fill the struct, decide runs fire_control_decide on it) end-to-end
// through the DLL under wine. FireOutcome: 0=Ineligible 1=NoAim 2=OutOfRange 3=NoLead 4=Unreachable 5=Fire.
#include <windows.h>
#include <stdio.h>

typedef void (*reset_fn)(void);
typedef void (*gates_fn)(unsigned int);
typedef void (*aim_fn)(int,float,float,float);
typedef void (*range_fn)(int,float,float,float,float,float,float);
typedef void (*stageg_fn)(float,float,float,float,float,float,int,int);
typedef void (*lead_fn)(float,float,float,float,float,float,float,float,float,float,float,int);
typedef void (*spread_fn)(int,float,float,float,float);
typedef void (*screate_fn)(unsigned long long,unsigned int,int,unsigned int,unsigned int,unsigned long long,int,int);
typedef void (*spayload_fn)(float,float,int,float,unsigned int,unsigned int,int,int,int,float,float,float,int,float);
typedef int  (*decide_fn)(unsigned int,int*,float*,float*,float*,unsigned int*,unsigned long long*,int*,float*,unsigned int*,float*,int*,int*);

static HMODULE H;
#define GP(t,n) (t)GetProcAddress(H,n)

int main(void) {
    H = LoadLibraryA("fc_bridge.dll");
    if (!H) { printf("LOAD FAIL %lu\n", GetLastError()); return 1; }
    reset_fn   reset = GP(reset_fn,"fc_bridge_in_reset");
    gates_fn   gates = GP(gates_fn,"fc_bridge_in_gates");
    aim_fn     aim   = GP(aim_fn,"fc_bridge_in_aim");
    range_fn   range = GP(range_fn,"fc_bridge_in_range");
    stageg_fn  stage = GP(stageg_fn,"fc_bridge_in_stageg");
    lead_fn    lead  = GP(lead_fn,"fc_bridge_in_lead");
    spread_fn  spread= GP(spread_fn,"fc_bridge_in_spread");
    screate_fn scr   = GP(screate_fn,"fc_bridge_in_spawn_create");
    spayload_fn spay = GP(spayload_fn,"fc_bridge_in_spawn_payload");
    decide_fn  decide= GP(decide_fn,"fc_bridge_in_decide");
    if (!reset||!gates||!aim||!range||!stage||!lead||!spread||!scr||!spay||!decide) { printf("GETPROC FAIL\n"); return 2; }

    int gate; float pos[3],ld[3],gl[3]; unsigned int firer,life; unsigned long long tgt; int sub,vis,guided; float dmg,ms;
    #define DECIDE() decide(12345u,&gate,pos,ld,gl,&firer,&tgt,&sub,&dmg,&life,&ms,&vis,&guided)
    int fails = 0;

    // 1. ineligible: clear gate bit 3 (TargetTargetable). Expect outcome 0, blocked_gate 4.
    reset(); gates(0x0FFF & ~(1u<<3)); aim(1, 100,0,0); range(1, 0,0,0, 1e30f,0,0);
    int o = DECIDE();
    if (!(o==0 && gate==4)) { printf("FAIL ineligible: outcome=%d gate=%d (want 0,4)\n", o, gate); fails++; }

    // 2. out-of-range: all gates pass, has_ctx, muzzle INVALID → OutOfRange (2).
    reset(); gates(0x0FFF); aim(1, 100,0,0); range(0, 0,0,0, 1e30f,0,0);
    o = DECIDE();
    if (o!=2) { printf("FAIL oor(muzzle): outcome=%d (want 2)\n", o); fails++; }

    // 3. out-of-range: muzzle valid but target beyond weapon_range → OutOfRange (2).
    reset(); gates(0x0FFF); aim(1, 1000,0,0); range(1, 0,0,0, /*range*/10,/*ext*/0,/*min*/0);
    o = DECIDE();
    if (o!=2) { printf("FAIL oor(dist): outcome=%d (want 2)\n", o); fails++; }

    // 4. fire: gates pass, aim at a reachable target in range, stationary target ⇒ non-zero lead, reach=1.
    reset(); gates(0x0FFF); aim(1, 100,0,0); range(1, 0,0,0, /*range*/1e6f,0,0);
    stage(0,0,0, 0,0,0, /*full_random*/0, /*has_bone2*/0);   // shooter_ref = mat1 = origin
    lead(100,0,0, 0,0,0, 0,0,0, /*gs*/30, /*ps*/50, /*reach*/1);  // tgt at (100,0,0), still → lead toward it
    spread(/*no_spread*/1, 0,0,0,0);                          // no spread → launch_dir = lead
    scr(0,0,-1,0, /*firer*/7, /*target*/0x1234, /*sub*/-1, /*guided*/0);
    spay(/*od*/5,/*td*/9,0,1.0f, /*ol*/0,/*tl*/40, /*hv*/0,0,0, /*bs*/0,/*ts*/12,/*ext*/0,0,0);
    o = DECIDE();
    if (o!=5) { printf("FAIL fire: outcome=%d (want 5=Fire)\n", o); fails++; }
    else if (!(firer==7 && tgt==0x1234 && sub==-1 && dmg==5.0f && life==40u)) {
        printf("FAIL fire payload: firer=%u tgt=%llx sub=%d dmg=%g life=%u (want 7,1234,-1,5,40)\n",
               firer, tgt, sub, dmg, life); fails++;
    }

    printf("%s builder smoke: %d failure(s)\n", fails? "FAIL":"PASS", fails);
    return fails ? 3 : 0;
}
