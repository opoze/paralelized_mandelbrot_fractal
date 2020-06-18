//
//  Task.h
//  mandelbrotfractal
//
//  Created by Luis Alberto Zagonel Pozenato on 14/06/20.
//  Copyright © 2020 LuisPoze. All rights reserved.
//

#ifndef Task_hpp
#define Task_hpp

#include "Fractal.hpp"

class Task
{
    
public:
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    int index = 0;
    int areaLength;
    float* colors;
    
    Task(int x, int y, int w, int h, int areaLength, int index)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->areaLength = areaLength;
        this->index = index;
    }
    
    int getX()
    {
        return x;
    }
    
    int getY()
    {
        return y;
    }
    
    int getIndex()
    {
        return index;
    }
    
    float* getColors()
    {
        return colors;
    }
    
    void computeFractal()
    {
        colors = new float[w*h*3];
        // TODO: passar aqui o tamanho da janela e nao o fbWidth
        fractal(colors, x, y, 800, 600, areaLength);
    }
    
    void print()
    {
        printf("(X: %d, Y: %d), ", x, y);
    }
    
};
#endif /* Task_hpp */
