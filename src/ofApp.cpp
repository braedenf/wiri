#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofHideCursor();
	ofSetBackgroundColor(247, 247, 247);

	std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();

	ofLogNotice("ofApp::setup") << "Connected Devices: ";

	for (std::size_t i = 0; i < devicesInfo.size(); ++i)
	{
		ofLogNotice("ofApp::setup") << "\t" << devicesInfo[i];
	}

	if (!devicesInfo.empty())
	{
		// Connect to the first matching device.
		bool success = device.setup(devicesInfo[0], 115200);

		if (success)
		{
			ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[0];
		}
		else
		{
			ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[0];
		}
	}
	else
	{
		ofLogNotice("ofApp::setup") << "No devices connected.";
	}

	// END SERIAL SETUP

	origx = ofGetWidth() / 2;
	origy = ofGetHeight() / 2;
}

//--------------------------------------------------------------
void ofApp::update()
{	
	if(sendData) {
		// The serial device can throw exeptions.
		try
		{	
			if(isSoundPlaying) {
				std::string msg = '<' + to_string(isSoundPlaying) + ',' + to_string(ofGetMouseX()) + ',' + to_string(ofGetMouseY()) + ',' + to_string(currentWaveform) + '>';

				cout << msg << endl;

				//Write message to text buffer
				ofx::IO::ByteBuffer textBuffer(msg);

				//Write text buffer to serial
				device.writeBytes(textBuffer);
				device.writeByte('\n');
			} else {
				std::string msg = '<' + to_string(isSoundPlaying) + ',' + to_string(ofGetMouseX()) + ',' + to_string(ofGetMouseY()) + ',' + to_string(currentWaveform) + '>';

				cout << msg << endl;

				//Write message to text buffer
				ofx::IO::ByteBuffer textBuffer(msg);

				//Write text buffer to serial
				device.writeBytes(textBuffer);
				device.writeByte('\n');
				
				//stop writing data
				sendData = false;
			}
			
			
		}
		catch (const std::exception &exc)
		{
			ofLogError("ofApp::update") << exc.what();
		}
	}
	
}

//--------------------------------------------------------------
void ofApp::draw()
{

	float xPct = (float)(mouseX) / (float)(ofGetWidth());
	float yPct = (float)(mouseY) / (float)(ofGetHeight());
	int nTips = 5 + xPct * 30;
	int nStarPts = nTips * 2;
	float angleChangePerPt = TWO_PI / (float)nStarPts;
	float innerRadius = 80 + yPct * 60;
	float outerRadius = 80;

	float angle = 0;
	
	switch (currentWaveform) {
		case 0:
			ofSetColor(242, 174, 193);
			break;
		case 1:
			ofSetColor(160, 138, 191);
			break;
		case 2:
			ofSetColor(65, 208, 233);
			break;
		case 3:
			ofSetColor(239, 155, 63);
			break;
	}

	ofBeginShape();
	for (int i = 0; i < nStarPts; i++)
	{
		if (i % 2 == 0)
		{
			// inside point:
			float x = origx + innerRadius * cos(angle);
			float y = origy + innerRadius * sin(angle);
			ofVertex(x, y);
		}
		else
		{
			// outside point
			float x = origx + outerRadius * cos(angle);
			float y = origy + outerRadius * sin(angle);
			ofVertex(x, y);
		}
		angle += angleChangePerPt;
	}
	ofEndShape();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	//Start playing sound
	sendData = true;
	isSoundPlaying = 1;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
	//Stop playing sound
	isSoundPlaying = 0;
	currentWaveform = wrap(currentWaveform + 1, 0, 3);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

//--------------------------------------------------------------
int ofApp::wrap(int kX, int const kLowerBound, int const kUpperBound)
{
    int range_size = kUpperBound - kLowerBound + 1;

    if (kX < kLowerBound)
        kX += range_size * ((kLowerBound - kX) / range_size + 1);

    return kLowerBound + (kX - kLowerBound) % range_size;
}