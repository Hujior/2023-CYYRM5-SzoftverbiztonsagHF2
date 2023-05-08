#include <stdio.h>
#include "../include/libwebp/encode.h"


int main() {
    printf("Hello World\n");
    const uint8_t* rgb;
    int w, h, s, q = 0;
    uint8_t** out;
    size_t t = WebPEncodeRGB(rgb, w, h, s, q, out);
    printf("the return: %ld\n", t);
    return 0;
}