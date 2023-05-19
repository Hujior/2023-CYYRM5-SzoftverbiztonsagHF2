#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/libwebp/encode.h"

uint8_t* file_buffer;
long fb_size;
long curr_index = 0;

// fügvény deklarációk
uint64_t bytes_to_int(int index);
int parse_caff_header();
int parse_caff_credits();
int arse_caff_animation();


int webp_parser(uint8_t* rgb, int w, int h, int s, int q)
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

    int ret = 0;
    while(curr_index < fb_size)
    {
        // a block ID értéke alapján szétválasztom
        // switch statement arra az esetre, ha más sorrendben követnék egymást a blokkok
        switch(file_buffer[curr_index])
        {
        case 1: ret = parse_caff_header(); break;
        case 2: ret = parse_caff_credits(); break;
        case 3: ret = arse_caff_animation(); break;
        }

        if(ret == -1)
        {
            return -1;
        }
    }

    return 0;
    
}

//----------------------------------------------------------
int ciffParser(char* fileName)
{
    if(read_file(fileName) == -1)
    {
        return -1;
    }

    int width;
    int height;
    int s;
    int q;


    webp_parser(file_buffer[curr_index], width, height, s, q);

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

/*****************************************************
 * Segéd függvények 
******************************************************/

int parse_caff_header()
{
    long block_start = curr_index;
    long block_lenght = bytes_to_int(curr_index + 1);
    curr_index = curr_index + 9; // átugrom a 8 bájtos számot
    uint8_t magic[4];
    magic[0] = file_buffer[curr_index];
    magic[1] = file_buffer[curr_index+1];
    magic[2] = file_buffer[curr_index+2];
    magic[3] = file_buffer[curr_index+3];
    curr_index = curr_index + 4;
    long header_size = bytes_to_int(curr_index);
    curr_index = curr_index + 9;
    long num_anim = bytes_to_int(curr_index);
    curr_index = curr_index + 9;
    long block_end = curr_index - 1;

    //block méret ellenőrzés
    if((block_end - (block_start + 9)) != block_lenght)
    {
        return -1;
    }

    //magic ellenőrzés
    int magic_ok = -1; 
    if(magic[0] == 'C')
    {
        if(magic[1] == 'A')
        {
            if(magic[2] == 'F')
            {
                if(magic[3] == 'F')
                {
                    magic_ok = 0;
                }
            }
        }
    }
    return magic_ok;

    
}
int parse_caff_credits()
{
    return -1; //TODO
}
int arse_caff_animation()
{
    return -1; //TODO
}

uint64_t bytes_to_int(int index)
{
    //printf("%02X%02X%02X%02X%02X%02X%02X%02X\n", file_buffer[index], file_buffer[index + 1], file_buffer[index + 2], file_buffer[index + 3], file_buffer[index + 4], file_buffer[index + 5], file_buffer[index + 6], file_buffer[index + 7]);

    return (uint64_t)file_buffer[index] 
    | (uint64_t)(file_buffer[index + 1] << 8)
    | ((uint64_t)file_buffer[index + 2] << 16) 
    | ((uint64_t)file_buffer[index + 3] << 24) 
    | ((uint64_t)file_buffer[index + 4] << 32) 
    | ((uint64_t)file_buffer[index + 5] << 40) 
    | ((uint64_t)file_buffer[index + 6] << 48) 
    | ((uint64_t)file_buffer[index + 7] << 56);
}