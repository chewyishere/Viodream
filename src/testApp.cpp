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
    //ofToggleFullscreen();

    number=0;
    Snumber=1;
    
    //sound
    soundStream.listDevices();
    

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
   
   	//sound
    
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 0.1, true);
    //lets record the volume into an array
    volHistory.push_back( scaledVol );
    
    //if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}
    
    
    currentSpeed = ofMap(scaledVol, 1.0f, 30.0f, 0.8f, 4.0f);

    
    //prevVid - [ thisVid1 - thisVid2 ] - nextVid
    
    //vidPlayer[thisVid2].setSpeed(currentSpeed);
    //vidPlayer[thisVid1].setSpeed(currentSpeed);
    
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
    
  
             
    if( numberBlend == 1)blendMode = OF_BLENDMODE_ALPHA;
    else if( numberBlend == 2)blendMode = OF_BLENDMODE_MULTIPLY;
    else if( numberBlend == 3)blendMode = OF_BLENDMODE_SUBTRACT;
    else if( numberBlend == 4)blendMode = OF_BLENDMODE_DISABLED;
    
 
}


//--------------------------------------------------------------

void testApp::draw(){


    ofEnableBlendMode(blendMode);
    
    vidPlayer[thisVid1].draw(0,0);
    
    ofSetColor(255, 200);
    vidPlayer[thisVid2].draw(0,0);
    
    cout << "section "<<Snumber<<endl;
    cout << " vid num: "<<thisVid1<<" and "<<thisVid2<<endl;
    cout << "blending mode "<<numberBlend<<endl;
    
    

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
    
    //-------------------- Section Number  ---------------
    
    if( key == 'q' ) Snumber = 1;
    else if( key == 'w' ) Snumber = 2;
    else if( key == 'e' ) Snumber = 3;
    else if( key == 'r' ) Snumber = 4;
    else if( key == 't' ) Snumber = 5;
    
    
      // ---------------- Video Number   ----------------
    
    else if( key == '1' ) number = 1;
    else if( key == '2' ) number = 2;
    else if( key == '3' ) number = 3;
    else if( key == '4' ) number = 4;
    else if( key == '5' ) number = 5;
  	else if( key == '6' ) number = 6;
    else if( key == '7' ) {

        vidPlayer[thisVid2].firstFrame();
        vidPlayer[thisVid2].stop();
        vidPlayer[thisVid1].firstFrame();
        vidPlayer[thisVid1].stop();
        
        ofFill();
        ofSetColor(0);
        ofRect(0,0,1440,900);
    }
    

    
    
    
    if(pNumber != number){
        
        string vidFile = "01/s"+ ofToString(number) +".mov";
        
        if (Snumber == 1) {
          vidFile = "01/s"+ ofToString(number) +".mov";
        }
        else if (Snumber == 2) {
          vidFile = "02/s"+ ofToString(number) +".mov";
        }
        else if (Snumber == 3) {
          vidFile = "03/s"+ ofToString(number) +".mov";
        }
        else if (Snumber == 4) {
          vidFile = "04/s"+ ofToString(number) +".mov";
        }
        else if (Snumber == 5) {
          vidFile = "05/s"+ ofToString(number) +".mov";
        }
        
        nextVid = number;
        vidPlayer[nextVid].loadMovie(vidFile);
        vidPlayer[nextVid].firstFrame();
    }
    
        if( key == 'z' ){
            numberBlend = 1;
        }
        else if( key == 'x' ){
            numberBlend = 2;
        }
        else if( key == 'c' ){
            numberBlend = 3;
        }
        else if( key == 'v' ){
            numberBlend = 4;
        }
        
        if( key == ' ' ){
            
            if(vidPlayer[thisVid2].isPlaying()){
                vidPlayer[thisVid2].stop();
            } else {
            
                vidPlayer[thisVid2].play();
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
