//
//  main.hpp
//  mandelbrotfractal
//
//  Created by Luis Alberto Zagonel Pozenato on 25/05/20.
//  Copyright © 2020 LuisPoze. All rights reserved.
//

#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <pthread.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Camera.hpp"
#include "Scene.hpp"
#include "Buffer.hpp"
#include "Task.hpp"

GLFWwindow * window;

int WIDTH = 800;
int HEIGHT = 600;
int fbWidth = 0 ;
int fbHeight = 0;

Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 2.0f));
Scene* scene;

// TODO: apagar buffers no final
// TODO: fractal somente da metade
Buffer* workPollBuffer;
Buffer* doneBuffer;
Buffer* tasks;

pthread_mutex_t my_mutex;

pthread_cond_t work_pool_buffer_not_full;
pthread_cond_t work_pool_buffer_not_empty;
pthread_cond_t done_buffer_not_full;
pthread_cond_t done_buffer_not_empty;

bool finish;
int tasksDone = 0;
int tasksQty = 0;

const short AREA_LENGTH = 50;
const short N_THREADS = 2;
const short WORK_POOL_BUFFER_LENGTH = 2; // quantas tasks contem no buffer
const short DONE_BUFFER_LENGTH = 20;
const float* BACKGROUND_COLOR = new float[3]{.0f, 1.0f, 1.0f};
const short WORKER_SLEEP_TIME_IN_SECONDS  = 0;
const bool SORT_TASKS = 1;

void onChangeWindowSize(GLuint w, GLuint h) {
    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    scene->changeView(camera, width, height);
    // TODO: chamar 1 render loop aqui
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    onChangeWindowSize(width, height);
}

void inicializeGraphics() {
    if(!glfwInit()) {
       exit(1);
   }
    window = glfwCreateWindow(WIDTH, HEIGHT, "PAD - GB - Luis Pozenato - Unisinos - Righi - 2020", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
}

void setup()
{
    inicializeGraphics();
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    int xAreas = fbWidth/AREA_LENGTH; // 8
    int yAreas = fbHeight/AREA_LENGTH; // 6
    tasksQty = xAreas * yAreas;
    
    tasks = new Buffer("TaskArray", tasksQty);
    scene = new Scene(AREA_LENGTH, BACKGROUND_COLOR); // cria sena com areas de 100x100 pixels
    int x;
    int y;
    int index = 0;
    
    for (int i = 0; i < xAreas; i++) {
        for (int j = 0; j < yAreas; j++) {
            x = i * AREA_LENGTH;
            y = j * AREA_LENGTH;
            scene->addArea(x, y);
            tasks->add(new Task(x, y, fbWidth, fbHeight, AREA_LENGTH, index++));
        }
    }
    
    if(SORT_TASKS){
        tasks->randIt(); // tasks will act like FIFO, so rand it to make sweet effects on screen
    }
    
    workPollBuffer = new Buffer("WorkPoll", WORK_POOL_BUFFER_LENGTH);
    doneBuffer = new Buffer("DoneBuffer", DONE_BUFFER_LENGTH);
}

void finalizeGraphics() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void renderGraphics() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->render();
}

void graphicLoop()
{
    Task *task;
    onChangeWindowSize(WIDTH, HEIGHT);
    double elapseTime, savedTime, updateTime, currentTime;

    savedTime = 0;
    updateTime = 0;
    const double updateInterval = 1.0/60.0;

    
    while (!glfwWindowShouldClose(window))
    {
        
        // Para controlar o framerate
        currentTime =  glfwGetTime();
        elapseTime = currentTime - savedTime;
        savedTime = currentTime;
        updateTime += elapseTime;

        if(updateTime >= updateInterval){
            pthread_mutex_lock(&my_mutex);
            while (doneBuffer->isEmpty() && tasksDone < tasksQty) {
               pthread_cond_wait(&done_buffer_not_empty, &my_mutex);
            }
            if(tasksDone < tasksQty) {
                task = doneBuffer->get();
                printf("Ploter %ld: plotting task ", (long)pthread_self());
                scene->setColor(task->getColors(), task->getIndex());
                tasksDone++;
                task->print();
                printf("\n");
            }
            pthread_mutex_unlock(&my_mutex);
            pthread_cond_signal(&done_buffer_not_full);
            
            glfwPollEvents();
            renderGraphics();
            glfwSwapBuffers(window);
        }
        
    }
    
    finalizeGraphics();
    finish = true;
    pthread_cond_broadcast(&work_pool_buffer_not_empty);
    pthread_cond_broadcast(&work_pool_buffer_not_full);
    pthread_cond_broadcast(&done_buffer_not_full);

}

static void* producer(void* str)
{
    while (!tasks->isEmpty()) {
        // mutex para consistencia
        pthread_mutex_lock(&my_mutex);
        // verifica se o buffer esta cheio
        while (workPollBuffer->isFull()) {
            // Buffer cheio aguarda sinal de nao cheio para trabalhar
            pthread_cond_wait(&work_pool_buffer_not_full, &my_mutex);
            if(finish){
               break;
            }
        }
        if(finish){
            pthread_mutex_unlock(&my_mutex);
            break;
        }
        // neste ponto o buffer tem espaço e tem-se o mutex, gera trabalho
        // verifica se a tarefas penentes
        if(!tasks->isEmpty()){
            // put a task into the task buffer
            printf("Producer %ld: working \n", (long)pthread_self());
            workPollBuffer->add(tasks->get());
            printf("Producer %ld: done \n", (long)pthread_self());
            // libera mutex
            pthread_mutex_unlock(&my_mutex);
            // notifica que tem intens no buffer
            pthread_cond_signal(&work_pool_buffer_not_empty);
        } else {
            printf("Producer %ld: no more tasks \n", (long)pthread_self());
            pthread_mutex_unlock(&my_mutex);
            break;
        }
    }
    printf("Producer %ld: finish \n", (long)pthread_self());
    return 0;
}
    
static void* worker(void* str)
{
    Task* task = nullptr;
    while (1) {
        //########## Mutex mutex ##########
        pthread_mutex_lock(&my_mutex);
        while (workPollBuffer->isEmpty()) {
            printf("Worker %ld: WAITING \n", (long)pthread_self());
            pthread_cond_wait(&work_pool_buffer_not_empty, &my_mutex);
            printf("Worker %ld: RELEASED \n", (long)pthread_self());
            if(finish){
                break;
            }
        }
        if(finish){
            pthread_mutex_unlock(&my_mutex);
            break;
        }
        
        printf("Worker %ld: getting from workPool \n", (long)pthread_self());
        task = workPollBuffer->get();
        pthread_mutex_unlock(&my_mutex);
        pthread_cond_signal(&work_pool_buffer_not_full);
        
        printf("Worker %ld: working on task \n", (long)pthread_self());
        task->computeFractal();
        sleep(WORKER_SLEEP_TIME_IN_SECONDS);
        printf("Worker %ld: task done \n", (long)pthread_self());
    
        //########## Mutex mutex ##########
        pthread_mutex_lock(&my_mutex);
        while (doneBuffer->isFull()) {
            pthread_cond_wait(&done_buffer_not_full, &my_mutex);
            
            if(finish){
                break;
            }
            
        }
        if(finish){
            pthread_mutex_unlock(&my_mutex);
            break;
        }
        printf("Worker %ld: adding task on doneBuffer \n", (long)pthread_self());
        doneBuffer->add(task);
        pthread_mutex_unlock(&my_mutex);
        pthread_cond_signal(&done_buffer_not_empty);
        printf("Worker %ld: done \n", (long)pthread_self());
    }
    return 0;
}
    
int main(int argc, const char * argv[]) {
    setup();

    // pthreads mutex
    pthread_mutex_init(&my_mutex, nullptr);
    
    // pthreads sync conditions
    pthread_cond_init(&work_pool_buffer_not_full, nullptr);
    pthread_cond_init(&work_pool_buffer_not_empty, nullptr);
    pthread_cond_init(&done_buffer_not_full, nullptr);
    pthread_cond_init(&done_buffer_not_empty, nullptr);

    // threads
    pthread_t producer_tid;
    pthread_t* workers_tid = new pthread_t[N_THREADS];
    pthread_create(&producer_tid, nullptr, producer, nullptr);
    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&workers_tid[i], nullptr, worker, nullptr);
    }

    // main graphic loop
    graphicLoop();

    // pthreads join
    pthread_join(producer_tid, nullptr);
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(workers_tid[i], nullptr);
    }
    exit(0);
}
