//
//  Scene.hpp
//  mandelbrotfractal
//
//  Created by Luis Alberto Zagonel Pozenato on 28/05/20.
//  Copyright © 2020 LuisPoze. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

#include <vector>
#include "Area.hpp"

class Scene
{
public:
    
    GLfloat * uniformColorArray;
    vector<Area> areas;
    int w;
    int h;
    
    Scene(const unsigned int areaLength, const float* backgroundColor)
    {
        w = areaLength;
        h = areaLength;
        setAreaSize(backgroundColor);
    }
    
    ~Scene(){
        for (int i = 0; i < areas.size(); i++) {
            areas[i].clear();
        }
        delete[] uniformColorArray;
    }
    
    void setColor(GLfloat * colorArray, int i)
    {
        areas[i].setColor(colorArray);
    }
    
    void setUniformRandonColor(int i)
    {
        GLfloat r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        GLfloat g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        GLfloat b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        uniformColorArray = Area::uniformColorArray(r, g, b, w, h);
        setColor(uniformColorArray, i);
    }
    
    unsigned long int size(){
        return areas.size();
    }
    
    unsigned long int addArea(int x, int y)
    {
        Area * area = new Area();
        area->Setup(uniformColorArray, w, h, x, y);
        areas.push_back(*area);
        return areas.size() - 1ul;
    }
    
    void render() {
        for (int i = 0; i < areas.size(); i++) {
            areas[i].render();
        }
    }
    
    void changeView(Camera *camera, GLuint w, GLuint h){
        for (int i = 0; i < areas.size(); i++) {
            areas[i].changeView(camera, w, h);
        }
    }
    
    
private:
    void setAreaSize(const GLfloat* backgroundColor){
        GLfloat r = backgroundColor[0];
        GLfloat g = backgroundColor[1];
        GLfloat b = backgroundColor[2];
        uniformColorArray = Area::uniformColorArray(r, g, b, w, h);
    }
};

#endif /* Scene_h */
