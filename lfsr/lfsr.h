#ifndef LFSR_H_   /* Include guard */
#define LFSR_H_

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

#endif // LFSR_H_
