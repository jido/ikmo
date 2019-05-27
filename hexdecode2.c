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

short hexbyte(char hex[2]) {
   return (hextable[hex[0] & 0x1f] << 4) | hextable[hex[1] & 0x1f];
}

// Decode the string until the pointer reaches a NUL byte or "something"
// which the function cannot decode.
// That "something" is pretty loose and it will happily decode characters
// outside the 0-F range, in fact half of the 256 possible characters
// values are decoded as if they were all hex characters.
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
    size_t len = hexdecode(data, data);  // reuse the beginning of the data as decoding buffer
    fwrite(data, 1, len, stdout);
}
