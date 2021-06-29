#pragma once

#include "ofMain.h"
#include "ofxSerial.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofx::IO::SerialDevice device;
		
		bool sendData = false;
		int isSoundPlaying = 0;
		int currentWaveform = 0;
		int wrap(int kX, int const kLowerBound, int const kUpperBound);
		
		float origx;
		float origy;
};
