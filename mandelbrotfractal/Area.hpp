//
//  Square.cpp
//  mandelbrotfractal
//
//  Created by Luis Alberto Zagonel Pozenato on 27/05/20.
//  Copyright © 2020 LuisPoze. All rights reserved.
//

#ifndef Area_h
#define Area_h

#include "GL/glew.h"
#include "Shader.hpp"
#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>

using namespace std;

class Area {
    
public:
    Shader *shader;
    GLfloat *colorArray;
    GLfloat *vertexArray;
    GLuint *indexArray;
    
    unsigned int VAO, VBOv, EBO, VBOc;
    const unsigned int floatsPerVertice = 3;
    const unsigned int floatsPerColor = 3;
    unsigned int vertexArraySize;
    unsigned int indexArraySize;
    unsigned int colorArraySize;
    
    int x;
    int y;
    
    Area(){}
    
    void Setup(GLfloat * colorArray, int w, int h, int x, int y) {
    
        createVertexData(w, h);
        
        this->x = x;
        this->y = y;
        
        shader = new Shader(
            "/Users/luispozenato/Desktop/PADUnisinos2020/mandelbrotfractal/mandelbrotfractal/shaders/transformations-color.vs",
            "/Users/luispozenato/Desktop/PADUnisinos2020/mandelbrotfractal/mandelbrotfractal/shaders/transformations-color.frag"
        );
        
        shader->Use();
        
        glGenVertexArraysAPPLE(1, &VAO);
        glBindVertexArrayAPPLE(VAO);
        
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArraySize * sizeof(GLuint), indexArray, GL_STATIC_DRAW);
        
        GLint vertexAttribLocation = glGetAttribLocation(shader->Program, "position");
        glGenBuffers(1, &VBOv);
        glBindBuffer(GL_ARRAY_BUFFER, VBOv);
        glBufferData(GL_ARRAY_BUFFER, vertexArraySize * sizeof(GLfloat), vertexArray, GL_STATIC_DRAW);
        glVertexAttribPointer(vertexAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(vertexAttribLocation);
        
        GLint colorAttribLocation = glGetAttribLocation(shader->Program, "color");
        glGenBuffers(1, &VBOc);
        glBindBuffer(GL_ARRAY_BUFFER, VBOc);
        glBufferData(GL_ARRAY_BUFFER, vertexArraySize * sizeof(GLfloat), colorArray, GL_STATIC_DRAW);
        glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(colorAttribLocation);

        glBindVertexArrayAPPLE(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram(0);
        
        delete[] vertexArray;
        delete[] indexArray;
        
    }
    
    
    void render()
    {
        glBindVertexArrayAPPLE(VAO);
        shader->Use();
        GLint transformLoc = glGetUniformLocation(shader->Program, "transform");
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3((GLfloat)this->x, (GLfloat)this->y, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawElements(GL_POINTS, indexArraySize, GL_UNSIGNED_INT, 0);
        glUseProgram(0);
        glBindVertexArrayAPPLE(0);
    }
    
    void changeView(Camera* camera, int w, int h) {
        glBindVertexArrayAPPLE(VAO);
        shader->Use();
        GLint viewLoc = glGetUniformLocation(shader->Program, "view");
        GLint projLoc = glGetUniformLocation(shader->Program, "projection");
        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = glm::ortho(0.0f, (GLfloat)w, (GLfloat)h, 0.0f, -2.0f, 2.0f);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUseProgram(0);
        glBindVertexArrayAPPLE(0);
    }
    
    void setColor(GLfloat * colorArray)
    {
        glBindVertexArrayAPPLE(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBOc);
        glBufferData(GL_ARRAY_BUFFER, vertexArraySize * sizeof(GLfloat), colorArray, GL_STATIC_DRAW);
        glBindVertexArrayAPPLE(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }
    
    static GLfloat* uniformColorArray(float r, float g, float b, int w, int h)
    {
        GLfloat * colorArray = new GLfloat[w * h * 3];
        for (int i = 0; i < (w * h * 3); i+=3) {
            colorArray[i] = r;
            colorArray[i+1] = g;
            colorArray[i+2] = b;
        }
        return colorArray;
    }
    
    void clear() {
        delete[] vertexArray;
        delete[] indexArray;
    }
    
protected:
    
    void createVertexData(int w, int h)
    {
        vertexArraySize = w * h * floatsPerVertice;
        vertexArray = new GLfloat[vertexArraySize];
        indexArraySize = w * h;
        indexArray = new GLuint[indexArraySize];
        
        int lineWidth = w * floatsPerVertice;
        int lineOffset = 0;
        int columnOffset = 0;
        int offset;
        GLuint index = 0;
        const float z = 0.0f;
                
        for (int line = 0; line < w; line++) {
            lineOffset = line * lineWidth;
            for (int column = 0; column < h; column++) {
                columnOffset = column * floatsPerVertice;
                offset = 0;
                vertexArray[lineOffset + columnOffset + offset++] = (GLfloat)line;
                vertexArray[lineOffset + columnOffset + offset++] = (GLfloat)column;
                vertexArray[lineOffset + columnOffset + offset] = (GLfloat)z;
                indexArray[index] = index;
                index++;
            }
        }
    }

};

#endif /* Area_h */
