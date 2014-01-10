#pragma once

#include "ofxNodejsConstants.h"

OFX_NODEJS_BEGIN_NAMESPACE


#define OFX_NODEJS_BEGIN_CLASS_DEFINE(CLASSNAME) \
static v8::Persistent<v8::Function>& getConstructor() { \
	static v8::Persistent<v8::Function> o; \
	return o; \
}; \
static const char* getClassName() { return #CLASSNAME; } \
static void Dispose(v8::Persistent<v8::Value> handle, void* self) { \
	delete static_cast<ObjectWrapper::WrapperClass*>(self); \
	handle.Dispose(); \
}\
static v8::Persistent<v8::Function> Init() { \
	v8::HandleScope handle_scope; \
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New); \
	tpl->SetClassName(v8::String::NewSymbol(getClassName())); \
	tpl->InstanceTemplate()->SetInternalFieldCount(1); \



#define OFX_NODEJS_END_CLASS_DEFINE \
	getConstructor() = v8::Persistent<v8::Function>::New(tpl->GetFunction()); \
	return getConstructor(); \
}



#define OFX_NODEJS_REGISTER_METHOD(NAME) tpl->PrototypeTemplate()->Set(v8::String::NewSymbol(#NAME), v8::FunctionTemplate::New(__wrap_ ## NAME)->GetFunction());



#define OFX_NODEJS_DEFINE_CONSTRUCTOR() static v8::Handle<v8::Value> New(const v8::Arguments& v8_args) \
{ \
	v8::HandleScope scope; \
	if (v8_args.IsConstructCall()) { \
		ObjectWrapper::WrapperClass *obj = new ObjectWrapper::WrapperClass; \
		\
		vector<ofxNodejs::Object> args; \
		for (int i = 0; i < v8_args.Length(); i++) \
			args.push_back(ofxNodejs::Object(v8_args[i])); \
		\
		__wrap_ ## New(&obj->value, args); \
		\
		obj->Wrap(v8_args.This()); \
		return v8_args.This(); \
	} else { \
		const int argc = 1; \
		v8::Local<v8::Value> argv[argc] = { v8_args[0] }; \
		return scope.Close(getConstructor()->NewInstance(argc, argv)); \
	} \
} \
static void __wrap_ ## New(ObjectWrapper::Type *self, const vector<ofxNodejs::Object>& args)



#define OFX_NODEJS_DEFINE_METHOD(NAME) \
static v8::Handle<v8::Value> __wrap_ ## NAME(const v8::Arguments& v8_args) { \
	v8::HandleScope handle_scope; \
	ObjectWrapper::Type* self = &ObjectWrap::Unwrap<ObjectWrapper::WrapperClass>(v8_args.This())->value; \
	\
	vector<ofxNodejs::Object> args; \
	for (int i = 0; i < v8_args.Length(); i++) \
		args.push_back(ofxNodejs::Object(v8_args[i])); \
	\
	return NAME(self, args); \
} \
static ofxNodejs::Object NAME(ObjectWrapper::Type *self, const vector<ofxNodejs::Object>& args)



template <typename T, typename W>
class ObjectWrapper : public node::ObjectWrap
{
public:
	typedef W WrapperClass;
	typedef T Type;
	T value;
	
	ObjectWrapper() {}
	ObjectWrapper(const T& value) : value(value) {}
};


OFX_NODEJS_END_NAMESPACE