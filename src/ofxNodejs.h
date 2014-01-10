#pragma once

#include "ofMain.h"

#include "v8.h"
#include "node.h"
#include "node_object_wrap.h"

#include "ofxNodejsConstants.h"

#include "ofxNodejsObject.h"
#include "ofxNodejsFunction.h"
#include "ofxNodejsBinding.h"

OFX_NODEJS_BEGIN_NAMESPACE

void init(string node_modules_path = "node_modules");

Object eval(const string& source, const string& source_name = "<string>");
inline Object $(const string& source, const string& source_name = "<string>") { return eval(source, source_name); }

Object run(const string& path);
inline Object $$(const string& path) { return run(path); }

typedef ofxNodejs::Object (*FunctionCallback)(const vector<ofxNodejs::Object>& args);

template <FunctionCallback F>
v8::Handle<v8::Value> FunctionWrapper(const v8::Arguments& v8_args)
{
	v8::HandleScope handle_scope;
	
	vector<ofxNodejs::Object> args;
	for (int i = 0; i < v8_args.Length(); i++)
		args.push_back(ofxNodejs::Object(v8_args[i]));
	
	return F(args);
}

void registerFunc(string funcname, v8::InvocationCallback function);

template <FunctionCallback F>
void registerFunc(string funcname, v8::InvocationCallback function = FunctionWrapper<F>)
{
	registerFunc(funcname, function);
}

void registerClass(string classname, v8::Persistent<v8::Function> function);

template <typename T>
void registerClass()
{
	registerClass(T::getClassName(), T::Init());
}

OFX_NODEJS_END_NAMESPACE