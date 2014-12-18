//Copyright © 2014 Fred Mariowitsch Flügge <fmfluegge@gmail.com>
//This work is free. You can redistribute it and/or modify it under the
//terms of the Do What The Fuck You Want To Public License, Version 2,
//as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL\glut.h>
#endif

#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdio>

#define size32 1024
#define scale32 64
#define size16 256

int mode;
unsigned int container32[size32][size32];
unsigned int container16[size16][size16];
unsigned int maxCount16;
unsigned int maxCount32;

float toScale(unsigned int in){
    float ret = 0.0f;
    if (mode==0) {
        ret = (((double)in * (0.98084291187739f/1024.0f))+10*(0.98084291187739f/1024.0f));
    }
    if (mode==1) {
        ret = (((double)in * (0.92753623188406f/256.0f)))+10*(0.92753623188406f/256.0f);
    }
    
    return ret;
}

void process_Keys(unsigned char key, int x, int y)
{
    
    switch(key){
        case 27:exit(EXIT_SUCCESS);break;//esc
        case 49:
            mode=0;
            glutSetWindowTitle("fvis 32");
            glutReshapeWindow(1044, 1044);
            break;//key 1
        case 50:
            mode=1;
            glutSetWindowTitle("fvis 16");
            glutReshapeWindow(276, 276);
            break;//key 2
        default:
            printf("key_code =%d  \n",key);
            break;
    }
    
}

void draw(void){
    
    float ratio;
    int width= glutGet(GLUT_WINDOW_WIDTH);
    int height= glutGet(GLUT_WINDOW_HEIGHT);
    ratio = width / (float) height;
    
    glViewport(0, 0, width, height);
    
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f);
    glMatrixMode(GL_MODELVIEW);
    
    if (mode==0) { //32 bit scatterplott
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i<size32; i++) {
            for (unsigned int j = 0;j<size32;j++){
                if (container32[i][j]!=0) {
                    float colval=container32[i][j];
                    
                    colval = colval * (0.8f/maxCount32);
                    
                    glColor3f(0.2f, 0.2f+colval, 0.2f);
                    glVertex3f(toScale(i), toScale(j), 1.0f);
                }
            }
        }
        glEnd();
    }
    
    if (mode==1) { //16 bit scatterplott
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i<size16; i++) {
            for (unsigned int j = 0;j<size16;j++){
                if (container16[i][j]!=0) {
                    float colval=container16[i][j];
                    
                    colval = colval * (0.8f/maxCount16);
                    
                    glColor3f(0.2f, 0.2f+colval, 0.2f);
                    glVertex3f(toScale(i), toScale(j), 1.0f);
                }
            }
        }
        glEnd();
    }
    
    
    glFlush();
}

int main(int argc, char *argv[])
{
    mode = 0;
    
    if (argc < 2) {
        std::cout << "please give a file path" << std::endl;
        return 1;
    }
    /*if (argc > 2) {
        std::cout << "to many arguments" << std::endl;
        return 1;
    }*/
    std::cout << "plotting " << argv[1] << std::endl;
    
    //////////32bit
    for (int i=0;i<size32;i++) {
        for (int j=0;j<size32;j++){
            container32[i][j]=0;
        }
    }
    unsigned short buff32_1;
    unsigned short buff32_2;
    //Load File Stuff
    std::ifstream myfile32 (argv[1], std::ios::binary);
    //std::ifstream myfile32 ("testfile", std::ios::binary);
    if(myfile32.is_open()){
        while(!myfile32.eof()){
            myfile32.read((char*)&buff32_1,sizeof(buff32_1));
            myfile32.read((char*)&buff32_2,sizeof(buff32_2));
            container32[buff32_1/scale32][buff32_2/scale32]++;
        }
        myfile32.close();
    }else{
        std::cout << "file not found";
    }
    
    maxCount32=0;
    
    for (unsigned int i=0; i<size32; i++) {
        for (unsigned int j=0; j<size32; j++) {
            if (maxCount32<container32[i][j]) {
                maxCount32=container32[i][j];
            }
        }
    }
    
    /////////
    //////////16bit
    for (int i=0; i<(size16); i++) {
        for (int j=0; j<(size16); j++) {
            container16[i][j]=0;
        }
    }
    unsigned char buff16_1;
    unsigned char buff16_2;
    //Load File Stuff
    std::ifstream myfile16 (argv[1], std::ios::binary);
    //std::ifstream myfile16 ("testfile", std::ios::binary);
    
    if(myfile16.is_open()){
        while(!myfile16.eof()){
            myfile16.read((char*)&buff16_1,sizeof(buff16_1));
            myfile16.read((char*)&buff16_2,sizeof(buff16_2));
            container16[buff16_1][buff16_2]++;
        }
        myfile16.close();
    }else{
        std::cout << "file not found";
    }
    maxCount16=0;
    
    for (int i=0; i<(size16); i++) {
        for (int j=0; j<(size16); j++) {
            if (maxCount16<container16[i][j]) {
                maxCount16=container16[i][j];
            }
        }
    }
    /////////
    
    /* Initialize GLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE);
    glutInitWindowPosition(50, 25);
    glutInitWindowSize(1044,1044);
    glutCreateWindow("fvis32");
    glutKeyboardFunc( process_Keys );
    glutDisplayFunc(draw);
    glutMainLoop();
    
    exit(EXIT_FAILURE);
}