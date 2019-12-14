#include <string.h>
#include <limits.h>

typedef unsigned long long u64;
const int b32_chars_per_u64 = (sizeof(u64) * CHAR_BIT + 4) / 5;

size_t b32encoded_size(size_t size) {
    size_t len = size / sizeof(u64);
    int rest = size % sizeof(u64);
    return len * b32_chars_per_u64 + (rest * CHAR_BIT + 4) / 5;      // round up number of base32 digits
}

size_t b32decoded_size(size_t size) {
    size_t len = size / b32_chars_per_u64;
    int rest = size % b32_chars_per_u64;
    return len * sizeof(u64) + rest * 5 / CHAR_BIT;   // round down number of bytes
}

/*
 Alternative alphabets:
 00  ( @ `
 01  ) A a
 02  * B b      -- use * if worried someone will read B as 8
 03  + C c
 04  , D d
 05  - E e
 06  . F f
 07  / G g
 08  [space] H h
 09  ! I i      -- similar looking characters are equivalent
 0a  " J j
 0b  # K k
 0c  $ L l      -- use $ if worried someone will read l as 1 or I
 0d  % M m
 0e  & N n
 0f  ' O o      -- unfortunately 0 is not equivalent to O
 10  8 P p
 11  9 Q q      -- use 9 if worried someone will read Q as O
 12  : R r
 13  ; S s      -- unfortunately 5 is not equivalent to S
 14  < T t
 15  = U u
 16  > V v
 17  ? W w
 18  0 X x
 19  1 Y y
 1a  2 Z z      -- similar looking characters are equivalent
 1b  3 [ {
 1c  4 | \
 1d  5 ] }
 1e  6 ^ ~
 1f  7 _
 This can be used to replace individual characters to hide profanity or unwanted words.
*/
const char b32table[32] = {
  '@', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
  'x', 'y', 'z', '3', '4', '5', '6', '7'
};

// Note: little endian byte order, assume u64 is 64 bits
void encode_word(u64 bin, char *to) {
    to[0] = b32table[bin & 0x1f];
    to[1] = b32table[(bin >> 5) & 0x1f];
    to[2] = b32table[(bin >> 10) & 0x1f];
    to[3] = b32table[(bin >> 15) & 0x1f];
    to[4] = b32table[(bin >> 20) & 0x1f];
    to[5] = b32table[(bin >> 25) & 0x1f];
    to[6] = b32table[(bin >> 30) & 0x1f];
    to[7] = b32table[(bin >> 35) & 0x1f];
    to[8] = b32table[(bin >> 40) & 0x1f];
    to[9] = b32table[(bin >> 45) & 0x1f];
    to[10] = b32table[(bin >> 50) & 0x1f];
    to[11] = b32table[(bin >> 55) & 0x1f];
    to[12] = b32table[(bin >> 60) & 0x1f];
}

// len = byte length of source data
size_t b32encode(char *from, char *to, size_t len) {
    size_t es = len / sizeof(u64) * b32_chars_per_u64;
    while (len >= sizeof(u64))
    {
        u64 *bin = (u64 *)from;     // assume from is properly aligned
        encode_word(*bin, to);
        to += b32_chars_per_u64;
        from += sizeof(u64);
        len -= sizeof(u64);
    }
    if (len != 0)
    {
        u64 rest = 0;
        char *prest = (char *)&rest;
        for (int c = 0; c < len; ++c) prest[c] = from[c];   // copy remaining bytes
        char temp[b32_chars_per_u64];
        encode_word(rest, temp);    // encode to temporary buffer
        int count = b32encoded_size(len);
        strncpy(to, temp, count);
        es += count;
    }
    return es;
}

// len = length of base 32 string
size_t b32decode(char *from, char *to, size_t len) {
    u64 buf[b32_chars_per_u64];
    size_t ds = 0;
    while (len > 0)
    {
        int x = (len < sizeof(buf) ? len : sizeof(buf));
        strncpy((char *)buf, from, x);
        // Assume u64 is 64 bits
        for (int w = 0; w < b32_chars_per_u64 && w * sizeof(u64) < len + sizeof(u64); ++w)
        {
            u64 t =  ~buf[w] & 0x4040404040404040LL;
            buf[w] = (buf[w] & 0x1f1f1f1f1f1f1f1fLL) ^ (t >> 3);
        }
        for (char *bytes = (char *)buf; len > 0 && bytes < ((char *)buf) + sizeof(buf); bytes += b32_chars_per_u64)
        {
            // Note: little endian byte order
            u64 bin = (u64)bytes[0]
                | ((u64)bytes[1] << 5)
                | ((u64)bytes[2] << 10)
                | ((u64)bytes[3] << 15)
                | ((u64)bytes[4] << 20)
                | ((u64)bytes[5] << 25)
                | ((u64)bytes[6] << 30)
                | ((u64)bytes[7] << 35)
                | ((u64)bytes[8] << 40)
                | ((u64)bytes[9] << 45)
                | ((u64)bytes[10] << 50)
                | ((u64)bytes[11] << 55)
                | ((u64)bytes[12] << 60);
            int count = sizeof(u64);
            if (len >= b32_chars_per_u64)
            {
                len -= b32_chars_per_u64;
                from += b32_chars_per_u64;
            }
            else
            {
                count = b32decoded_size(len);      // number of bytes left to copy
                len = 0;
            }
            char *pbin = (char *)&bin;
            for (int c = 0; c < count; ++c) to[c] = pbin[c];
            to += sizeof(u64);
            ds += count;
        }
    }
    return ds;
}

