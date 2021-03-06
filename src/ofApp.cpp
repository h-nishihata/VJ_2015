#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0,0,0);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    
    //  videos
    vid.loadMovie("merged.mov");
    vid.play();
    
    
    //  GUI buffer
    fbo[numFbos-1].allocate(1440, 900, GL_RGBA);
    fbo[numFbos-1].begin();
    ofClear(255,255,255, 0);
    fbo[numFbos-1].end();
    
    //  buffers
    for (int i=0; i<numFbos-1; i++) {
        
        fbo[i].allocate(1024, 768, GL_RGBA);
        fbo[i].begin();
        ofClear(255,255,255, 0);
        fbo[i].end();
        
    }
    
    
    //  cameras & nodes
    camPosX = camPosY = 300;
    camPosZ = -200;
    
    
    //  01 _ particle letters
    resetImg(0);
    testNode[3].setPosition(0, -200, 0);
    testController[3].setPosition(0, 0, 0);
    cam[3].setPosition(camPosX, camPosY, camPosZ);
    cam[3].setParent(testNode[3]);
    
    
    for (int i=0; i<10; i++) {
        r[i] = ofRandom(255);
        g[i] = ofRandom(255);
        b[i] = ofRandom(255);
        
        lastingTime[i] = ofRandom(50);
    }
    
    
    //  03 _ parallelograms
    for (int i=0; i<10; i++) {
        
        xPos_03[i] = ofRandom(fbo[3].getWidth());
        yPos_03[i] = ofRandom(fbo[3].getHeight())-200;
        vel_03[i] = ofRandom(100)-100;
        
    }
    
    
    //  04 _ simple rectangles
    for (int i=0; i<10; i++) {
        
        xPos_04[i] = ofRandom(fbo[4].getWidth());
        yPos_04[i] = ofRandom(fbo[4].getHeight())-200;
        width_04[i] = ofRandom(300)+10;
        height_04[i] = ofRandom(400)+50;
        vel_04[i] = ofRandom(100)-100;
        
    }
    
    
    //  05 _ growing stripes
    translateX = 0;
    translateY = 200;
    rotAng = -45;
    barScale = 1;
    barR = 50;
    barG = 255;
    barB = 180;
    
    
    //  06 _ rect bars
    for (int i=0; i<10; i++) {
        
        xPos_06[i] = ofRandom(fbo[6].getWidth());
        width_06[i] = ofRandom(300)+10;
        vel_06[i] = ofRandom(80)-50;
        
    }
    
    elapsedTime_06 = 70;
    
    
    //  07 _ 3D boxes
    testNode[0].setPosition(200, 200, 400);
    testController[0].setPosition(0, 0, 0);
    cam[0].setPosition(camPosX, camPosY, camPosZ);
    cam[0].setParent(testController[0]);
    
    arrayBoxSize = ofRandom(20)+1;
    
    
    //  08 _ spectrum boxes
    testNode[1].setPosition(700, 0, 1000);
    cam[1].setPosition(700, 350, 2000);
    
    
    //  09 _ sphere boxes
    testNode[2].setPosition(500, 500, 100);
    testController[2].setPosition(0, 0, 0);
    cam[2].setPosition(camPosX, camPosY, camPosZ);
    cam[2].setParent(testController[2]);
    
    
    
    
    
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
    gui.add(testPattern.setup("test pattern : ", true));
    gui.add(readyToShow.setup("ready to show : ", false));
    gui.add(vidState.setup("video state : ", 0, 0, 3));
    gui.add(vidSpeed.setup("video speed : ", 2.0, 0.1, 5.0));
    gui.add(sensitivity.setup("sensitivity : ", 10, 0, 100));
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofEnableAlphaBlending();
    
    // videos
    vid.update();
    vid.setSpeed(vidSpeed);
    
    if (vidState == 0) {
        // intro
        if (vid.getPosition()*vid.getDuration() >= 39.0) {
            vid.setPosition((int)0/202.2);
        }
        
    }else if (vidState == 1) {
        // mid
        if (vid.getPosition()*vid.getDuration() >= 86.0) {
            vid.setPosition((int)40.0/202.2);
        }
        
    }else if (vidState == 2) {
        // main 1
        if (vid.getPosition()*vid.getDuration() >= 121.0) {
            vid.setPosition((int)86.0/202.2);
        }
    }else if (vidState == 3) {
        // main 2
        if (vid.getPosition()*vid.getDuration() >= 201.0) {
            vid.setPosition((int)121.0/202.2);
        }
    }
    
    
    // final output
    setFboActive[numFbos-2] = true;
    // GUI buffer
    setFboActive[numFbos-1] = true;
    
    
    // buffers
    for (int i=0; i<numFbos; i++) {
        if (setFboActive[i] == true) {
            
            if(i == 7 || i == 9 || i == 1){
                int t;
                if (i == 7) {
                    t = 0;
                }else if(i == 9){
                    t = 2;
                }else if(i == 1){
                    t = 3;
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
    
    // sounds
    avg_power = 0.0;
    myfft.powerSpectrum(0, fft_size, input, buffer_size, magnitude, phase, power, &avg_power);
    
}

//--------------------------------------------------------------
void ofApp::resetImg(int imgID){
    
    char str[5];
    sprintf(str, "%1d.png", imgID);
    img.loadImage(str);
    
    pixels = img.getPixels();
    
    for (int i=0; i<imgWidth; i+=4) {
        for (int j=0; j<imgHeight; j+=4) {
            
            ofColor cur = img.getColor(i, j);
            if(cur.a > 0){
                
                // memorize positions in the image
                _initPos[j*imgWidth+i] = ofVec3f(i-300, j, 0);
                
                
            }
            // deploy all particles randomly
            _pos[j*imgWidth+i] = ofVec3f(ofRandom(-500,500),
                                         ofRandom(500),
                                         ofRandom(-500,500));
            points[j*imgWidth+i].set(_pos[j*imgWidth+i]);
            pct[j*imgWidth+i] = 0;
            
            _vel[j*imgWidth+i] = ofVec3f(ofRandom(20)-10, ofRandom(20)-10, ofRandom(20)-10);
            
        }
    }
    
}

//--------------------------------------------------------------
ofVec3f ofApp::interpolateByPct(float _pct, int _id){
    
    ofVec3f pos;
    float shapedPct = powf(_pct, 0.5);
    pos.x = (1.0 - shapedPct) * _startPos[_id].x + (shapedPct) * _initPos[_id].x;
    pos.y = (1.0 - shapedPct) * _startPos[_id].y + (shapedPct) * _initPos[_id].y;
    pos.z = (1.0 - shapedPct) * _startPos[_id].z + (shapedPct) * _initPos[_id].z;
    return pos;
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_00(){
    
    /*****      videos     *****/
    
    ofClear(255,255,255, 0);
    vid.draw(0, 0, 1024, 768);
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_01(){
    
    /*****      particle letters     *****/
    
    time_letters++;
    
    ofClear(255, 255, 255, 0);
    cam[3].lookAt(testNode[3]);
    
    for (int i=0; i<imgParticles; i+=4) {
        
        if(_pos[i].x > abs(500)){_vel[i].x*-1;}
        if(_pos[i].y > abs(500)){_vel[i].y*-1;}
        if(_pos[i].z > abs(500)){_vel[i].z*-1;}
        
        _frc[i].set(ofVec3f(0,0,0));
        _pos[i] -= _vel[i]*0.9;
        _pos[i] += _vel[i];
        points[i].set(_pos[i]);
        
    }
    
    
    if(time_letters == 50){
        for (int i=0; i<imgWidth; i++) {
            for (int j=0; j<imgHeight; j++) {
                _startPos[j*imgWidth+i] = _pos[j*imgWidth+i];
            }
        }
        emergeMode = true;
    }
    
    
    if(emergeMode){
        
        for (int i=0; i<imgWidth; i++) {
            for (int j=0; j<imgHeight; j++) {
                
                ofColor cur = img.getColor(i, j);
                if(cur.a > 0){
                    pct[j*imgWidth+i] += 0.01;
                    
                    _currentPos[j*imgWidth+i] = interpolateByPct(pct[j*imgWidth+i], j*imgWidth+i);
                    if(pct[j*imgWidth+i] < 1.0){
                        points[j*imgWidth+i].set(_currentPos[j*imgWidth+i]);
                    }else{
                        points[j*imgWidth+i].set(_initPos[j*imgWidth+i]);
                    }
                    
                }
                if(time_letters == 600){
                    emergeMode = false;
                    pct[j*imgWidth+i] = 0;
                }
            }
        }
        
    }
    
    
    if(time_letters == 650){
        if(_imgID < numImgs){
            _imgID ++;
        }else{
            _imgID = 0;
        }
        resetImg(_imgID);
        time_letters = 0;
    }
    
    
    cam[3].begin();
    glPointSize(5.0);
    ofSetColor(255,255,0);
    ofRotateZ(180);
    
    p.setVertexData(&points[0], imgParticles, GL_DYNAMIC_DRAW);
    p.draw(GL_POINTS, 0, imgParticles);
    cam[3].end();
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_02(){
    
    /*****      lottery machine     *****/
    
    ofClear(255,255,255, 0);
    ofTranslate(fbo[2].getWidth()/2, fbo[2].getHeight()/2);
    float maxSize = 80;
    
    for(int i = 0; i < 120; i++) {
        ofPushMatrix();
        float t = (ofGetElapsedTimef() + i * 2) * .3;
        ofVec3f pos(ofSignedNoise(t, 0, 0),
                    ofSignedNoise(0, t, 0),
                    ofSignedNoise(0, 0, t));
        
        float size = maxSize * ofNoise(pos.x, pos.y, pos.z);
        pos *= fbo[2].getWidth()/2;
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
            ofVertex(xPos_03[i]-500 + j*40*i, yPos_03[i] + 0);
            ofVertex(xPos_03[i]-500 + j*40*i+20*i, yPos_03[i] + 0);
            ofVertex(xPos_03[i]-500 + j*40*i+40*i, yPos_03[i] + 50*i);
            ofVertex(xPos_03[i]-500 + j*40*i+20*i, yPos_03[i] + 50*i);
            ofVertex(xPos_03[i]-500 + j*40*i, yPos_03[i] + 0*i);
            ofEndShape();
        }
        
        if(i == 1){
            xPos_03[i] += vel_03[i]/2;
        }else if(i == 2){
            xPos_03[i] -= vel_03[i]/2;
        }
        
        if(elapsedTime[i] > lastingTime[i]){
            elapsedTime[i] = 0;
            xPos_03[i]  = ofRandom(fbo[3].getWidth());
            yPos_03[i]  = ofRandom(fbo[3].getHeight());
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
        
        ofRect(xPos_04[i], yPos_04[i], width_04[i], height_04[i]);
        
        if(i<=2){
            xPos_04[i] += vel_04[i]/5;
        }else if(i>2 && i <= 4){
            yPos_04[i] += vel_04[i]/5;
        }else if(i>4 && i <= 6){
            width_04[i]++;
        }else if(i>6 && i <= 8){
            width_04[i]++;
        }else if(i>8 && i <= 10){
            if(ofRandom(10)>5){
                xPos_04[i]--;
                height_04[i]++;
            }else{
                yPos_04[i]--;
                width_04[i]++;
            }
        }
        
        if(elapsedTime[i] > lastingTime[i]){
            elapsedTime[i] = 0;
            xPos_04[i]  = ofRandom(fbo[4].getWidth());
            yPos_04[i]  = ofRandom(fbo[4].getHeight());
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
    
    elapsedTime_06++;
    ofTranslate(translateX, translateY);
    ofRotate(rotAng);
    ofScale(barScale, barScale);
    ofSetColor(barR, barG, barB);
    
    if(elapsedTime_06 < lastingTime[0]){
        
        for (int i = 0; i < 10; i++) {
            ofRect(i*30, 0, 15, 30 + rectLength);
        }
        
        rectLength+=10;
        if(rectLength > 500){
            rectLength = 0;
        }
        
    }else{
        
        elapsedTime_06 = 0;
        translateX = ofRandom(-200);
        translateY = ofRandom(-200);
        
        if(ofRandom(10) > 4.5){
            rotAng *= -1;
            barR = 50;
            barG = 255;
            barB = 180;
        }else{
            barR = 255;
            barG = 100;
            barB = 80;
        }
        
        barScale = ofRandom(1,5);
        
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
        
        ofRect(xPos_06[i], 0, width_06[i], fbo[6].getHeight());
        xPos_06[i] += vel_06[i];
        if(xPos_06[i] > fbo[6].getWidth() || xPos_06[i] < 0){
            xPos_06[i]  = ofRandom(fbo[6].getWidth());
            r[i] = ofRandom(255);
            g[i] = ofRandom(255);
            b[i] = ofRandom(255);
            
        }
        
        if(width_06[i] > 50){
            for (int k = 0; k < fbo[6].getHeight(); k++) {
                for (int j = xPos_06[i]; j<width_06[i]/20; j++) {
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
    ofSetColor(255);
    
    for (int i=0; i<5; i++) {
        for (int j=0; j<5; j++) {
            for (int k=0; k<10; k++) {
                
                arrayBox.set(arrayBoxSize+magnitude[20]*sensitivity);
                arrayBox.setPosition(i*80, j*80, k*80);
                arrayBox.draw();
            }
        }
    }
    
    cam[0].end();
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_08(){
    
    /*****      spectrum boxes     *****/
    
    ofClear(255,255,255, 0);
    
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
            
            ofSetColor(255, i*10+5, j*10+5);
            int height = magnitude[j*22+i]*sensitivity*3;
            
            transZPos[j*22+i] += 20;
            if (boxZPos[j*22+i] > (22*(30+80))) {
                boxZPos[j*22+i] = 0;
                transZPos[j*22+i] = 0;
            }
            boxZPos[j*22+i] = j*80 + transZPos[j*22+i];
            
            waveBox.set(30, 20+height, 30);
            waveBox.setPosition(i*80, 200, boxZPos[j*22+i]);
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
    ofTranslate(fbo[9].getWidth()/2, fbo[9].getHeight()/2);
    
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            
            ofSetColor(i*10, j*20, 255);
            sphereSize = 500 + sensitivity * magnitude[20];
            
            ofQuaternion latRot, longRot, spinQuat;
            latRot.makeRotate(i*1000/60., 1, 0, 0);
            longRot.makeRotate(j*800/60., 0, 1, 0);
            spinQuat.makeRotate(ofGetFrameNum(), 0, 1, 0);
            ofVec3f center = ofVec3f(0,0,sphereSize);
            ofVec3f worldPoint = latRot * longRot * spinQuat * center;
            
            sphereBox.set(10, 10, 10);
            sphereBox.setPosition(worldPoint);
            sphereBox.draw();
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
    fbo[numFbos-2].draw((1440-1024), (900-768), 1024, 768);
    
    ofDrawBitmapString("duration: " + ofToString(vid.getPosition()*vid.getDuration(),2) + "/"+ofToString(vid.getDuration(),2),20,400);
    
    
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
    if(readyToShow){
        fbo[numFbos-2].draw(1441,0);
    }
    
    //  test pattern
    if(testPattern){
        ofSetColor(0, 0, 255);
        ofRect(1441, 0, fbo[numFbos-1].getWidth(), fbo[numFbos-1].getHeight());
    }
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float* _input, int bufferSize, int nChannels) {
    
    input = _input;
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'q'){
        myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
    }
    if (key == 'w'){
        myGlitch.setFx(OFXPOSTGLITCH_SHAKER, true);
        guiGlitch.setFx(OFXPOSTGLITCH_SHAKER, true);
    }
    if (key == 'e'){
        myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
    }
    if (key == 'r'){
        myGlitch.setFx(OFXPOSTGLITCH_TWIST, true);
        guiGlitch.setFx(OFXPOSTGLITCH_TWIST, true);
    }
    if (key == 't'){
        myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, true);
        guiGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, true);
    }
    if (key == 'y'){
        myGlitch.setFx(OFXPOSTGLITCH_SWELL, true);
        guiGlitch.setFx(OFXPOSTGLITCH_SWELL, true);
    }
    
    // invert
    if (key == 'u'){
        myGlitch.setFx(OFXPOSTGLITCH_INVERT, true);
        guiGlitch.setFx(OFXPOSTGLITCH_INVERT, true);
    }
    if (key == 'i'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
    }
    if (key == 'o'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
    }
    if (key == 'p'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
    }
    
    
    // scenes
    if (key == 'z') {
        if(setFboActive[0] == true){
            setFboActive[0] = false;
        }else{
            setFboActive[0] = true;
        }
    }
    if (key == 'x') {
        if(setFboActive[1] == true){
            setFboActive[1] = false;
        }else{
            setFboActive[1] = true;
        }
    }
    if (key == 'c') {
        if(setFboActive[2] == true){
            setFboActive[2] = false;
        }else{
            setFboActive[2] = true;
        }
    }
    if (key == 'v') {
        if(setFboActive[3] == true){
            setFboActive[3] = false;
        }else{
            setFboActive[3] = true;
        }
    }
    if (key == 'b') {
        if(setFboActive[4] == true){
            setFboActive[4] = false;
        }else{
            setFboActive[4] = true;
        }
    }
    if (key == 'n') {
        if(setFboActive[5] == true){
            setFboActive[5] = false;
        }else{
            setFboActive[5] = true;
        }
    }
    if (key == 'm') {
        if(setFboActive[6] == true){
            setFboActive[6] = false;
        }else{
            setFboActive[6] = true;
        }
    }
    if (key == ',') {
        if(setFboActive[7] == true){
            setFboActive[7] = false;
        }else{
            setFboActive[7] = true;
        }
        
    }
    if (key == '.') {
        if(setFboActive[8] == true){
            setFboActive[8] = false;
        }else{
            setFboActive[8] = true;
        }
        
    }
    if (key == '/') {
        if(setFboActive[9] == true){
            setFboActive[9] = false;
        }else{
            setFboActive[9] = true;
        }
        
    }
    
    // reset
    if (key == ' ') {
        setFboActive[0] = true;
        for (int i=1; i<numFbos-2; i++) {
            setFboActive[i] = false;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == 'q'){
        myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
    }
    if (key == 'w'){
        myGlitch.setFx(OFXPOSTGLITCH_SHAKER, false);
        guiGlitch.setFx(OFXPOSTGLITCH_SHAKER, false);
    }
    if (key == 'e'){
        myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
    }
    if (key == 'r'){
        myGlitch.setFx(OFXPOSTGLITCH_TWIST, false);
        guiGlitch.setFx(OFXPOSTGLITCH_TWIST, false);
    }
    if (key == 't'){
        myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
        guiGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
    }
    if (key == 'y'){
        myGlitch.setFx(OFXPOSTGLITCH_SWELL, false);
        guiGlitch.setFx(OFXPOSTGLITCH_SWELL, false);
    }
    
    // invert
    if (key == 'u'){
        myGlitch.setFx(OFXPOSTGLITCH_INVERT, false);
        guiGlitch.setFx(OFXPOSTGLITCH_INVERT, false);
    }
    if (key == 'i'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
    }
    if (key == 'o'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
        guiGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
    }
    if (key == 'p'){
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
