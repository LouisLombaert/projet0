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

uint8_t set_lsb_used(int8_t x) { //  Set le dernier bit a 1 (met le flag)
    return x |= 1;
}

uint8_t set_lsb_free(uint8_t x) { // Set le dernier bit a 0 (retire le flag)
    return x &= ~1;
}


void *my_malloc(size_t size) {
    for (int i = 0; i < SIZE/2; i++) {
        printf("Number at HEAP[%d]: %u\n",i, HEAP[i]);
        if((HEAP[i] & 0b1) == 0) { // Reg si le block est free

                if(size % 2 == 0) { // +2

                    if(HEAP[i] >= size+2) { // Reg si taille suffisante
                        int last_size = HEAP[i];

                        printf("MALLOC\n");
                        printf("UTILISE %ld case\n", size+2);
                        
                        // Metadonnée -> flag a 1
                        HEAP[i] = set_lsb_used(HEAP[i]);
                        

                        // Prochain Metadonné-> size
                        printf("Avant set flag a HEAP[%lu] %u\n", i+2+size, HEAP[i+2+size]);
                        //HEAP[i+1+size] = set_lsb_free(HEAP[i+1+size]);

                        printf("%d - (%d + 2)\n", last_size, size);
                        HEAP[i+2+size] = last_size - (size+2);                    

                        printf("Apres set flag a HEAP[%lu] %u\n",  i+2+size, HEAP[i+2+size]);

                        // return pointer


                        break;
                    }


                } else { // +1

                    if(HEAP[i] >= size+1) { // Reg si taille suffisante
                        int last_size = HEAP[i];

                        printf("MALLOC\n");
                        printf("UTILISE %ld case\n", size+1);
                        
                        // Metadonnée -> flag a 1
                        HEAP[i] = set_lsb_used(HEAP[i]);
                        

                        // Prochain Metadonné-> size
                        printf("Avant set flag a HEAP[%lu] %u\n", i+1+size, HEAP[i+1+size]);
                        print_block(HEAP[i+1+size]);
                        //HEAP[i+1+size] = set_lsb_free(HEAP[i+1+size]);

                        printf("%d - (%d + 1)\n", last_size, size);
                        HEAP[i+1+size] = last_size - (size+1);                    

                        printf("Apres set flag a HEAP[%lu] %u\n",  i+1+size, HEAP[i+1+size]);
                        print_block(HEAP[i+1+size]);

                        // return pointer

                        
                        
                        break;
                    }

                }
            
                   
        } else {
            printf("RESET %u\n", reset_lsb(HEAP[i])); 
            i += reset_lsb(HEAP[i]) -1; // JUMP DE n BLOCK
        }
    }
    
}

int main(int argc, char const *argv[])
{
    init_memory();

    HEAP[0] = 0b0010000; // 4 block utilisé
    HEAP[1] = 0b0000000; // donnée 
    HEAP[2] = 0b0000011; // donnée
    HEAP[3] = 0b0000000; // donnée
    HEAP[4] = 0b1001110; // 12 Block free 
    HEAP[5] = 0b1000000; // block free

    // de HEAP[6] -> HEAP[16] : free 

    my_malloc(sizeof(uint8_t)*4);

/*     print_block(HEAP[0]);
    print_block(HEAP[3]);
    print_block(HEAP[4]); */

   // HEAP[0] = 0b0001000;
   // print_block(HEAP[0]);
    
    return 0;
}

