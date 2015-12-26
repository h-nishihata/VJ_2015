#pragma once

#include "ofMain.h"
#include "ofxPostGlitch.h"
#include "fft.h"
#include "ofxGui.h"

#define numVids 11
#define numFbos 12
#define numCams 3

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void drawFboTest_00();
    void drawFboTest_01();
    void drawFboTest_02();
    void drawFboTest_03();
    void drawFboTest_04();
    void drawFboTest_05();
    void drawFboTest_06();
    void drawFboTest_07();
    void drawFboTest_08();
    void drawFboTest_09();
    void drawFbo_Final();
    void drawFbo_GUI();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void audioIn(float* input, int bufferSize, int nChannels);
    

    
    ofFbo fbo[numFbos];
    
    ofCamera cam[numCams];
    ofNode testNode[numCams];
    ofNode testController[numCams];
    
    ofVideoPlayer vid[numVids];
    int vidState;
    
    
    
    int pos[10];
    int posHeight[10];
    int width[10];
    int width_[10];
    int vel[10];
    int rectLength;

    int r[10];
    int g[10];
    int b[10];
    int elapsedTime[10];
    int lastingTime[10];
    
    
    bool zFlag;
    int camPosX, camPosY, camPosZ;
    int yawDegree;
    int totalYaw;
    bool yawFlag;
    
    ofBoxPrimitive arrayBox;
    int arrayBoxSize;
    
    ofBoxPrimitive waveBox;
    int boxZPos[512];
    int transZPos[512];
    
    ofBoxPrimitive sphereBox;
    float sphereSize;
    
    
//    int watchDog;
    
    
    
    //  glitchs
    ofxPostGlitch myGlitch;
    ofxPostGlitch guiGlitch;
    
    
    // sounds
    //左右2chのFFTの入出力の値を確保
    float *input;
    float *magnitude, *phase, *power;
    //    float *magnitudeR, *phaseR, *powerR;
    float avg_power;
    //バッファーサイズとFFTサイズ
    int buffer_size;
    int fft_size;
    //FFTクラスのインスタンス
    fft myfft;
    
    
    // GUI
    ofxPanel gui;
    ofxFloatSlider radius;
    ofxToggle setFboActive[numFbos];
    
};
