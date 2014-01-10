#pragma once

#include "ofMain.h"
#include "v8.h"

#include "ofxNodejsConstants.h"

OFX_NODEJS_BEGIN_NAMESPACE

class Object;
	
class Function
{
public:
	
	Function() {}
	Function(const string& object, const string& method);
	Function(v8::Handle<v8::Function> func);
	
	Object operator()();
	
	Object operator()(const Object& a1);
	
	Object operator()(const Object& a1, 
					  const Object& a2);
	
	Object operator()(const Object& a1, 
					  const Object& a2, 
					  const Object& a3);
	
	Object operator()(const Object& a1, 
					  const Object& a2, 
					  const Object& a3,
					  const Object& a4);
	
	Object operator()(const Object& a1,
					  const Object& a2,
					  const Object& a3, 
					  const Object& a4,
					  const Object& a5);

	Object operator()(const Object& a1,
					  const Object& a2,
					  const Object& a3, 
					  const Object& a4,
					  const Object& a5,
					  const Object& a6);

	Object operator()(const Object& a1,
					  const Object& a2,
					  const Object& a3,
					  const Object& a4,
					  const Object& a5,
					  const Object& a6, 
					  const Object& a7);
	
	Object operator()(const Object& a1, 
					  const Object& a2,
					  const Object& a3, 
					  const Object& a4,
					  const Object& a5,
					  const Object& a6,
					  const Object& a7,
					  const Object& a8);
	
	Object operator()(const Object& a1,
					  const Object& a2,
					  const Object& a3,
					  const Object& a4, 
					  const Object& a5,
					  const Object& a6,
					  const Object& a7,
					  const Object& a8, 
					  const Object& a9);
	
	Object operator()(const Object& a1,
					  const Object& a2,
					  const Object& a3,
					  const Object& a4, 
					  const Object& a5,
					  const Object& a6,
					  const Object& a7,
					  const Object& a8, 
					  const Object& a9,
					  const Object& a10);
	
private:
	
	v8::Handle<v8::Function> f;
	
};

OFX_NODEJS_END_NAMESPACE