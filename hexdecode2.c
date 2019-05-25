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
static const short hextable[] = {
   -1, 10, 11, 12, 13, 14, 15,
   -1, -1, -1, -1, -1, -1, -1, -1, -1,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    -1, -1, -1, -1, -1, -1
};

short hexbyte(char hex[2]) {
   return (hextable[hex[0] & 0x1f] << 4) | hextable[hex[1] & 0x1f];
}

size_t hexdecode(char *from, char *to) {
    short value;
    char *bin;
    for (bin = to; *from && (value = hexbyte(from)) >= 0; ++bin)
    {
        *bin = value;
        from += 2;
    }
    return bin - to;
}

int main(int n, char *args[]) {
    size_t len = hexdecode(data, data);
    fwrite(data, 1, len, stdout);
}
