#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BIT(n) (1<<(n))

char *byte2bin(uint8_t n, char *binstr) 
{
    // one element per bit (0/1)
    uint8_t binary[8];

    int i = 0;
    for(i = 0; i < 8; i++) {
        binary[i] = n % 2;
        n = n / 2;
    }

    // printing binary array in reverse order
    for (int j = 7, k= 0; j >= 0; j--, k++)
        sprintf(binstr + k, "%d", binary[j]);
        
    return binstr;
}

void print_usage(char *name) {
    printf("Usage: %s <action> <byte> <bit no>\n", name);
    printf("\tWhere: <action> one of 'h'|'l'|'r'|'s'|'t'\n"
        "\t\t <byte> 8-bit integer in hexadecimal\n"
        "\t\t <bit no> of bit to operate on\n");
}

int main(int argc, char *argv[])
{
    char a;             // action: 'h', 'l', 'r', 's', 't'
    unsigned long n;    // value to convert must be smaller than 256
    int bit;            // bit to operate on: must be between 0 an 7
    
    { // Validate command line arguments 
        if (argc != 4) {
            print_usage(argv[0]);
            return 1;
        }
        
        // In C a string is essentially a null terminated character array
        a = argv[1][0];
        if( a != 'h' && a != 'l' && a != 'r' && a != 's' && a != 't' ) {
            print_usage(argv[0]);
            printf("\t<action> must be one of 'h'|'l'|'r'|'s'|'t'\n");
            return 1;
        }
    
        // We are expecting the argument to be an hexadecimal string
        n = strtoul(argv[2],NULL,16);
        if( n < 0 || n > (1 << 8) - 1) {
            print_usage(argv[0]);
            printf("\t<byte> must be non-negative and smaller than 256\n");
            return 1;
        }
        
        bit = atoi(argv[3]);
        if( bit < 0 || bit > 7 ) {
            print_usage(argv[0]);
            printf("\t<bit no> must be between 0 and 7\n");
            return 1;
        }    
    } // End of command line arguments validation
    
    // Print to stdout the binary representation of n
    char binary[9];
    if (byte2bin(n, binary) == NULL) {
        printf("Error in decimal to binary translation\n");
        return 1;
    }
    printf("Number %s in hexadecimal is %ld in decimal and %s in binary\n", argv[2], n, binary);

    // Do what the user asked and print the result
    uint8_t result, mask = BIT(bit);
    switch (a) {

        case 'h': case 'l':
            if (mask & n) {
                printf("Bit %d of number %ld is high (1)\n", bit, n);
            } else {
                printf("Bit %d of number %ld is lower (0)\n", bit, n);
            }
            break;

        case 'r':
            result = n & ~mask;
            printf("Reset bit %d of number %ld results in %d\n", bit, n, result);
            break;

        case 's':
            result = n | mask;
            printf("Set bit %d of number %ld results in %d\n", bit, n, result);
            break;

        case 't':
            result = n ^ mask;
            printf("Toggle bit %d of number %ld results in %d\n", bit, n, result);
            break;

        default:
            return 1;
    }

    return 0;
}