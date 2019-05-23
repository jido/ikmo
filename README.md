# ikmo
Very fast hex-like encoding of binary data

## Algorithm

This implementation is based on 64-bit numbers, but it can be trivially adapted to any bit width.

1. Take the four smallest bits of each byte making up the 64-bit number

2. Add a value in the upper bits of each byte to make the bytes printable:

- 48 ('0') if the lower bit is between 0 and 7
 
- 96 ('a'-1) if the lower bit is between 8 and 15
 
3. Shift the four largest bits of each byte to lower position and repeat.

Decoding just requires to mask away the upper bits of the 64-bit string, and combine with the successive
64-bit string into a 64-bit number after a shift.

## Example

```
u64 x = 0xfedcba9876543210LL;
u64 y = encode(x);
u64 z = encode(x >> 4);
```

The output for y and z is:

"0246hjln"
"1357ikmo"

Note that easily-confused "0" and "o", "1" and "l" characters are present in the output.

## Applications

Extremely performance-sensitive applications where alphanumerical digits are required, but human entry
is not expected.

Data size should be a multiple of 8 (ie. 64 bit) else 0 padding is added.
