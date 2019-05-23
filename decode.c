#include <stdio.h>

char data[] =
#include "code.txt"
#include "code.txt"
#include "code.txt"
#include "code.txt"
#include "code.txt"
#include "code.txt"
#include "code.txt"
#include "code.txt"
#include "code.txt"
#include "code.txt"
;

typedef unsigned long long u64;

u64 decode(u64 a, u64 b);

int main(int n, char *args[]) {
   u64 *end = (u64 *)(data + sizeof(data) - 1);
   
   const int size = 128;
   u64 buf[size];
   int p = 0;
   for (u64 *x = (u64 *)data; x < end; x += 2)
   {
       buf[p] = decode(x[0], x[1]);
       if (p == size - 1)
       {
           fwrite(&buf, sizeof(u64), size, stdout);
           p = 0;
       }
       else ++p;
   }
   fwrite(&buf, sizeof(u64), p, stdout);
}