#include <stdint.h>
#include <stdio.h>

uint8_t MY_HEAP[256]; 

// ATM SIZE est 16 -> 64000

void init_memory() { // Dans les meta, on écrit la taille du tab, pas si ya assez de place apres pour stocké
    uint8_t high_byte = ((8-2) >> 8) & 0xFF;
    uint8_t low_byte = (8-2) & 0xFF; 

    MY_HEAP[0] = high_byte;
    MY_HEAP[1] = low_byte;
}

void print_block(uint8_t block) {
    for (int i = 0; i < 8; i++) {
        uint8_t bit = (block >> i) & 1;
        printf("Bit %d: %u\n", i, bit);
    }
}

void *my_malloc(size_t size) {
    uint8_t adjust = size % 2; // +1 ou 0 block en plus si pair ou impair

    for (size_t i = 0; i < 8; i+=2) {

        printf("Size de HEAP[%ld]+HEAP[%ld]: %u\n",i, i+1, (MY_HEAP[i] << 8) | MY_HEAP[i+1]); // DELETE

        if((MY_HEAP[i+1] & 1) == 0) { // Reg si le block est free / (0=free et 1=use)
            printf("Block dispo: %u >= Taille a stocké: %lu + Adjust: %u\n", ((MY_HEAP[i] << 8) | MY_HEAP[i+1]), size, adjust); // DELETE

            if(((MY_HEAP[i] << 8) | MY_HEAP[i+1]) >= size+adjust) { // Reg si assez grand.
                
                if(i+2+size+adjust < 8) { // trjs un block a coté (si taille max, pas ecrire apres)
                    size_t new_index = i+2+size+adjust;
                    size_t new_size = ((MY_HEAP[i] << 8) | MY_HEAP[i+1]) - (size+adjust) - 2;

                    printf("MALLOC, NEW TAILLE: %lu A HEAP[%lu]\n", new_size, new_index);

                    // Stocke les metadonnés du prochain block
                    MY_HEAP[new_index] = (uint8_t) ((new_size >> 8) & 0xFF);
                    MY_HEAP[new_index+1] = (uint8_t) (new_size & 0xFF); 
                }

                // Stocke la taille de ce malloc
                uint8_t high_byte = ((size+adjust) >> 8) & 0xFF;
                uint8_t low_byte = (size+adjust) & 0xFF; 

                low_byte |= 1;

                MY_HEAP[i] = high_byte;
                MY_HEAP[i+1] = low_byte;

                return &MY_HEAP[i+2];

            } else {
                printf("pas assez de place: %u\n", ((MY_HEAP[i] << 8) | MY_HEAP[i+1]));
                i += ((MY_HEAP[i] << 8) | MY_HEAP[i+1]);
            }

        } else {
            i += (((MY_HEAP[i] << 8) | MY_HEAP[i+1]) - 1); // -2 pcq c'est 1 flag et -1 debut de la loop
        }   

    }

    printf("No place available\n");
    return NULL;    
}

int main(int argc, char const *argv[])
{
    init_memory();

    printf("%u\n",(MY_HEAP[0] << 8) | MY_HEAP[1]);
    
    my_malloc(sizeof(uint8_t) * 1);

    printf("%u\n",(MY_HEAP[0] << 8) | MY_HEAP[1]);
    printf("%u\n",(MY_HEAP[6] << 8) | MY_HEAP[7]);

    void* allocated_ptr = my_malloc(sizeof(uint8_t) * 1);

    if(allocated_ptr) { // Au cas ou ya plus de place (return null)
        uint8_t* data = (uint8_t*) allocated_ptr;

        printf("Set data malloced\n");
        for (int i = 0; i < 1; i++) {
            data[i] = 0b11111111;
        }
    }

    return 0;
}
