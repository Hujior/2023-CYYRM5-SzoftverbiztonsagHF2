#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/libwebp/encode.h"

// globális változók
uint8_t* file_buffer;
long fb_size;
long curr_index = 0;
char* FileName;

// segéd fügvény deklarációk
uint64_t bytes_to_int(int index);
int parse_caff_header();
int parse_caff_credits();
int parse_caff_animation();


//-------------------------------------------------------
int webp_parser(int w, int h)
{
    const uint8_t* rgbp = file_buffer + curr_index;
    uint8_t** out;
    if(WebPEncodeRGB(rgbp, w, h, 0, 50, out) == 0)
    {
        return -1;
    }

    FILE* fwebp;
    fwebp = fopen(strcat(FileName, ".webp"), "wb" );

    if(fwebp == NULL)
    {
        WebPFree(*out);
        return -1;
    }

    fputs(*out, fwebp);
    WebPFree(*out);
    fclose(fwebp);
    
    return 0;
}

// -----------------------------------------------
int read_file(char* fileName)
{
    FILE* fptr;
    fptr = fopen(fileName, "rb");
    FileName = fileName;

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


    if(parse_caff_header() == -1)
    {
        return -1;
    }

    if(parse_caff_credits() == -1)
    {
        return -1;
    }

    if(parse_caff_animation() == -1)
    {  
        return -1;
    }


    return 0;
    
}

//----------------------------------------------------------
int ciffParser(char* fileName, int isCalledFromCAFF)
{
    if(!isCalledFromCAFF)
    {
        if(read_file(fileName) == -1)
        {
            return -1;
        }
    }

    int width;
    int height;

    uint8_t magic[4];
    magic[0] = file_buffer[curr_index];
    magic[1] = file_buffer[curr_index + 1];
    magic[2] = file_buffer[curr_index + 2];
    magic[3] = file_buffer[curr_index + 3];
    curr_index = curr_index + 4;
    
    int header_size = bytes_to_int(curr_index);
    curr_index = curr_index + 9;

    int content_size = bytes_to_int(curr_index);
    curr_index = curr_index + 9;

    width = bytes_to_int(curr_index);
    curr_index = curr_index + 9;

    height = bytes_to_int(curr_index);
    curr_index = curr_index + 9;

    int i = curr_index;
    while(file_buffer[i] != 10) // 0A = \n
    {
        i++;
    }

    int caption_size = i - curr_index;

    char* caption = (char*)malloc(caption_size * sizeof(char));

    for(int i = 0; i < caption_size; i++)
    {
        caption[i] = file_buffer[curr_index + i];
    }
    free(caption); // a feladatban nem csinálunk semmit az adatokkal

    int hg = 0; // header_size határ figyelő
    int tn = 0; // tag számláló
    int j = curr_index; // futó változó
    char** tags;
    while(hg < (header_size - (36 + caption_size))) // fix + dinamikus méretek a headerben korábban
    {
        while(file_buffer[j] != 0) // 00 = \0
        {
            hg++;
            j++;
        }

        tags[tn] = (char*)malloc((j - curr_index) * sizeof(char));
        for(int i = 0; i < (j - curr_index); i++)
        {
            *(tags[tn]) = file_buffer[curr_index + i];
        }

        tn++;
        curr_index = curr_index + hg + 1;
        j = curr_index;
    }

    for(int i = 0; i < tn; i++) // a feladatban nem csinálunk semmit az adatokkal
    {
        free(tags[i]);
    }
    free(tags);

    // magic ellenőrzés
    int magic_ok = 0; 
    if(magic[0] == 'C')
    {
        if(magic[1] == 'I')
        {
            if(magic[2] == 'F')
            {
                if(magic[3] == 'F')
                {
                    magic_ok = 1;
                }
            }
        }
    }
    

    // méret ellenőrzés
    if(magic_ok)
    {
        if(content_size == (width * height * 3))
        {
            return webp_parser(width, height);
        }
    }

    return -1;

}

//-------------------------------------------------------------
int main(int argc, char** argv) 
{

    int ret = 0;
    // hiba ha több vagy kevesebb input paraméter van
    if(argc != 3)
    {
        return -1;
    }

    // hiba ha az első paraméter nagyobb mint a megengedett (strcmp védelme)
    if(strlen(argv[1]) > 6)
    {
        return -1;
    }
    

    if(0 == strcmp(argv[1], "-ciff")) 
    {
        ret = ciffParser(argv[2], 0);
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
    uint8_t block_id = file_buffer[curr_index];
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

    //block tipus ellenőrzés
    if(block_id != 1)
    {
        return -1;
    }

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

//------------------------------------------------------------------------------
int parse_caff_credits()
{
    uint8_t block_id = file_buffer[curr_index];
    long block_start = curr_index;
    long block_lenght = bytes_to_int(curr_index + 1);
    curr_index = curr_index + 9;

    uint8_t yy[2];
    yy[0] = file_buffer[curr_index];
    yy[1] = file_buffer[curr_index + 1];
    uint8_t M = file_buffer[curr_index + 2];
    uint8_t D = file_buffer[curr_index + 3];
    uint8_t h = file_buffer[curr_index + 4];
    uint8_t m = file_buffer[curr_index + 5];
    curr_index = curr_index + 6;
    
    int creator_lenght = bytes_to_int(curr_index);
    curr_index = curr_index + 9;

    uint8_t* creator = (uint8_t*)malloc(creator_lenght * sizeof(uint8_t));
    for(int i = 0; i < creator_lenght; i++)
    {
        creator[i] = file_buffer[curr_index + i];
    }
    free(creator); // a háziban nem csinálunk semmit az adatokkal
    curr_index = curr_index + creator_lenght;

    long block_end = curr_index - 1;

    //block tipus ellenőrzés
    if(block_id != 2)
    {
        return -1;
    }

    //block méret ellenőrzés
    if((block_end - (block_start + 9)) != block_lenght)
    {
        return -1;
    }

    return 0;

}

//---------------------------------------------------------------
int parse_caff_animation()
{
    uint8_t block_id = file_buffer[curr_index];
    long block_start = curr_index;
    long block_lenght = bytes_to_int(curr_index + 1);
    curr_index = curr_index + 9;

    long duration = bytes_to_int(curr_index);
    curr_index = curr_index + 9;

    return ciffParser(NULL, 1); // a CAFF-ból hívásnál nem kell a fájlnév
}


//---------------------------------------------------------------------
uint64_t bytes_to_int(int index)
{
    
    return (uint64_t)file_buffer[index] 
    | (uint64_t)(file_buffer[index + 1] << 8)
    | ((uint64_t)file_buffer[index + 2] << 16) 
    | ((uint64_t)file_buffer[index + 3] << 24) 
    | ((uint64_t)file_buffer[index + 4] << 32) 
    | ((uint64_t)file_buffer[index + 5] << 40) 
    | ((uint64_t)file_buffer[index + 6] << 48) 
    | ((uint64_t)file_buffer[index + 7] << 56);
}