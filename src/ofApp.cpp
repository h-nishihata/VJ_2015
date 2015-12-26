#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0,0,0);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    
    // videos
    char str[6];
    for (int i=0; i<numVids; i++) {
        sprintf(str, "%1d.mov", i);
        vid[i].loadMovie(str);
        vid[i].setLoopState(OF_LOOP_NORMAL);
    }
    
    
    // buffers
    for (int i=0; i<numFbos; i++) {
        
        fbo[i].allocate(1024, 768, GL_RGBA);
        fbo[i].begin();
        ofClear(255,255,255, 0);
        fbo[i].end();
        
    }
    
    
    // cameras & nodes
    camPosX = camPosY = 300;
    camPosZ = 100;
    
    
    //  3D boxes
    testNode[0].setPosition(500, 500, 100);
    testController[0].setPosition(0, 0, 0);
    cam[0].setPosition(camPosX, camPosY, camPosZ);
    cam[0].setParent(testController[0]);
    
    
    //  spectrum boxes
    testNode[1].setPosition(700, 0, 1000);
    cam[1].setPosition(700, 500, 2000);
    
    
    //  sphere boxes
    testNode[2].setPosition(500, 500, 100);
    testController[2].setPosition(0, 0, 0);
    cam[2].setPosition(camPosX, camPosY, camPosZ);
    cam[2].setParent(testController[2]);
    
    
    arrayBoxSize = ofRandom(20)+1;
    
    
    // glitchs
    // add effects to the final output
    myGlitch.setup(&fbo[numFbos-2]);
    // ...also to the GUI screen so that you can check it
    guiGlitch.setup(&fbo[numFbos-1]);
    
    
    // sounds
    fft_size = 512;
    buffer_size = fft_size * 2;
    
    input = new float[buffer_size];
    magnitude = new float[fft_size];
    phase = new float[fft_size];
    power = new float[fft_size];
    
    ofSoundStreamSetup(0, 2, this, 44100, buffer_size, 4);
    
    
    // GUI
    gui.setup();
    gui.add(radius.setup("radius", 200, 0, 400));
    for (int i=0; i<numFbos-2; i++) {
        gui.add(setFboActive[i].setup("fbo", false));
    }
    //    gui.add(color.setup("color", initColor, minColor, maxColor));
    //    gui.add(position.setup("position", initPos, minPos, maxPos));
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofEnableAlphaBlending();
    
    // videos
    if(ofGetKeyPressed('0')){
        vidState = 0;
    }else if(ofGetKeyPressed('9')){
        vidState = 9;
    }
    vid[vidState].play();
    vid[vidState].update();
    
    
    // final output
    setFboActive[numFbos-2] = true;
    // GUI buffer
    setFboActive[numFbos-1] = true;
    
    
    // buffers
    for (int i=0; i<numFbos; i++) {
        if (setFboActive[i] == true) {
            
            if(i == 7 || i == 9){
                int t;
                if (i == 7) {
                    t = 0;
                }else if(i == 9){
                    t = 2;
                }
                cam[t].lookAt(testNode[t]);
                testController[t].setPosition(ofVec3f(
                                                      sin(ofGetElapsedTimef()),
                                                      cos(ofGetElapsedTimef()),
                                                      sin(ofGetElapsedTimef())));
                testController[t].setOrientation(ofVec3f(
                                                         sin(ofGetElapsedTimef()),
                                                         cos(ofGetElapsedTimef()),
                                                         sin(ofGetElapsedTimef())));
                
                if(zFlag == false){
                    camPosZ += 5;
                    if(cam[t].getGlobalPosition().z > 1000){
                        zFlag = true;
                    }
                }else if(zFlag == true){
                    camPosZ -= 5;
                    if(cam[t].getGlobalPosition().z < -1000){
                        zFlag = false;
                    }
                }
                cam[t].setPosition(camPosX, camPosY, camPosZ);
            }
            
//            watchDog ++;
//            if (watchDog < 5) { //  safe :)
            
                fbo[i].begin();
                switch (i) {
                    case 0:
                        drawFboTest_00();
                        break;
                    case 1:
                        drawFboTest_01();
                        break;
                    case 2:
                        drawFboTest_02();
                        break;
                    case 3:
                        drawFboTest_03();
                        break;
                    case 4:
                        drawFboTest_04();
                        break;
                    case 5:
                        drawFboTest_05();
                        break;
                    case 6:
                        drawFboTest_06();
                        break;
                    case 7:
                        drawFboTest_07();
                        break;
                    case 8:
                        drawFboTest_08();
                        break;
                    case 9:
                        drawFboTest_09();
                        break;
                    case numFbos-2:
                        drawFbo_Final();
                        break;
                    case numFbos-1:
                        drawFbo_GUI();
                        break;
                }
                fbo[i].end();
                
//            }
        }
    }

//    watchDog = 0;

    
    for (int i=0; i<10; i++) {
        r[i] = ofRandom(255);
        g[i] = ofRandom(255);
        b[i] = ofRandom(255);
        
        pos [i] = ofRandom(ofGetWidth());
        posHeight [i] = ofRandom(ofGetHeight())-200;
        width[i] = ofRandom(300)+10;
        width_[i] = ofRandom(400)+50;
        vel[i] = ofRandom(100)-50;
        lastingTime[i] = ofRandom(50);
    }
    
    
    // sounds
    avg_power = 0.0;
    myfft.powerSpectrum(0, fft_size, input, buffer_size, magnitude, phase, power, &avg_power);
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_00(){
    
    /*****      videos     *****/
    
    ofClear(255,255,255, 0);
    vid[vidState].draw(0, 0, 1024, 768);
    
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_01(){
    
    ofClear(255,255,255, 0);
    
    /* 2D spectrum bars
     float w = (float)ofGetWidth()/ (float)fft_size / 2.0f;
     for (int i = 0; i < fft_size; i++) {
     
     float h = magnitude[i] * ofGetHeight();
     ofRect(400/2 - w * i, 400/2, w, h);
     ofRect(400/2 + w * i, 400/2, w, -h);
     }
     */
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_02(){
    
    /*****      lottery machine     *****/
    
    ofClear(255,255,255, 0);
    
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
        ofTriangle(0, 0, 0, size, size, size);
        ofPopMatrix();
    }
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_03(){
    
    /*****      parallelograms     *****/
    
    ofClear(255,255,255, 0);
    
    for(int i = 1; i < 6; i++) {
        ofPushMatrix();
        
        elapsedTime[i]++;
        ofSetColor(r[i], g[i], b[i]);
        
        for(int j = 0; j < 10; j++) {
            ofBeginShape();
            ofVertex(pos[i]-500 + j*40*i, posHeight[i] + 0);
            ofVertex(pos[i]-500 + j*40*i+20*i, posHeight[i] + 0);
            ofVertex(pos[i]-500 + j*40*i+40*i, posHeight[i] + 50*i);
            ofVertex(pos[i]-500 + j*40*i+20*i, posHeight[i] + 50*i);
            ofVertex(pos[i]-500 + j*40*i, posHeight[i] + 0*i);
            ofEndShape();
        }
        
        if(i == 1){
            pos[i] += vel[i]/5;
        }else if(i == 2){
            pos[i] -= vel[i]/5;
        }
        
        if(elapsedTime[i] > lastingTime[i]){
            elapsedTime[i] = 0;
            pos[i]  = ofRandom(ofGetWidth());
            posHeight[i]  = ofRandom(ofGetHeight());
            r[i] = ofRandom(255);
            g[i] = ofRandom(255);
            b[i] = ofRandom(255);
        }
        
        ofPopMatrix();
    }
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_04(){
    
    /*****      simple rectangles      *****/
    
    ofClear(255,255,255, 0);
    
    for(int i = 0; i < 10; i++) {
        ofPushMatrix();
        
        elapsedTime[i]++;
        
        ofSetColor(r[i], g[i], b[i]);
        ofFill();
        
        ofRect(pos[i], posHeight[i], width_[i], width[i]);
        
        if(i<=2){
            pos[i] += vel[i]/5;
        }else if(i>2 && i <= 4){
            posHeight[i] += vel[i]/5;
        }else if(i>4 && i <= 6){
            width_[i]++;
        }else if(i>6 && i <= 8){
            width[i]++;
        }else if(i>8 && i <= 10){
            if(ofRandom(10)>5){
                pos[i]--;
                width_[i]++;
            }else{
                posHeight[i]--;
                width[i]++;
            }
        }
        
        if(elapsedTime[i] > lastingTime[i]){
            elapsedTime[i] = 0;
            pos[i]  = ofRandom(ofGetWidth());
            posHeight[i]  = ofRandom(ofGetHeight());
            r[i] = ofRandom(255);
            g[i] = ofRandom(255);
            b[i] = ofRandom(255);
        }
        
        ofPopMatrix();
    }
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_05(){
    
    /*****      growing stripes     *****/
    
    ofClear(255,255,255, 0);
    
    elapsedTime[0]++;
    ofRotate(-45);
    ofTranslate(ofGetWidth()/4, ofGetHeight()/2);
    
    if(elapsedTime[0] > lastingTime[0]){
        elapsedTime[0] = 0;
        ofTranslate(ofRandom(-700), ofRandom(-450));
        ofScale(5, 5);
    }
    
    for (int i = 0; i < 10; i++) {
        ofSetColor(20, 255, 200);
        ofRect(i*30, 0, 15, 30 + rectLength);
    }
    rectLength+=10;
    
    if(rectLength > 500){
        rectLength = 0;
    }
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_06(){
    
    /*****      rect bars     *****/
    
    ofClear(255,255,255, 0);
    
    for (int i=0; i<10; i++) {
        ofPushMatrix();
        
        ofSetColor(r[i], g[i], b[i]);
        ofFill();
        
        ofRect(pos[i], 0, width[i], ofGetHeight());
        pos[i] += vel[i];
        if(pos[i] > ofGetWidth() || pos[i] < 0){
            pos[i]  = ofRandom(ofGetWidth());
            r[i] = ofRandom(255);
            g[i] = ofRandom(255);
            b[i] = ofRandom(255);
            
        }
        
        if(width[i] > 50){
            for (int k = 0; k < ofGetHeight(); k++) {
                for (int j = pos[i]; j<width[i]/20; j++) {
                    ofSetColor(255);
                    ofRect(j*20, k*20, 10, 10);
                }
            }
        }
        
        ofPopMatrix();
    }
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_07(){
    
    /*****      3D boxes     *****/
    
    ofClear(255,255,255, 0);
    
    cam[0].begin();
    
    ofTranslate(ofGetWidth()/4, ofGetHeight()/4);
    ofSetColor(255);
    
    for (int i=0; i<5; i++) {
        for (int j=0; j<5; j++) {
            for (int k=0; k<10; k++) {
                
                arrayBox.set(arrayBoxSize+magnitude[10]*80);
                arrayBox.setPosition(i*80, j*80, k*80);
                arrayBox.draw();
            }
        }
    }
    
    //    ofSetColor(255, 0, 0);
    //    testNode.draw();
    
    cam[0].end();
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_08(){
    
    /*****      spectrum boxes     *****/
    
    ofClear(255,255,255, 0);
    
    
    cam[1].lookAt(testNode[1]);
    cam[1].begin();
    
    if(yawFlag == true){
        if(totalYaw < 50){
            yawDegree = 1;
            totalYaw += 1;
        }else{
            yawFlag = false;
        }
    }else if(yawFlag == false){
        if(totalYaw > -50){
            yawDegree = -1;
            totalYaw -= 1;
        }else{
            yawFlag = true;
        }
    }
    
    cam[1].rotate(yawDegree, 0,0,1);
    
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            
            ofSetColor(i*10, j*20, 255);
            int height = magnitude[j*22+i]*500;
            
            transZPos[j*22+i] += 20;
            if (boxZPos[j*22+i] > (22*50)) {
                transZPos[j*22+i] = 0;
            }
            boxZPos[j*22+i] = j*100 + transZPos[j*22+i];
            
            waveBox.set(20, 20+height, 20);
            waveBox.setPosition(i*50, 10, boxZPos[j*22+1]);
            waveBox.draw();
            
        }
    }
    
    cam[1].end();
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_09(){
    
    /*****      sphere boxes     *****/
    
    ofClear(255,255,255, 0);
    
    cam[2].begin();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            
            ofSetColor(i*10, j*20, 255);
            sphereSize = 300 + 100 * magnitude[20];
            
            ofQuaternion latRot, longRot, spinQuat;
            latRot.makeRotate(i*1000/60., 1, 0, 0);
            longRot.makeRotate(j*800/60., 0, 1, 0);
            spinQuat.makeRotate(ofGetFrameNum(), 0, 1, 0);
            ofVec3f center = ofVec3f(0,0,sphereSize);
            ofVec3f worldPoint = latRot * longRot * spinQuat * center;
            
            sphereBox.set(10, 10, 10);
            sphereBox.setPosition(worldPoint);
            sphereBox.draw();
            
            ofSetColor(255, 0, 0);
            testNode[2].draw();
        }
    }
    cam[2].end();
    
}

//--------------------------------------------------------------
void ofApp::drawFbo_Final(){
    
    ofClear(255,255,255, 0);
    
    // draw active buffers
    for (int i=0; i<numFbos-2; i++) {
        
        if (setFboActive[i] == true) {
            ofSetColor(255);
            fbo[i].draw(0,0);
        }
        
    }
    
}

//--------------------------------------------------------------
void ofApp::drawFbo_GUI(){
    
    ofClear(255,255,255);
    
    ofSetColor(255);
    // check the final output on the GUI screen
    fbo[numFbos-2].draw(500,500,612,384);
    
    
    gui.draw();
    ofShowCursor();
    
    // cursor
    ofSetColor(255);
    ofRect(ofGetMouseX(), ofGetMouseY(), 20, 20);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255,255,255);
    myGlitch.generateFx();
    guiGlitch.generateFx();
    
    // GUI buffer
    fbo[numFbos-1].draw(0, 0);
    
    // main buffer
    fbo[numFbos-2].draw(1441,0);
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float* _input, int bufferSize, int nChannels) {
    
    input = _input;
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == '1'){
        myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
    }
    if (key == '2'){
        myGlitch.setFx(OFXPOSTGLITCH_GLOW, true);
        guiGlitch.setFx(OFXPOSTGLITCH_GLOW, true);
    }
    if (key == '3'){
        myGlitch.setFx(OFXPOSTGLITCH_SHAKER, true);
        guiGlitch.setFx(OFXPOSTGLITCH_SHAKER, true);
    }
    if (key == '4'){
        myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
    }
    if (key == '5'){
        myGlitch.setFx(OFXPOSTGLITCH_TWIST, true);
        guiGlitch.setFx(OFXPOSTGLITCH_TWIST, true);
    }
    if (key == '6'){
        myGlitch.setFx(OFXPOSTGLITCH_INVERT, true);
        guiGlitch.setFx(OFXPOSTGLITCH_INVERT, true);
    }
    if (key == '7'){
        myGlitch.setFx(OFXPOSTGLITCH_NOISE, true);
        guiGlitch.setFx(OFXPOSTGLITCH_NOISE, true);
    }
    if (key == '8'){
        myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, true);
        guiGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, true);
    }
    if (key == '9'){
        myGlitch.setFx(OFXPOSTGLITCH_SWELL, true);
        guiGlitch.setFx(OFXPOSTGLITCH_SWELL, true);
    }
    if (key == 'q'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    }
    if (key == 'w'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
    }
    if (key == 'e'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
    }
    if (key == 'r'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
    }
    if (key == 't'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
    }
    if (key == 'y'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
    }
    if (key == 'u'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == '1'){
        myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
    }
    if (key == '2'){
        myGlitch.setFx(OFXPOSTGLITCH_GLOW, false);
        guiGlitch.setFx(OFXPOSTGLITCH_GLOW, false);
    }
    if (key == '3'){
        myGlitch.setFx(OFXPOSTGLITCH_SHAKER, false);
        guiGlitch.setFx(OFXPOSTGLITCH_SHAKER, false);
    }
    if (key == '4'){
        myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
    }
    if (key == '5'){
        myGlitch.setFx(OFXPOSTGLITCH_TWIST, false);
        guiGlitch.setFx(OFXPOSTGLITCH_TWIST, false);
    }
    if (key == '6'){
        myGlitch.setFx(OFXPOSTGLITCH_INVERT, false);
        guiGlitch.setFx(OFXPOSTGLITCH_INVERT, false);
    }
    if (key == '7'){
        myGlitch.setFx(OFXPOSTGLITCH_NOISE, false);
        guiGlitch.setFx(OFXPOSTGLITCH_NOISE, false);
    }
    if (key == '8'){
        myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
        guiGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
    }
    if (key == '9'){
        myGlitch.setFx(OFXPOSTGLITCH_SWELL, false);
        guiGlitch.setFx(OFXPOSTGLITCH_SWELL, false);
    }
    if (key == 'q'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
    }
    if (key == 'w'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
    }
    if (key == 'e'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
    }
    if (key == 'r'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
    }
    if (key == 't'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
    }
    if (key == 'y'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
    }
    if (key == 'u'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
