#include <stdio.h>

typedef unsigned long long u64;
u64 encode(u64 bin);

int main(int n, char *args[]) {
   u64 buf[5];
   int len;
   while ( (len = fread(buf, 1, 40, stdin)) )
   {
      for (int i = 0; i < len / 8; ++i)
      {
          u64 code[2] = { encode(buf[i]), encode(buf[i] >> 4) };
          fwrite(code, 8, 2, stdout);
      }
      if (len < 40) break;
      fputc('\n', stdout);
   }
   int left = len % 8;
   u64 *last = buf + len / 8;
   if (left != 0) {
       u64 data = 0;
       sprintf((char *)&data, "%.*s", left, (char *)last);
       u64 code[2] = { encode(data), encode(data >> 4) };
       fwrite(code, 8, 2, stdout);
   }
   if (len != 0) fputc('\n', stdout);
}

