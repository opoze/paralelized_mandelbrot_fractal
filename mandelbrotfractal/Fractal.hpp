//
//  Fractal.hpp
//  mandelbrotfractal
//
//  Created by Luis Alberto Zagonel Pozenato on 16/06/20.
//  Copyright © 2020 LuisPoze. All rights reserved.
//

#ifndef Fractal_hpp
#define Fractal_hpp

#include <complex>

short max_nb_iter = 34;
short colorMode = 1;

int* HsvToRgb(int h, int s, int v)
{
    int r;
    int g;
    int b;
    unsigned char region, remainder, p, q, t;

    if (s == 0)
    {
        r = v;
        g = v;
        b = v;
        return new int[3]{r,g,b};
    }

    region = h / 43;
    remainder = (h - (region * 43)) * 6;

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            r = v; g = t; b = p;
            break;
        case 1:
            r = q; g = v; b = p;
            break;
        case 2:
            r = p; g = v; b = t;
            break;
        case 3:
            r = p; g = q; b = v;
            break;
        case 4:
            r = t; g = p; b = v;
            break;
        default:
            r = v; g = p; b = q;
            break;
    }

    return new int[3]{r, g, b};
}

int* pixelValue(int x, int y, int width, int height)
{

    complex<float> point((float)x/width-1.5f, (float)y/height-1.0f);
    complex<float> z(0, 0);
    unsigned int nb_iter = 0;
    while (abs (z) < 2 && nb_iter <= max_nb_iter) {
           z = z * z + point;
           nb_iter++;
    }
    
    
    if(colorMode == 1) {
        
        int h = (255*nb_iter)/(max_nb_iter-1);
        int s = 255;

        int v = 0;

        if (nb_iter < max_nb_iter) {
            v = 255;
        }
        return new int[3]{h, s, v};
    }
    
    if(colorMode == 2){
        int n = 0;
        if (nb_iter == max_nb_iter){
            n = nb_iter;
        }
        else {
            n = nb_iter + 1 - log(log2(abs(z)));
        }
    
        int h = (255*n)/(max_nb_iter-1);
        int s = 255;
    
        int v = 0;
    
        if (n < max_nb_iter) {
            v = 255;
        }
        return new int[3]{h, s, v};
    }
    
    
    int h = (255*nb_iter)/(max_nb_iter-1);
    return new int[3]{h, 0, 0};

}

void fractal(float* colors, int x, int y, int width, int height, int areaLength)  {
    int colorOffset = 0;
    int pixel = 0;
    for (int i = x; i < x + areaLength; i++) {
        for (int j = y; j < y + areaLength; j++)  {
            int* HSV = pixelValue(i, j, width, height);
            
            float r , g, b;
            
            if(colorMode != 0) {
                int* RGB = HsvToRgb(HSV[0], HSV[1], HSV[2]);
                colorOffset = pixel * 3;
                r = (float)RGB[0] / 255.0f;
                g = (float)RGB[1] / 255.0f;
                b = (float)RGB[2] / 255.0f;
            }
            else {
                colorOffset = pixel * 3;
                r = (float)HSV[0] / 255.0f;
                g = (float)HSV[1] / 255.0f;
                b = (float)HSV[2] / 255.0f;
            }
            
            colors[colorOffset++] = r;
            colors[colorOffset++] = g;
            colors[colorOffset] = b;
            
            pixel ++;
        }
    }
}

#endif /* Fractal_hpp */
