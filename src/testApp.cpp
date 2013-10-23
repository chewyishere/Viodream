#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){
    
    nextVid = 0;
    thisVid1 = 0;
    thisVid2 = 0;
    prevVid = 0;

    ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	//ofBackground(255);
    ofToggleFullscreen();

    number=0;
    
    //sound
    soundStream.listDevices();
    
    //bfo
    rgbaFbo.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGBA);
    rgbaFboFloat.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGBA32F_ARB);
	
    rgbaFbo.begin();
	ofClear(255,255,255, 0);
    rgbaFbo.end();
    
    fadeAmnt=50;
    
	
	int bufferSize = 256;
	
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
	
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    ofSetBackgroundColor(0);

}

//--------------------------------------------------------------
void testApp::update(){
    
    //prevVid - [ thisVid1 - thisVid2 ] - nextVid
    
    vidPlayer[thisVid1].update();
    vidPlayer[thisVid2].update();
    
    if(vidPlayer[nextVid].isLoaded() && thisVid2 != nextVid){
        cout << "nextVid is Loaded: "<<nextVid<<endl;
        vidPlayer[nextVid].play();

        prevVid = thisVid1; // set currentVid to nextVid
        thisVid1 = thisVid2; // set prevVid to currentVid
        thisVid2 = nextVid;
        
        
        vidPlayer[prevVid].close();
    }
    
    
    if( numberBlend == 1)blendMode = OF_BLENDMODE_ADD;
    else if( numberBlend == 2)blendMode = OF_BLENDMODE_MULTIPLY;
    else if( numberBlend == 3)blendMode = OF_BLENDMODE_SUBTRACT;
    else if( numberBlend == 4)blendMode = OF_BLENDMODE_DISABLED;
    else if( numberBlend == 5)blendMode = OF_BLENDMODE_ALPHA;
    
}



//--------------------------------------------------------------

void testApp::drawFboTest(){
//	
//	if( ofGetKeyPressed('0') ){
//		ofClear(255,255,255, 0);
//	}
    
    
    ofFill();
    
    ofColor dark(10,fadeAmnt);
    ofColor black(0, fadeAmnt);
    
    ofBackgroundGradient(dark, black);
    
    
}

//--------------------------------------------------------------

void testApp::draw(){
    
    ofSetColor(255, 50);
    rgbaFbo.draw(0,0);
    rgbaFboFloat.draw(0,0);
    
   // ofDrawBitmapString(ofToString(fadeAmnt),0,100);
    
    ofEnableBlendMode(blendMode);
    //ofSetHexColor(0xFFFFFF);
    //ofSetBackgroundColor(0);
    
    ofSetColor(255, 255);
    vidPlayer[thisVid1].draw(0,0);//,1440,900);
    
    ofSetColor(255, 200);
    vidPlayer[thisVid2].draw(0,0);//,1440,900);
    
    cout << "currently playing vid num: "<<thisVid1<<" and "<<thisVid2<<endl;
}


//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
        
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    int pNumber = number;
    
    if( key == '`' ) ofToggleFullscreen();
    else if( key == '1' ) number = 1;
    else if( key == '2' ) number = 2;
    else if( key == '3' ) number = 3;
    else if( key == '4' ) number = 4;
    else if( key == '5' ) number = 5;
  	else if( key == '6' ) number = 6;
    else if( key == '7' ) number = 7;
    else if( key == '8' ) number = 8;
    //if( key == '9' ) number = 9;
    else if( key == '0' ) number = 0;
    
    
    if(pNumber != number){
        string vidFile = "02/s"+ ofToString(number) +".mov";
        
        nextVid = number;
        vidPlayer[nextVid].loadMovie(vidFile);
        vidPlayer[nextVid].firstFrame();
    }
    
    else {
        //-------------------- Interactive  ---------------
        
        if( key == 'q' ) number = 11;
        if( key == 'w' ) number = 12;
        if( key == 'e' ) number = 13;
        if( key == 'r' ) number = 14;
        if( key == 't' ) number = 15;
        if( key == 'y' ) number = 16;
        if( key == 'u' ) number = 17;
        
        if( key == 'z' ){
            numberBlend = 1;
        }
        if( key == 'x' ){
            numberBlend = 2;
        }
        if( key == 'c' ){
            numberBlend = 3;
        }
        if( key == 'v' ){
            numberBlend = 4;
        }
        if( key == 'b' ){
            numberBlend = 5;
        }
        if( key == 'n' ){
            numberBlend = 6;
        }
        if( key == 's' ){
            
            if(vidPlayer[thisVid2].isPlaying()){
                vidPlayer[thisVid2].stop();
            } else {
            
                vidPlayer[thisVid2].play();
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
