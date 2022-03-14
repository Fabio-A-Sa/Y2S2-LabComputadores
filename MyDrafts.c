#include <stdio.h>

int main () {
    printf("Hello World!\n");
    return 0;
}

int main () {
    
    // variables
    char letter = 'F';
    unsigned int unsigned_value_hx = 0x68;
    int value = 120;
    unsigned int unsigned_value = 34324;
    long long_value = 287;
    unsigned long int unsigned_long_int_value = 2453;
    char string[] = "Hello\n";
    int* pointer = &value;

    // output using printf() method
    printf("Char: %c \n", letter);
    printf("Unsigned integer in hexadecimal: %x \n", unsigned_value_hx);
    printf("Integer in decimal: %d \n", value);
    printf("Unsigned int in decimal: %u \n", unsigned_value);
    printf("Long in decimal: %l \n", long_value);
    printf("Unsigned long in decimal: %lu \n", unsigned_long_int_value);
    printf("String: %s ", string);
    printf("Address: %p \n", pointer);

    return 0;
}

void showErrors() {

    // erro comum 1
    signed char cresult, c1, c2, c3;
    c1 = 100;
    c2 = 3;
    c3 = 4;
    cresult = c1 * c2 / c3; // overflow pois um char não consegue conter 400

    // erro comum 2
    uchar port = 0x54;
    uchar result_8 = ( ~port ) >> 4; // se tiver 32 bits e não 8 aparecerá FFs em vez de 00s

    // solução -> um cast antes do resultado
    cresult = ( (int) c1 * c2 ) / c3;
    uchar result_8 = (uint8_t) ( ~port ) >> 4;
}