#include "testApp.h"

#include "ofxNodejs.h"

ofxNodejs::Object myfunc(const vector<ofxNodejs::Object>& args)
{
	cout << args[0].as<string>() << endl;
	cout << "this is cpp function" << endl;
	return 42;
}

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	ofxNodejs::init();
	ofxNodejs::registerFunc<myfunc>("cppfunc");
	ofxNodejs::eval("console.log(cppfunc(10, 20));");
	
	ofExit(0);
}

//--------------------------------------------------------------
void testApp::update()
{
}

//--------------------------------------------------------------
void testApp::draw()
{
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}