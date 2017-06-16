#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "lfsr.h"

void lfsr(int length, int taps[], int numberOfTaps) {

    arrayOfChars arrayToPrint;
    charByte lsbByte;
    initializeVariables(&arrayToPrint, &lsbByte);  //Initializes the arrayToPrint and lsbByte variables

    uint32_t regstr = 1; //0b00000000000000000000000000000001;
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


uint32_t getRegstrFeed(uint32_t regstr, int taps[], int numberOfTaps, int regstrLength) {
    int i;
    uint32_t regstrFeed = getBit(regstr, taps[0]);  //Gets the bit indicated in taps[0] to initialize variable

    for (i = 1; i < numberOfTaps; i++) {    //For every tap it does an XOR and saves the result in regstrFeed
        regstrFeed ^= getBit(regstr, taps[i]);
    }

    return regstrFeed << (regstrLength - 1);    //Returns the registerFeed bit in the most significant bit according to the length of the register
}

uint32_t getBit(uint32_t number, int bitNumber) {
    uint32_t mask =  1 << bitNumber;	    //  00000001 -> 00001000  (bitNumber = 3)
    uint32_t masked_n = number & mask;		// 0b10101001 &= 00001000 = 00001000

    return masked_n >> bitNumber;           // 00001000 -> 00000001  Returns the bit that we wanted as the least significant bit
}
/*
The logic behind saveLsb is that its going to save every bit produced by the lfsr in a char (lsbByte, 8 bits),
when the char reaches its maximum space then it saves that char in an array of chars (arrayToPrint),
sets the counter of the lsbByte to 0 so the whole process starts again.
For performance reasons, it only prints to the file once it has 1024 chars -> 1024 bytes,
so the size of the file requested must be a multiple of 1024
*/
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

