#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0,0,0);
    ofSetFrameRate(60);
    
    intro_0.load("intro_0.mov");
    intro_0.setLoopState(OF_LOOP_NORMAL);
    intro_0.play();
    
    
    rgbaFbo.allocate(400, 400, GL_RGBA);
    fbo_02.allocate(400, 400, GL_RGBA);
#ifdef TARGET_OPENGLES
    rgbaFboFloat.allocate(400, 400, GL_RGBA );
    ofLogWarning("ofApp") << "GL_RGBA32F_ARB is not available for OPENGLES.  Using RGBA.";
#else
    rgbaFboFloat.allocate(400, 400, GL_RGBA32F_ARB);
#endif
    
    myGlitch_00.setup(&rgbaFbo);
    myGlitch_01.setup(&rgbaFboFloat);
    myGlitch_02.setup(&fbo_02);
    
    
    
    rgbaFbo.begin();
    ofClear(255,255,255, 0);
    rgbaFbo.end();
    
    rgbaFboFloat.begin();
    ofClear(255,255,255, 0);
    rgbaFboFloat.end();
    
    fbo_02.begin();
    ofClear(255,255,255, 0);
    fbo_02.end();
    
    
    
    // *****    sounds  *****    
    fft_size = 512;
    buffer_size = fft_size * 2;
    
    
    input = new float[buffer_size];
    magnitude = new float[fft_size];
    phase = new float[fft_size];
    power = new float[fft_size];
    
    ofSoundStreamSetup(0, 2, this, 44100, buffer_size, 4);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofEnableAlphaBlending();
    intro_0.update();
    
    
    rgbaFbo.begin();
    drawFboTest_00();
    rgbaFbo.end();
	  	
    rgbaFboFloat.begin();
    drawFboTest_01();
    rgbaFboFloat.end();
    
    fbo_02.begin();
    drawFboTest_02();
    fbo_02.end();
    
    
    
    // *****    sounds  *****
    avg_power = 0.0;
    myfft.powerSpectrum(0, fft_size, input, buffer_size, magnitude, phase, power, &avg_power);
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_00(){
    
    ofClear(255,255,255, 0);
    
    intro_0.draw(0, 0, 400, 400);
    
    float maxSize = 50;
    for(int i = 0; i < 100; i++) {
        ofPushMatrix();
        float t = (ofGetElapsedTimef() + i * 2) * .3;
        ofVec3f pos(ofSignedNoise(t, 0, 0),
                    ofSignedNoise(0, t, 0),
                    ofSignedNoise(0, 0, t));
        
        float size = maxSize * ofNoise(pos.x, pos.y, pos.z);
        pos *= ofGetWidth()/2;
        ofTranslate(pos);
        ofRotateX(pos.x*.8);
        ofRotateY(pos.y*.8);
        ofRotateZ(pos.z*.8);
        
        ofFill();
        ofSetColor(ofColor::fromHsb(sinf(t) * 128 + 128, 255, 255));
        ofDrawTriangle(0, 0, 0, size, size, size);
        ofPopMatrix();
    }
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_01(){
    
    ofClear(255,255,255, 0);
    
    // waves
    float w = (float)ofGetWidth()/ (float)fft_size / 2.0f;
    for (int i = 0; i < fft_size; i++) {
        
        float h = magnitude[i] * ofGetHeight();
        ofDrawRectangle(400/2 - w * i, 400/2, w, h);
        ofDrawRectangle(400/2 + w * i, 400/2, w, -h);
    }
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_02(){
    
    ofClear(255,255,255, 0);
    
    ofSetLineWidth(1);
    ofSetColor(255);
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            int height = magnitude[j*22+i]*100;
            box.set(20, height, 20);
            box.setPosition(i*50, 10+height, j*50);
            
            box.drawWireframe();
            
        }
    }
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    //    ofFill();
    myGlitch_00.generateFx();
    myGlitch_01.generateFx();
    myGlitch_02.generateFx();
    
    ofSetColor(255,255,255);
    rgbaFbo.draw(0,0);
    rgbaFboFloat.draw(100,0);
    fbo_02.draw(200,0);

}

//--------------------------------------------------------------
void ofApp::audioIn(float* _input, int bufferSize, int nChannels) {
    
    input = _input;
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == '1'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
    }
    if (key == '2'){
        myGlitch_00.setFx(OFXPOSTGLITCH_GLOW, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_GLOW, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_GLOW, true);
    }
    if (key == '3'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SHAKER, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_SHAKER, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_SHAKER, true);
    }
    if (key == '4'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
    }
    if (key == '5'){
        myGlitch_00.setFx(OFXPOSTGLITCH_TWIST, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_TWIST, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_TWIST, true);
    }
    if (key == '6'){
        myGlitch_00.setFx(OFXPOSTGLITCH_INVERT, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_INVERT, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_INVERT, true);
    }
    if (key == '7'){
        myGlitch_00.setFx(OFXPOSTGLITCH_NOISE, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_NOISE, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_NOISE, true);
    }
    if (key == '8'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SLITSCAN, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_SLITSCAN, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_SLITSCAN, true);
    }
    if (key == '9'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SWELL, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_SWELL, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_SWELL, true);
    }
    if (key == 'q'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    }
    if (key == 'w'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
    }
    if (key == 'e'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
    }
    if (key == 'r'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
    }
    if (key == 't'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
    }
    if (key == 'y'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
    }
    if (key == 'u'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == '1'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
    }
    if (key == '2'){
        myGlitch_00.setFx(OFXPOSTGLITCH_GLOW, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_GLOW, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_GLOW, false);
    }
    if (key == '3'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SHAKER, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_SHAKER, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_SHAKER, false);
    }
    if (key == '4'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
    }
    if (key == '5'){
        myGlitch_00.setFx(OFXPOSTGLITCH_TWIST, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_TWIST, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_TWIST, false);
    }
    if (key == '6'){
        myGlitch_00.setFx(OFXPOSTGLITCH_INVERT, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_INVERT, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_INVERT, false);
    }
    if (key == '7'){
        myGlitch_00.setFx(OFXPOSTGLITCH_NOISE, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_NOISE, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_NOISE, false);
    }
    if (key == '8'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SLITSCAN, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_SLITSCAN, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_SLITSCAN, false);
    }
    if (key == '9'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SWELL, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_SWELL, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_SWELL, false);
    }
    if (key == 'q'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
    }
    if (key == 'w'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
    }
    if (key == 'e'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
    }
    if (key == 'r'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
    }
    if (key == 't'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
    }
    if (key == 'y'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
    }
    if (key == 'u'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
