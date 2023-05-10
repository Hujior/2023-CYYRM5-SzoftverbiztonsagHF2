#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/libwebp/encode.h"

uint8_t* file_buffer;
int fb_size;


uint64_t bytes_to_int(int index)
{
    printf("%02X%02X%02X%02X%02X%02X%02X%02X\n", file_buffer[index], file_buffer[index + 1], file_buffer[index + 2], file_buffer[index + 3], file_buffer[index + 4], file_buffer[index + 5], file_buffer[index + 6], file_buffer[index + 7]);

    return (uint64_t)file_buffer[index] 
    | (uint64_t)(file_buffer[index + 1] << 8)
    | ((uint64_t)file_buffer[index + 2] << 16) 
    | ((uint64_t)file_buffer[index + 3] << 24) 
    | ((uint64_t)file_buffer[index + 4] << 32) 
    | ((uint64_t)file_buffer[index + 5] << 40) 
    | ((uint64_t)file_buffer[index + 6] << 48) 
    | ((uint64_t)file_buffer[index + 7] << 56);
}

int webp_parser()
{
    //const uint8_t* rgb;
    //int w, h, s, q = 0;
    //uint8_t** out;
    //size_t t = WebPEncodeRGB(rgb, w, h, s, q, out);
    //printf("the return: %ld\n", t);
    return 0;
}

// -----------------------------------------------
int read_file(char* fileName)
{
    FILE* fptr;
    fptr = fopen(fileName, "rb");

    if(fptr == NULL)
    {
        return -1;
    }

    fseek(fptr, 0L, SEEK_END);
    fb_size = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);

    file_buffer = malloc(fb_size);

    fread(file_buffer, 1, fb_size, fptr);
    fclose(fptr);

    return 0;
}

//-------------------------------------------------------
int caffParser(char* fileName)
{


    if(read_file(fileName) == -1)
    {
        return -1;
    }



    uint64_t length = bytes_to_int(93);
    printf("%ld\n", length * 2);
    printf("%d\n", fb_size);
    // CAFF check


    

    //printf("last element: %02X\n", file_buffer[fb_size - 1]);
    //printf("%02X%02X%02X%02X\n", file_buffer[0],file_buffer[1],file_buffer[2],file_buffer[3]);
    return 0;
    
}

//----------------------------------------------------------
int ciffParser(char* fileName)
{
    if(read_file(fileName) == -1)
    {
        return -1;
    }



    //webp_parser();

    printf("last element: %02X\n", file_buffer[fb_size - 1]);
    printf("%02X%02X%02X%02X\n", file_buffer[0],file_buffer[1],file_buffer[2],file_buffer[3]);

    return 0;
}

//-------------------------------------------------------------
int main(int argc, char** argv) 
{

    int ret = 0;
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
        ret = ciffParser(argv[2]);
    }
    else if(0 == strcmp(argv[1], "-caff"))
    {
        ret = caffParser(argv[2]);
    }
    else
    {
        ret = -1;
    }

    free(file_buffer);
    return ret;

}