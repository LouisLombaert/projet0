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
    for (int i = 0; i < 8; i++) {
        printf("Number at HEAP[%d]: %u\n",i, HEAP[i]);
        if((HEAP[i] & 0b1) == 1) {
            printf("%u\n", HEAP[i]);
            printf("RESET %u\n", reset_lsb(HEAP[i]));
        }
    }
    

  /*   while ((HEAP[index] & 0b1) == 1) {
        
        index 
    } */
    

}





int main(int argc, char const *argv[])
{
    init_memory();

    HEAP[0] = 0b0001000; // 8
    HEAP[1] = 0b0001001; // 9
    HEAP[2] = 0b0001011; // 11
    HEAP[3] = 0b1001001; // 137

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

