#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


void mono (unsigned char* image, unsigned char* data, int sizeW, int sizeH, int n) 
{
    int size = sizeW * sizeH * n;
    int k = 0;
    for (int i = 0; i < size; i +=  n) 
    {
        data[k] = 0.299 * image[i] + 0.587 * image[i + 1] + 0.114 * image[i + 2];
        k++;
    }

}


void contrast (unsigned char* data, int sizeW, int sizeH, int black, int white) 
{
    for (int i = 2; i < sizeH-1; i++) 
    {
        for (int j = 2; j < sizeW-1; j++) 
        {
            if (data[sizeW * i + j] <= black - 1) data[sizeW * i + j] = 0;
            if (data[sizeW * i + j] >= white + 1) data[sizeW * i + j] = 255;
        }
    }
}


void dfs (int i, int j, int w, int h, unsigned char* data, int* v, int m)
{
    v[w * i + j] = m;
    if((i >= 1) && (i <= h - 1) && (j - 2 >= 1) && (j - 2 <= w - 1))
    {
        if((abs(data[w * i + j] - data[w * i + (j - 2)]) <= 60) && (v[w * i + (j - 2)] == 0))
        {
            dfs(i, j - 2, w, h, data, v, m);
        }
    }
    if((i - 2 >= 1) && (i - 2 <= h - 1) && (j + 1 >= 1) && (j + 1 <= w - 1))
    {
        if((abs(data[w * i + j] - data[w * (i - 2) + (j + 1)]) <= 60) && (v[w * (i - 2) + (j + 1)] == 0))
        {
            dfs(i - 2, j + 1, w, h, data, v, m);
        }
    }
    if((i + 2 >= 1) && (i + 2 <= h - 1) && (j + 1 >= 1) && (j + 1 <= w - 1))
    {
        if((abs(data[w * i + j] - data[w * (i + 2) + (j + 1)]) <= 60) && (v[w * (i + 2) + (j + 1)] == 0))
        {
            dfs(i + 2, j + 1, w, h, data, v, m);
        }
    }
}


int main() {
    char* inputPath = "hampster.png";
    int iw, ih, n, i = 0, j = 0, k = 0, m = 0, black = 100, white = 150;

    unsigned char *input_image = stbi_load(inputPath, &iw, &ih, &n, 0);
    if (input_image == NULL) 
    {
        printf("Can't read file %s\n", inputPath);
        return 0;
    }

    unsigned char* data = (unsigned char*)malloc(ih * iw * sizeof(unsigned char));
    int* vert = (int*) malloc(iw * ih * sizeof(int));
    if (data == NULL || vert == NULL)
    {
        printf("Memory allocation error at main()" );
        return 0;
    }
    
    mono(input_image, data, iw, ih, n);
    contrast(data, iw, ih, black, white);
    
    for(i = 0;i < iw * ih; i++)
    {
        vert[i] = 0;
    }
    for (i = 1; i <= ih - 1; i++)
    {
        for (j = 1; j <= iw - 1; j++)
        {
            if(vert[iw * i + j] == 0)
            {
                m += 1;
                dfs(i, j, iw, ih, data, vert, m);
            }
        }
    }
    
    unsigned char* odata=(unsigned char*)malloc(iw * ih * n * sizeof(unsigned char));
    int c;
    
    for (i = 0; i < ih * iw * n; i += n)
    {
        c = vert[k] % 63 + vert[k] % 27;
        odata[i] = 3 * c - 70;
        odata[i + 1] = 2 * c + 10;
        odata[i + 2] = 5 * c + 13;
        odata[i + 3] = 200;
        k++;
    }
    char* outputPath = "colored_hamster.png";
    stbi_write_png(outputPath, iw, ih, n, odata, 0);
    free(data);
    stbi_image_free(input_image);
    return 0;
}

