#include "testApp.h"

#include "ofxNodejs.h"

using namespace ofxNodejs;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	ofxNodejs::init();
	
	// see http://localhost:8080
	$("var http = require('http');"
	  "var color = 0;"
      "http.createServer(function (request, response) {"
	  "    var t = Date.now();"
	  "    console.log('access at ' + t);"
      "    response.writeHead(200, {'Content-Type': 'text/plain'});"
      "    response.end('Hello oF! ' + t + '\\n');"
      "    color = Math.random() * 255;"
      "}).listen(8080);");
	
	ofLaunchBrowser("http://localhost:8080/");
}

//--------------------------------------------------------------
void testApp::update()
{
}

//--------------------------------------------------------------
void testApp::draw()
{
	int color = $("color").as<int>();
	ofBackground(color);
	ofDrawBitmapString("color = " + ofToString(color), 10, 20);
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