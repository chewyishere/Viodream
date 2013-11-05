#pragma once
#include "ofMain.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    
    // video
    ofVideoPlayer vidPlayer[9];
    
    int nextVid = 0;
    int thisVid1 = 0; // this is the current video number playing
    int thisVid2 = 0;
    int prevVid = 0;
      
   
    //audio
    void    audioIn(float * input, int bufferSize, int nChannels);
    
    
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    
    int 	bufferCounter;
    int 	drawCounter;
    
    
    ofSoundStream soundStream;
    float smoothedVol;
    float scaledVol;
    
    
    //mode selector
    int number;
    int numberBlend;
    int Snumber;
    
    //blendmode
    ofBlendMode blendMode;

    
    // speed
    float currentSpeed;
    
    
    
    
};
