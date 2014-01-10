#pragma once

#include "ofMain.h"

#include "v8.h"
#include "node.h"
#include "node_object_wrap.h"

#include "ofxNodejsConstants.h"

#include "ofxNodejsObject.h"
#include "ofxNodejsFunction.h"

// #define USE_JS_BINDING

OFX_NODEJS_BEGIN_NAMESPACE

void init(string node_modules_path = "node_modules");

Object eval(const string& source, const string& source_name = "<string>");
inline Object $(const string& source, const string& source_name = "<string>") { return eval(source, source_name); }

Object run(const string& path);
inline Object $$(const string& path) { return run(path); }

Function registerFunc(string funcname, v8::InvocationCallback function);

OFX_NODEJS_END_NAMESPACE