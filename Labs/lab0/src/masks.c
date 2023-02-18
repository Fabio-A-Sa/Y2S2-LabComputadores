#include <stdio.h>
#include <stdlib.h>

int is_odd_v1(const char number) {
    return number % 2;
}

int is_odd_v2(const char number) {
    return number & 1;
}

int main() {

    unsigned char byte = 0x38; // 0011 1000
    unsigned char mask = 0xA9; // 1010 1001
    unsigned char result;
    
    result = byte & mask;
    printf("Testing AND operation: 0x%02x\n", result);

    result = byte | mask;
    printf("Testing  OR operation: 0x%02x\n", result);

    result = byte ^ mask;
    printf("Testing XOR operation: 0x%02x\n", result);

    return 0;
}