#pragma once

#include "ofMain.h"
#include "ofxPostGlitch.h"
#include "fft.h"
#include "ofxGui.h"

//#define numVids 11
#define numFbos 12
#define numCams 4
#define numImgs 2

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
    void resetImg(int imgID);
    ofVec3f interpolateByPct(float _pct, int _id);
    
    int _imgID;
    static const int imgWidth = 313;
    static const int imgHeight = 339;
    static const int imgParticles  = imgWidth * imgHeight;
    unsigned char * pixels;
    
    ofFbo fbo[numFbos];
    
    ofCamera cam[numCams];
    ofNode testNode[numCams];
    ofNode testController[numCams];
    
    ofVideoPlayer vid;
    
    ofVbo p;
    ofImage img;
    ofVec3f points[imgParticles];
    ofVec3f _initPos[imgParticles];
    ofVec3f _pos[imgParticles];
    ofVec3f _vel[imgParticles];
    ofVec3f _frc[imgParticles];
    ofVec3f _startPos[imgParticles];
    ofVec3f _currentPos[imgParticles];
    bool emergeMode;

    float pct[imgParticles];
    int time_letters;
    
    // rect
    int pos[10], posHeight[10], width[10], width_[10], vel[10];
    int rectLength;

    int r[10], g[10], b[10];
    int elapsedTime[10], lastingTime[10];
    
    // growing stripes
    int translateX, translateY;
    int rotAng;
    int barScale;
    int barR, barG, barB;
    
    // spectrumboxes
    bool zFlag;
    int camPosX, camPosY, camPosZ;
    int yawDegree, totalYaw;
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
    ofxIntSlider vidState;
    ofxFloatSlider vidSpeed;
    ofxIntSlider sensitivity;
    bool setFboActive[numFbos];
    
};
