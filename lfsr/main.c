#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
typedef struct arrayOfChars {
    unsigned char array[1024];
    int position;
} arrayOfChars;

typedef struct charByte {
    unsigned char byte;
    int numberOfBits;
} charByte;

void lfsr(int, int[],int);
uint32_t getRegstrFeed(uint32_t, int [], int, int);
uint32_t getBit(uint32_t, int );
void saveLsb(unsigned, arrayOfChars*, charByte*);
void printArrayToFile(arrayOfChars*);
unsigned char convertByteToChar(unsigned);
void printCharInBits(char);
int getArrayLength(int []);
uint32_t generateRegstr(int);
void initializeVariables(arrayOfChars*, charByte*);

int main(int argc, char* argv[]){
    //int taps[3] = {0, 3, 5};
    //printf("E: %d\n",argc-1);
    int taps[argc-1];
    int i;
    if(argc -1 > 0)
        for(i=0;i < argc-1; i++)
            taps[i]= atoi(argv[i+1]);
    else
    {
        printf("No taps where provided to feed the register.");
        exit(EXIT_FAILURE);
    }
//  int numberOfTaps = NELEMS(taps);
    printf("number of elements = %d \n" , sizeof(taps)/sizeof(int));
    lfsr(10, taps, sizeof(taps)/sizeof(int));

    return 0;
}

void lfsr(int length, int taps[], int numberOfTaps) {

    arrayOfChars arrayToPrint;
    charByte lsbByte;
    initializeVariables(&arrayToPrint, &lsbByte);

    uint32_t regstr = 1; //0b11010001110100011001000111010001;//generateRegstr(length);
    uint32_t regstrFeed;
    long i;

    unsigned lsb;

    for (i = 0; i <  8 * 1024; i++) {
        lsb = regstr & 1;   //Gets the least significant bit
        saveLsb(lsb, &arrayToPrint, &lsbByte);  //Saves the lsb to the array thats going to be printed
        regstrFeed = getRegstrFeed(regstr, taps, numberOfTaps, length); //Gets the bit thats going to feed the register
        regstr >>= 1;   //Shifts the register one bit to the right
        regstr ^= regstrFeed;   //Adds the register feed bit to the register
    }
}

void initializeVariables(arrayOfChars* arrayToPrint, charByte* lsbByte) {
    arrayToPrint->position = 0;
    lsbByte->byte = 0b00000000;
    lsbByte->numberOfBits= 0;
}

uint32_t generateRegstr(int length) {
    uint32_t regstr = 0;
    unsigned pulse = 0b00000001;
    return regstr | pulse;
}

uint32_t getRegstrFeed(uint32_t regstr, int taps[], int numberOfTaps, int regstrLength) {
    int i;
    uint32_t regstrFeed = getBit(regstr, taps[0]);

    for (i = 1; i < numberOfTaps; i++) {
        regstrFeed ^= getBit(regstr, taps[i]);
    }

    return regstrFeed << (regstrLength - 1);
}

uint32_t getBit(uint32_t number, int bitNumber) {
    uint32_t mask =  1 << bitNumber;	    //  00000001 -> 00000010
    uint32_t masked_n = number & mask;		// 0b10101001 &= 00001000 = 00001000

    return masked_n >> bitNumber;
}

void saveLsb(unsigned lsb, arrayOfChars* arrayToPrint, charByte* lsbByte) {
    lsbByte->byte <<= 1;
    lsbByte->byte = lsbByte->byte | lsb;
    lsbByte->numberOfBits++;

    if (lsbByte->numberOfBits == 8) {
        lsbByte->numberOfBits = 0;
        arrayToPrint->array[arrayToPrint->position] = lsbByte->byte;
        arrayToPrint->position++;
        if (arrayToPrint->position == 1024) printArray(arrayToPrint);
    }
}

void printArray(arrayOfChars* arrayToPrint) {
//    int i;
//    for(i = 0 ; i < 1024 ; i++){
//    	//printf("Bit %d : %c\n", i, arrayToPrint->array[i]);
//    	printCharInBits(arrayToPrint->array[i]);
//    	if (i % 4 == 3) printf("\n");
//    }

    int i;
    int j;
    int k;
    FILE* arch;

    if( (arch = fopen("lfsr.txt","wt") ) == NULL)
    {
        printf("ERROR CREATING FILE\n");
        exit(EXIT_FAILURE);                                     // 1
    }
    for(i = 0 ; i < 1024 ; i++){
    	fprintf(arch , "Bit %d : ", i);
    	for (j = 0; j < 3; j++, i++) {
            for (k = 0; k < 8; k++) {
                fprintf(arch, "%d", !!((arrayToPrint->array[i] << k) & 0x80));
            }
    	}
    	fprintf(arch, "\n");
    }
}

void printArrayToFile(arrayOfChars* arrayToPrint) {
    int i;
    FILE* arch;
    FILE* arch2;
    if( (arch = fopen("lfsr.txt","wt") ) == NULL)
    {
        printf("ERROR CREATING FILE\n");
        exit(EXIT_FAILURE);
    }
    for(i = 0 ; i < 1024 ; i++){
    	fprintf(arch , "Bit %d : %c\n", i, arrayToPrint->array[i]);
    }
    /**ARCHIVO QUE ESTOY PROBANDO PARA VER SI RESPETA LOS TAMAÑOS*/
    if( (arch2 = fopen("lfsraux1.txt","wt") ) == NULL)
    {
        printf("ERROR CREATING FILE\n");
        exit(EXIT_FAILURE);
    }
    for(i = 0 ; i < 1024 ; i++){
    	fprintf(arch2 ,"%c", arrayToPrint->array[i]);
    }
    /**---------------------------------------*/
    fclose(arch2);
    fclose(arch);
    arrayToPrint->position = 0;
}


/*unsigned char convertByteToChar(unsigned byte) {
    unsigned char b = byte & 0xffl;
    return b;
}
*/

void printCharInBits (char c) {
    int i;
    for (i = 0; i < 8; i++) {
        printf("%d", !!((c << i) & 0x80));
    }
    printf("\n");
}



/*int getArrayLength(int array[]) {
    int i = 0;
    while(array[i] != NULL && array[i] != '\0')
        i++;
    return i;
}
*/
