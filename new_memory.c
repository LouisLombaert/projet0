#include <stdint.h>
#include <stdio.h>

uint8_t MY_HEAP[64000]; 

void init_memory() { // Dans les meta, on écrit la taille du tab donc nb de block devant lui (libre ou pas)
    uint8_t high_byte = ((64000-2) >> 8) & 0xFF; // SIZE
    uint8_t low_byte = (64000-2) & 0xFF; // SIZE

    MY_HEAP[0] = high_byte;
    MY_HEAP[1] = low_byte;
}

void *my_malloc(size_t size) {
    uint8_t adjust = size % 2; // +1 ou 0 block en plus si pair ou impair

    for (size_t i = 0; i < 64000; i+=2) { // SIZE

        printf("Size de HEAP[%ld]+HEAP[%ld]: %u\n",i, i+1, (MY_HEAP[i] << 8) | MY_HEAP[i+1]); // DELETE

        if((MY_HEAP[i+1] & 1) == 0) { // Reg si le block est free / (0=free et 1=use)
            printf("Block dispo: %u >= Taille a stocké: %lu + Adjust: %u\n", ((MY_HEAP[i] << 8) | MY_HEAP[i+1]), size, adjust); // DELETE

            if(((MY_HEAP[i] << 8) | MY_HEAP[i+1]) >= size+adjust) { // Reg si assez grand.
                
                if(i+2+size+adjust < 64000) { // trjs un block a coté (si taille max, pas ecrire apres) // SIZE
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

void my_free(void *pointer) {
    if(pointer == NULL) return; // check au cas ou

    // Recup 2 block de metadonné
    uint8_t* metadata_ptr1 = ((uint8_t*)pointer) - 2; 
    uint8_t* metadata_ptr2 = ((uint8_t*)pointer) - 1;

    // Check if it's already free using the least significant bit of metadata_ptr2
    if (!(*metadata_ptr2 & 1)) { 
        printf("Warning: Double free detected.\n");
        return;
    }   

    *metadata_ptr2 &= ~1; // Reset le flag

    size_t total = ((*metadata_ptr1) << 8) | *metadata_ptr2;

    uint8_t* current_ptr = (uint8_t*) metadata_ptr1;

    while ((current_ptr - MY_HEAP + total) < 16) { // Merge block devant 
        uint8_t* next_metadata_ptr1 = current_ptr + total - 2;
        uint8_t* next_metadata_ptr2 = current_ptr + total - 1;

        if (*next_metadata_ptr2 & 1) { break; } // Si pas de block free, stop merge

        uint16_t next_total = ((*next_metadata_ptr1) << 8) | *next_metadata_ptr2; // Total de place 
        total += next_total;

        current_ptr += next_total;
    }

    *metadata_ptr1 = (total >> 8) & 0xFF;
    *metadata_ptr2 = total & 0xFF;
}

int main(int argc, char const *argv[])
{
    init_memory();
    
    my_malloc(sizeof(uint8_t) * 1);

    printf("%u\n",(MY_HEAP[0] << 8) | MY_HEAP[1]);
    printf("%u\n",(MY_HEAP[4] << 8) | MY_HEAP[5]);

    void* allocated_ptr = my_malloc(sizeof(uint8_t) * 1);

    printf("%u\n",(MY_HEAP[0] << 8) | MY_HEAP[1]);
    printf("%u\n",(MY_HEAP[4] << 8) | MY_HEAP[5]);
    printf("%u\n",(MY_HEAP[8] << 8) | MY_HEAP[9]);

    if(allocated_ptr) { // Au cas ou ya plus de place (return null)
        uint8_t* data = (uint8_t*) allocated_ptr;

        printf("Set data malloced\n");
        for (int i = 0; i < 1; i++) {
            data[i] = 0b11111111;
        }
    }

    my_free(allocated_ptr);

    printf("%u\n",(MY_HEAP[0] << 8) | MY_HEAP[1]);
    printf("%u\n",(MY_HEAP[4] << 8) | MY_HEAP[5]);
    printf("%u\n",(MY_HEAP[8] << 8) | MY_HEAP[9]);

    my_malloc(sizeof(uint8_t) * 8);

    printf("%u\n",(MY_HEAP[0] << 8) | MY_HEAP[1]);
    printf("%u\n",(MY_HEAP[4] << 8) | MY_HEAP[5]);
    printf("%u\n",(MY_HEAP[14] << 8) | MY_HEAP[15]);

    return 0;
}
