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

uint8_t reset_lsb(uint8_t x) {
    return x &= (x-1);
}

void *my_malloc(size_t size) {
    for (int i = 0; i < SIZE/2; i++) {
        printf("Number at HEAP[%d]: %u\n",i, HEAP[i]);
        if((HEAP[i] & 0b1) == 0) {
            // MALLOC
            printf("MALLOC");
            break;
        } else {
            printf("%u\n", HEAP[i]);
            printf("RESET %u\n", reset_lsb(HEAP[i])); // JUMP DE n BLOCK
            i += reset_lsb(HEAP[i]);
            i--;
        }
        // FREE DONC -> MALLOC
    }


    

  /*   while ((HEAP[index] & 0b1) == 1) {
        
        index 
    } */
    

}

int main(int argc, char const *argv[])
{
    init_memory();

    HEAP[0] = 0b0000101; // 2 block free
    HEAP[1] = 0b0000000; // block free -> donnée osef
    HEAP[2] = 0b0000011; // 2 block free
    HEAP[3] = 0b0000000; // block free -> donnée osef
    HEAP[4] = 0b0000010; // 2 Block utilisé 
    HEAP[5] = 0b1000000; // block utilisé, donnée = 128 

    my_malloc(sizeof(uint8_t));

    /* uint8_t a = reset_lsb(0b00000001);
    uint8_t b = reset_lsb(0b10000001);
    uint8_t c = reset_lsb(0b11111110);
    uint8_t d = reset_lsb(0b00001111);

    print_block(a);
    printf("\nNow b\n");
    print_block(b);
    printf("\nNow c\n");
    print_block(c);
    printf("\nNow d\n");
    print_block(d);
 */
   // HEAP[0] = 0b0001000;
   // print_block(HEAP[0]);
    
    return 0;
}

