#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "lfsr.h"
//#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

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

