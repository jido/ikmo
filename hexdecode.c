#include <stdio.h>

char data[] = 
#include "hex.txt"
#include "hex.txt"
#include "hex.txt"
#include "hex.txt"
#include "hex.txt"
#include "hex.txt"
#include "hex.txt"
#include "hex.txt"
#include "hex.txt"
#include "hex.txt"
;

// This should fit in a cache line
const short hextable[32] = {
   -1, 10, 11, 12, 13, 14, 15,
   -1, -1, -1, -1, -1, -1, -1, -1, -1,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    -1, -1, -1, -1, -1, -1
};

short hexdecode(char hex[2]) {
   return (hextable[hex[0] & 0x1f] << 4) | hextable[hex[1] & 0x1f];
}

int main(int n, char *args[]) {
    char *end = data + sizeof(data) - 1;
   
    char buf[1024];
    int p = 0;
    for (char *x = data; x < end; x += 2)
    {
        buf[p] = hexdecode(x);
        if (p == sizeof(buf) - 1)
        {
            fwrite(&buf, sizeof(buf), 1, stdout);
            p = 0;
        }
        else ++p;
    }
    fwrite(&buf, 1, p, stdout);
}
