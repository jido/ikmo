#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long u64;
#define BYTES sizeof(u64)
u64 encode(u64 bin);

int main(int n, char *args[]) {
   u64 buf[1024 / BYTES];
   u64 code[1024 / BYTES][2];
   int count = 1024 / BYTES;
   if (n > 1)
   {
       int arg = strtod(args[1], NULL);
       if (arg < 1 || arg > count)
       {
           puts("Usage: encode n");
           printf(" n: number of 64-bit words per chunk (max %d)\n", count);
           puts("Warning - number out of range, using default.");
       }
       else count = arg;
   }
   int len;
   while ( (len = fread(buf, 1, count * BYTES, stdin)) )
   {
      for (int i = 0; i < len / BYTES; ++i)
      {
          u64 *pair = code[i];
          pair[0] = encode(buf[i]);
          pair[1] = encode(buf[i] >> 4);
      }
      if (len < count * BYTES) break;
      fwrite(code, BYTES * 2, count, stdout);
      fputc('\n', stdout);
   }
   int left = len % BYTES;
   if (len != 0 || left != 0)
   {
       if (left != 0)
       {
           char *last = ((char *)buf) + len - left;
           u64 data = 0;
           char *p = (char *)&data;
           for (int i = 0; i < left; ++i) p[i] = last[i];
           u64 *pair = code[len / BYTES];
           pair[0] = encode(data);
           pair[1] = encode(data >> 4);
       }
       fwrite(code, BYTES * 2, len / BYTES + (left != 0), stdout);
       fputc('\n', stdout);
   }
}
