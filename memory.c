#include <stdint.h>
#include <stdio.h>

uint8_t SIZE = 16;
uint8_t HEAP[16]; 

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


void print_memory(uint8_t heap[]) {
    printf("+");
    for (int i = 0; i < SIZE; i++) {
        printf("-------+");
    }
    printf("\n|");

    int i = 0;
    while (i < SIZE) {
        // Determine the number of total blocks and data blocks for the current allocation
        int total_blocks = heap[i] - 1; // Including the metadata block
        int data_blocks = total_blocks - 1; // Excluding the metadata block

        if (heap[i] & 1) {  // Used block
            printf(" %03dU |", total_blocks);
        } else {  // Free block
            printf(" %03dF |", total_blocks);
        }

        // Move to the next metadata block
        i++;
        for (int j = 1; j <= data_blocks; j++, i++) {
            if (i < SIZE) {
                printf("  /    |");
            }
        }
    }

    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        printf("+-------");
    }
    printf("+\n");
}


void *my_malloc(size_t size) {
    uint8_t adjust = (size % 2) ? 1 : 2; // +1 ou 2 block en plus si pair ou impair

    for (int i = 0; i < SIZE; i++) {
        //printf("Number at HEAP[%d]: %u\n",i, HEAP[i]);  // DELETE

        if((HEAP[i] & 0b1) == 0) { // Reg si le block est free

            if(HEAP[i] >= size+adjust) { // Reg si assez de place (size + ajustement + metadonné)
                
                HEAP[i+adjust+size] = HEAP[i] - (size+adjust); // Prochain block de metadonné -> taille de ce block - size

                printf("MALLOC UTILISE %ld case\n", size+adjust);

                HEAP[i] = size+adjust;
                HEAP[i] |= 1; // Metadonnée -> flag a 1

                return &HEAP[i+1];
            } else {
                //printf("NO PLACE, JUMP OF %u BLOCKS\n", HEAP[i]-1); // DELETE
                i += (HEAP[i]-1); // JUMP DE n BLOCK (-1 pcq ca fait +1 au debut de la boucle)
            }           
                   
        } else {
            //printf("NOT FREE, JUMP OF %u BLOCKS\n", HEAP[i]-2); // DELETE
            i += (HEAP[i]-2); // JUMP DE n BLOCK (-1 pcq le block est utilisé donc LSB=1 alors que la taille est multiple de 2) (-1 pcq ca fait +1 au debut de la boucle)
        }
    }

    printf("No place available\n");
    return NULL;
}

void my_free(void *ptr) {
    
}

void free_memory(int idx){
    //idx: index du premier bloc DE DONÉE
    //vérif si i == 0 ?
    printf("flag: %d\n", HEAP[idx-1] & 0x01);
    if (HEAP[idx-1] & 0x01){/*  */
        
        printf("avant free: %u\n", HEAP[idx-1]);
        HEAP[idx-1] = HEAP[idx-1] & 0xFFFFFFFE;  //bloc de métadonée, on passe le flag 0
        printf("apres free: %u\n", HEAP[idx-1]);
        uint8_t next = HEAP[HEAP[idx-1]+idx-1];
        printf("prochain bloc de metadonee (HEAP[%d]): %u\n", HEAP[idx-1]+idx-1, next);
        //répéter cette action tanq que la prochaine zone n'est pas libre
        while (!(next & 0x01)){ //prochaine zone libre ?
            //Fusion 
            HEAP[idx-1] = HEAP[idx-1] + next;
            next = HEAP[HEAP[idx-1]+idx-1];
            printf("Fusion, nouvelle taille: %u. Prochain bloc de metadonee (HEAP[%d]): %u\n", HEAP[idx-1], HEAP[idx-1]+idx-1, next);
        }
    }
    else {
        //on vérifie que la zone n'est pas déjà libre
        printf("Already free");
    }
}

void my_free(void *ptr) {
    if(ptr == NULL) return; // check au cas ou

    uint8_t* metadata_ptr = ((uint8_t*)ptr) - 1; // Recup de metadata

    if (!(*metadata_ptr & 1)) { // Check si c'est deja free
        printf("Warning: Double free detected.\n");
        return;
    }

    *metadata_ptr &= ~1; // Reset le flag

    uint8_t* current_ptr = metadata_ptr;
    uint8_t total = *metadata_ptr;

    while ((current_ptr - HEAP + total) < SIZE) {
        uint8_t* next_metadata_ptr = current_ptr + total;

        if (*next_metadata_ptr & 1) { break; } // next block is used, so stop merging

        total += (*next_metadata_ptr & ~1);  // Add size
    }

    *metadata_ptr = total;
}

int main(int argc, char const *argv[])
{
    init_memory();

    print_memory(HEAP);

    my_malloc(sizeof(uint8_t) * 3);
    my_malloc(sizeof(uint8_t));

    void* allocated_ptr = my_malloc(sizeof(uint8_t) * 4);

    void* other_malloc = my_malloc(sizeof(uint8_t)); // +1 block (1 meta + 1 data)

    my_malloc(sizeof(uint8_t));
    my_malloc(sizeof(uint8_t));

    if(allocated_ptr) { // Au cas ou ya plus de place (return null)
        uint8_t* data = (uint8_t*) allocated_ptr;

        printf("Set data malloced\n");
        for (int i = 0; i < 4; i++) {
            data[i] = 0b11111111;
        }
    }

    print_memory(HEAP);

    printf("Free\n");
    my_free(other_malloc);

    print_memory(HEAP);

    my_free(allocated_ptr); // 4 block: 1 metdonné + 4 donné + 1 vide
    
    print_memory(HEAP);

    my_malloc(sizeof(uint8_t)*7);

    print_memory(HEAP);
    
    return 0;
}

