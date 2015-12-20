#pragma once

#include "ofMain.h"
#include "ofxPostGlitch.h"
#include "fft.hpp"

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
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void audioIn(float* input, int bufferSize, int nChannels);
    
    
    
    ofFbo rgbaFbo;
    ofFbo rgbaFboFloat;
    ofFbo fbo_02;
    
    ofVideoPlayer intro_0;
    ofBoxPrimitive box;
    
    //  glitch
    ofxPostGlitch	myGlitch_00;
    ofxPostGlitch	myGlitch_01;
    ofxPostGlitch	myGlitch_02;
    
    
    
    // *****    sounds  *****    
    //左右2chのFFTの入出力の値を確保
    float *input;
    float *magnitude, *phase, *power;
    //    float *magnitudeR, *phaseR, *powerR;
    float avg_power;
    //バッファーサイズとFFTサイズ
    int buffer_size;
    int fft_size;
    
    fft myfft; //FFTクラスのインスタンス
    
};
