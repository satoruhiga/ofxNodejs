#include "testApp.h"

#include "ofxNodejs.h"

// function

ofxNodejs::Object myfunc(const vector<ofxNodejs::Object>& args)
{
	cout << "this is cpp function" << endl;
	return 42;
}

int num_instance = 0;


// Target class

class MyClass
{
public:
	
	string name;
	
	MyClass()
	{
		cout << "cnst: " << num_instance++ << endl;
	}
	
	~MyClass()
	{
		cout << "dest: " << --num_instance << endl;
	}
	
	void print()
	{
		cout << "print: " << name << endl;
	}
};

// Wrapper class

class MyClassWrap : ofxNodejs::ObjectWrapper<
	MyClass, // Target Class
	MyClassWrap // This class
>
{
public:
	
	OFX_NODEJS_BEGIN_CLASS_DEFINE(MyClass)
	{
		// register method
		OFX_NODEJS_REGISTER_METHOD(testWrap);
	}
	OFX_NODEJS_END_CLASS_DEFINE;

	
	OFX_NODEJS_DEFINE_CONSTRUCTOR()
	{
		// init MyClass with args = const vector<ofxNodejs::Object>&
		if (args.size() > 0)
			self->name = args[0].as<string>();
		else
			self->name = "NONE";
	}
	
	
	OFX_NODEJS_DEFINE_METHOD(testWrap)
	{
		cout << "args.size(): " << args.size() << endl;
		
		self->print();
		return NULL;
	}
};


//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	ofxNodejs::init();
	ofxNodejs::registerFunc<myfunc>("cppfunc");
	ofxNodejs::eval("console.log(cppfunc(10, 20));");
	
	ofxNodejs::registerClass<MyClassWrap>();
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
	// run external script
	ofxNodejs::run("test.js");
	
	// force call gc
	if (key == OF_KEY_BACKSPACE || key == OF_KEY_DEL)
		ofxNodejs::eval("global.gc();");
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