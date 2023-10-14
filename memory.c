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

void *my_malloc(size_t size) {
    uint8_t adjust = (size % 2) ? 1 : 2; // +1 ou 2 block en plus si pair ou impair

    for (int i = 0; i < SIZE/2; i++) {
        //printf("Number at HEAP[%d]: %u\n",i, HEAP[i]);  // DELETE

        if((HEAP[i] & 0b1) == 0) { // Reg si le block est free

            if(HEAP[i] >= size+adjust) { // Reg si assez de place (size + ajustement + metadonné)
                
                HEAP[i+adjust+size] = HEAP[i] - (size+adjust); // Prochain block de metadonné -> taille de ce block - size

                printf("MALLOC\n");
                printf("UTILISE %ld case\n", size+adjust);

                HEAP[i] = size+adjust;
                HEAP[i] |= 1; // Metadonnée -> flag a 1

                // return pointer
                return &HEAP[i+1];
            }            
                   
        } else {
            printf("MUST JUMP OF %u BLOCKS\n", HEAP[i] -1);
            i += (HEAP[i]-1); // JUMP DE n BLOCK (-1 pcq le block est utilisé donc LSB=1 alors que la taille est multiple de 2)
        }
    }

    printf("No place available\n");
    return NULL;
    
}

void free_memory(int idx){
    //idx: index du premier bloc DE DONÉE
    //vérif si i == 0 ?
    printf("flag: %d\n", HEAP[idx-1] & 0x01);
    if (HEAP[idx-1] & 0x01){
        
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

int main(int argc, char const *argv[])
{
    init_memory();

    HEAP[0] = 0b00000100; // 4 free
    HEAP[1] = 0b00000001; // unused data.
    HEAP[2] = 0b00000010; // unused data.
    HEAP[3] = 0b00000011; // unused data.

    HEAP[4] = 0b00000011; // 2 used
    HEAP[5] = 0b11111111; // used data

    HEAP[6] = 0b00000010; // 2 free
    // de HEAP[7] -> HEAP[15] : unused data 


    //void* allocated_ptr = my_malloc(sizeof(uint8_t) * 4);
    HEAP[7] = 0b11111111;  // used data
    HEAP[8] = 0b00000010;  // used data
    HEAP[9] = 0b11111111;  // used data
    HEAP[10] = 0b11111111; // used data

    //uint8_t* data = (uint8_t*) allocated_ptr;

    /*printf("Data malloced\n");
    for (int i = 0; i < 4; i++) {
        printf("Printing %d-th block of the malloc one.\n", i);
        print_block(data[i]);
    }*/

    /*printf("Printing [12]\n");
    print_block(HEAP[12]); // 00000100 ?*/

    //test free
    //free_memory(1); //working
    free_memory(5);
    
    return 0;
}

