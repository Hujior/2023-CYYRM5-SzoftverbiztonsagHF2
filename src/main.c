#include <stdio.h>
#include <string.h>
#include "../include/libwebp/encode.h"


void ciffParser(char* fileName)
{
    printf("CIFF %s\n", fileName);
}

void caffParser(char* fileName)
{
    printf("CAFF %s\n", fileName);
}


int main(int argc, char** argv) 
{

    // error if there is more or less input parameters
    if(argc != 3)
    {
        return -1;
    }

    // error if the first parameter wants to be bigger than allowed (protcting strcmp)
    if(strlen(argv[1]) > 6)
    {
        return -1;
    }
    

    if(0 == strcmp(argv[1], "-ciff")) 
    {
        ciffParser(argv[2]);
    }
    else if(0 == strcmp(argv[1], "-caff"))
    {
        caffParser(argv[2]);
    }
    else
    {
        return -1;
    }

    //printf("Hello World\n");
    const uint8_t* rgb;
    int w, h, s, q = 0;
    uint8_t** out;
    size_t t = WebPEncodeRGB(rgb, w, h, s, q, out);
    //printf("the return: %ld\n", t);
    return 0;
}