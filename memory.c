#include <stdint.h>
#include <stdio.h>

uint8_t SIZE = 16;
uint8_t HEAP[16]; // 8 * 16

//uint8_t MY_HEAP[64000];  --> real heap
 
void init_memory() {
    //HEAP[0] = 0b1101001;
    HEAP[0] = 16;
}

void print_block(uint8_t block) {
    for (int i = 0; i < 8; i++) {
        uint8_t bit = (block >> i) & 1;
        printf("Bit %d: %u\n", i, bit);
    }
}

void *my_malloc(size_t size) {
    int index = 0;
    

    /*while ((HEAP[index] & 0b1) == 1) {
        
        index 
    } */
    

}

void reset(uint8_t x) {
    printf("%u\n", (x | (1 << 7)));

}

/* uint64_t set_bit(uint64_t x, int pos, bool value) {
    if(value) {
        return x | (value << pos);
    } else {
        return x & (value << pos);
    }
} */


uint8_t r_big(uint8_t x) {
    return x &= (x-1);

    uint8_t b = 1 << 7;
    
    for (int i = 0; i < 8; i++) {
        if (x & b) {
            x ^= b;  
            break;  
        }
        b >>= 1;    
    }

    return x;
    printf("%d\n", x);
}



int main(int argc, char const *argv[])
{
    init_memory();

    uint8_t a = r_big(0b00000001);
    uint8_t b = r_big(0b10000001);
    uint8_t c = r_big(0b11111110);
    uint8_t d = r_big(0b00001111);

    print_block(a);
    printf("\nNow b\n");
    print_block(b);
    printf("\nNow c\n");
    print_block(c);
    printf("\nNow d\n");
    print_block(d);

    /* reset(0b00000001); // 1
    reset(0b11111110); // 255
    reset(0b11110000); // 
    reset(0b00001111); // 15
    reset(0b01010101); // 

   // HEAP[0] = 0b0001000;
    print_block(HEAP[0]); */
    
    return 0;
}

