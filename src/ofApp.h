#pragma once

#include "ofMain.h"
#include "ofxPostGlitch.h"
#include "fft.h"
#include "ofxGui.h"

#define numVids 11

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void drawFboTest_00();
    void drawFboTest_01();
    void drawFboTest_02();
    
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
    
    
    
    // buffers
    ofFbo fbo_00;
    ofFbo fbo_01;
    ofFbo fbo_02;
    
    
    // videos
    ofVideoPlayer vid[numVids];
    
    
    ofEasyCam cam;
    ofBoxPrimitive box;
    ofVbo spectrumBar;
    ofMesh Icosa;
    
    
    //  glitchs
    ofxPostGlitch	myGlitch_00;
    ofxPostGlitch	myGlitch_01;
    ofxPostGlitch	myGlitch_02;
    
    
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
    ofxColorSlider color;
    ofxVec2Slider position;
    
    //    ofQuaternion latRot[512];
    //    ofQuaternion longRot[512];
    //    ofQuaternion spinQuat[512];
    
};
