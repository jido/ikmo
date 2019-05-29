#include <stdio.h>
#include <string.h>

typedef unsigned long long u64;

const char b32table[32] = {
  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  '@', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
  'x', 'y', 'z', '3', '4', '5', '6', '7',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w' 
};

size_t b32encode(char *from, char *to, size_t len) {
    size_t es = len / sizeof(u64) * 13;
    while (len >= sizeof(u64))
    {
        u64 *bin = (u64 *)from;
        to[0] = b32table[*bin & 0x1f];
        to[1] = b32table[(*bin >> 5) & 0x1f];
        to[2] = b32table[(*bin >> 10) & 0x1f];
        to[3] = b32table[(*bin >> 15) & 0x1f];
        to[4] = b32table[(*bin >> 20) & 0x1f];
        to[5] = b32table[(*bin >> 25) & 0x1f];
        to[6] = b32table[(*bin >> 30) & 0x1f];
        to[7] = b32table[(*bin >> 35) & 0x1f];
        to[8] = b32table[(*bin >> 40) & 0x1f];
        to[9] = b32table[(*bin >> 45) & 0x1f];
        to[10] = b32table[(*bin >> 50) & 0x1f];
        to[11] = b32table[(*bin >> 55) & 0x1f];
        to[12] = b32table[(*bin >> 60) & 0x1f];
        to += 13;
        from += sizeof(u64);
        len -= sizeof(u64);
    }
    if (len != 0)
    {
        // 11100000 32222211 44443333 66555554 77777666 99988888 baaaaa99 ccccbbbb
        const int offset[12] = {0, 3, 0, 1, 4, 0, 2, 5, 0, 3, 0, 1};
        unsigned c = *from;
        --len;
        int count = 0;
        int stop = 0;
        while (1)
        {
            *to = b32table[c & 0x1f];
            ++count;
            if (stop) break;
            ++to;
            c >>= 5;
            if (offset[count] != 0)
            {
                stop = (len == 0);
                if (!stop)
                {
                    --len;
                    ++from;
                    c |= ((unsigned)*from) << offset[count];
                }
            }
        }
        es += count;
    }
    return es;
}

size_t b32decode(char *from, char *to, size_t len) {
    size_t es = len / 13 * sizeof(u64);
    while (len >= 13)
    {
        u64 *bin = (u64 *)to;
        u64 *code0 = (u64 *)from;
        u64 t =  *code0 & 0x4040404040404040LL;
        u64 lo = (*code0 & 0x1f1f1f1f1f1f1f1fLL) ^ (t >> 3);
        char *bytes = (char *)&lo;
        u64 dest = (u64)bytes[0]
            | ((u64)bytes[1] << 5)
            | ((u64)bytes[2] << 10)
            | ((u64)bytes[3] << 15)
            | ((u64)bytes[4] << 20)
            | ((u64)bytes[5] << 25)
            | ((u64)bytes[6] << 30)
            | ((u64)bytes[7] << 35);
        u64 code1 = (u64)from[8] | ((u64)from[9] << 8) | ((u64)from[10] << 16) | ((u64)from[11] << 24) | ((u64)from[12] << 32);
        t =  code1 & 0x4040404040404040LL;
        lo = (code1 & 0x1f1f1f1f1f1f1f1fLL) ^ (t >> 3);
        dest |= ((u64)(bytes[0]) << 40) | ((u64)bytes[1] << 45) | ((u64)bytes[2] << 50) | ((u64)bytes[3] << 55) | ((u64)bytes[4] << 60);
        *bin = dest;
        to += sizeof(u64);
        from += 13;
        len -= 13;
    }
    return es;
}

int main(int n, char* args[]) {
    char buf[1024];
    size_t size = b32encode(args[1], buf, strlen(args[1]));
    buf[size] = ' ';
    size_t size2 = b32decode(buf, buf + size + 1, size);
    buf[size + 1 + size2] = 0;
    printf("%lu->%lu. %s\n", strlen(args[1]), size, buf);
}