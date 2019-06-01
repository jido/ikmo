#include "ikmo.h"

#define half 0x0f0f0f0f0f0f0f0fLL

// Usage:
//  u64 lo = encode(value64);
//  u64 hi = encode(value64 >> 4);
u64 encode(u64 bin) {
   static const u64 mask8 = 0x0808080808080808LL;
   static const u64 alnum = 0x3030303030303030LL;
   u64 t = bin & mask8;
   u64 hij = (bin & half) | (t << 1) | (t << 3);
   return hij ^ alnum;
}

u64 decode(u64 lo, u64 hi) {
   return (lo & half) | ((hi & half) << 4);
}
