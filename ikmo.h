
typedef unsigned long long u64;

// Usage:
//  u64 lo = encode(value64);
//  u64 hi = encode(value64 >> 4);
u64 encode(u64 bin);
u64 decode(u64 lo, u64 hi);
