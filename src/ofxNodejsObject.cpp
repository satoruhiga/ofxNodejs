#include "ofxNodejsObject.h"

OFX_NODEJS_BEGIN_NAMESPACE

Object::Object() 
{
	v = v8::Persistent<v8::Value>::New(v8::Undefined()); 
}
	
Object::Object(v8::Handle<v8::Value> value)
{
	v8::HandleScope scope;
	if (!value->IsFunction())
		v = v8::Persistent<v8::Value>::New(value);
	else
		v = value;
}

Object::Object(bool o)
{
	v8::HandleScope scope;
	v = v8::Persistent<v8::Value>::New(v8::Boolean::New(o));
}

Object::Object(int o)
{
	v8::HandleScope scope;
	v = v8::Persistent<v8::Value>::New(v8::Int32::New(o));
}

Object::Object(unsigned int o)
{
	v8::HandleScope scope;
	v = v8::Persistent<v8::Value>::New(v8::Uint32::New(o));
}

Object::Object(float o)
{
	v8::HandleScope scope;
	v = v8::Persistent<v8::Value>::New(v8::Number::New(o));
}

Object::Object(double o)
{
	v8::HandleScope scope;
	v = v8::Persistent<v8::Value>::New(v8::Number::New(o));
}

Object::Object(string o)
{
	v8::HandleScope scope;
	v = v8::Persistent<v8::Value>::New(v8::String::New(o.c_str()));
}

Object::Object(const char* o)
{
	v8::HandleScope scope;
	v = v8::Persistent<v8::Value>::New(v8::String::New(o));
}

Object::~Object()
{
	v8::HandleScope scope;
	
	if (!v->IsFunction())
	{
		v8::Persistent<v8::Value> p = v8::Persistent<v8::Value>(v);
		p.Dispose();
	}

	v.Clear();
}

Object::Object(const Object& copy)
{
	*this = copy;
}

Object& Object::operator=(const Object& copy)
{
	if (!v.IsEmpty())
	{
		if (!v->IsFunction())
		{
			v8::Persistent<v8::Value> p = v8::Persistent<v8::Value>(v);
			p.Dispose();
		}
	}
	v = v8::Persistent<v8::Value>::New(copy.v);
}

OFX_NODEJS_END_NAMESPACE