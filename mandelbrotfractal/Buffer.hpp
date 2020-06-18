//
//  Buffer.hpp
//  mandelbrotfractal
//
//  Created by Luis Alberto Zagonel Pozenato on 14/06/20.
//  Copyright © 2020 LuisPoze. All rights reserved.
//

#ifndef Buffer_h
#define Buffer_h
#include "Task.hpp"
#include <vector>
#include <algorithm>

using namespace std;

class Buffer
{
    
public:
    const char* name = new char[20];
    Task* task;
    vector<Task*> buffer;
    int length;
    
    Buffer(const char* name, int length)
    {
        this->name = name;
        task = nullptr;
        this->length = length;
    }
    
    void add(Task *task) {
        if(buffer.size() < length){
            buffer.push_back(task);
        }
    }
    
    Task* get() {
        task = buffer.front();
        buffer.erase(buffer.begin());
        return task;
    }
        
    bool isFull()
    {
        return buffer.size() >= length;
    }
    
    bool isEmpty()
    {
        return buffer.size() <= 0;
    }
        
    void print()
    {
        cout << name << ": ";
        for (int i = 0; i < buffer.size(); i++) {
            buffer[i]->print();
        }
        cout << endl;
    }
    
    void randIt()
    {
        std::random_shuffle(buffer.begin(), buffer.end());
    }

};


#endif /* Buffer_h */
