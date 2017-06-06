//#include <stdint.h>
#include <stdio.h>

//#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
typedef struct arrayOfChars {
    unsigned char array[1024];
    int position;
} arrayOfChars;

typedef struct charByte {
    unsigned char byte;
    int numberOfBits;
} charByte;


arrayOfChars arrayToPrint;
charByte lsbByte;


int main(int argc, char* argv[])
{
    int taps[3] = {0, 3, 5};

    arrayToPrint.position = 0;
    lsbByte.numberOfBits = 0;
//    int numberOfTaps = NELEMS(taps);
    printf("number of elements = %d \n" + getArrayLength(taps));
    //printf("%d", sizeof ta)
    lfsr(7, taps, 3);


    return 0;
}


void lfsr(int length, int taps[], int numberOfTaps) {

    unsigned regstr = 0b10101001;
    int regstrFeed;
    long i;

    unsigned lsb;

    for (i = 0; i <  1024 * 8; i++) {
    //do {
        lsb = regstr & 1;
        saveLsb(lsb);
        regstr >>= 1;
        regstr ^= getRegstrFeed(regstr, taps, numberOfTaps);
        //printf("register = %d", regstr);
    } //while(regstr != 0b10101001);


}


int getRegstrFeed(int regstr, int taps[], int numberOfTaps) {
    int i;
    int regstrFeed = getBit(regstr, 7 - taps[0]);

    for (i = 1; i < numberOfTaps; i++) {
        regstrFeed ^= getBit(regstr, 7 - taps[i]);
    }

    return regstrFeed << 7;
}

int getBit(int number, int bitNumber) {
    int mask =  1 << bitNumber;
    int masked_n = number & mask;

    return masked_n >> bitNumber;
}

void saveLsb(unsigned lsb) {
    int i;
    int bitToAdd = lsb << 1; //lsb & 1;
    lsbByte.byte <<= 1;
    lsbByte.byte = lsbByte.byte | lsb;
    lsbByte.numberOfBits++;

    if (lsbByte.numberOfBits == 7) {
        lsbByte.numberOfBits = 0;

        arrayToPrint.array[arrayToPrint.position] = lsbByte.byte;
        arrayToPrint.position++;

        if (arrayToPrint.position == 1023) printArrayToFile ();
    }
}

void printArrayToFile () {
    printf("BBBBBBB");
    int i;
    for(i = 0; i < 1024 ; i++) {
        printf("Bit %d : %c\n", i, arrayToPrint.array[i]);
    }

    arrayToPrint.position = 0;
}

unsigned char convertByteToChar(unsigned byte) {
    unsigned char b = byte & 0xffl;
    return b;
}

void printCharInBits (char c) {
    int i;
    for (i = 0; i < 8; i++) {
        printf("%d", !!((c << i) & 0x80));
    }
    printf("\n");
}

int getArrayLength(int array[]) {
    int i = 0;

    while(array[i] != NULL && array[i] != '\0') i++;

    return i;
}
