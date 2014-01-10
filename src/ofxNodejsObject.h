#pragma once

#include <tr1/unordered_map>

#include "ofMain.h"
#include "v8.h"

#include "ofxNodejsFunction.h"

OFX_NODEJS_BEGIN_NAMESPACE

class Object
{
public:

	Object();
	Object(v8::Handle<v8::Value> value);
	Object(bool o);
	Object(int o);
	Object(unsigned int o);
	Object(float o);
	Object(double o);
	Object(string o);
	Object(const char* o);
	
	~Object();

	Object(const Object& copy);
	Object& operator=(const Object& copy);
	
	template<class T>
	inline T as() const
	{
		ofLogError("ofxNodejs") << "unregisted type";
		return T();
	}
	
	inline bool isArray() const { return v->IsArray(); }
	inline bool isObject() const { return v->IsObject(); }
	
	size_t size()
	{
		v8::HandleScope handle_scope;
		
		if (isArray())
		{
			v8::Array *arr = v8::Array::Cast(*v);
			return arr->Length();
		}
		else if (isObject())
		{
			v8::Object *obj = v8::Object::Cast(*v);
			return obj->GetOwnPropertyNames()->Length();
		}
		else return 0;
	}
	
	Object get(size_t index)
	{
		v8::HandleScope handle_scope;
		
		if (v->IsObject())
		{
			v8::Object *obj = v8::Object::Cast(*v);
			return Object(obj->Get(index));
		}
		else
		{
			return Object(v8::Null());
		}
	}
	
	Object get(const string& key)
	{
		v8::HandleScope handle_scope;
		
		if (v->IsObject())
		{
			v8::Object *obj = v8::Object::Cast(*v);
			v8::Local<v8::String> key_v8 = v8::String::NewSymbol(key.c_str());
			return Object(obj->Get(key_v8));
		}
		else
		{
			return Object(v8::Null());
		}
	}

	operator v8::Handle<v8::Value>() { return v; }
	operator const v8::Handle<v8::Value>() const { return v; }

private:

	v8::Handle<v8::Value> v;
};

template<>
inline bool Object::as() const
{
	v8::HandleScope scope;
	return v->BooleanValue();
}

template<>
inline int32_t Object::as() const
{
	v8::HandleScope scope;
	return v->Int32Value();
}

template<>
inline uint32_t Object::as() const
{
	v8::HandleScope scope;
	return v->Uint32Value();
}

template<>
inline float Object::as() const
{
	v8::HandleScope scope;
	return v->NumberValue();
}

template<>
inline double Object::as() const
{
	v8::HandleScope scope;
	return v->NumberValue();
}

template<>
inline string Object::as() const
{
	v8::HandleScope scope;

	if (v->IsArray())
	{
		string str;

		v8::Local<v8::Array> a = v8::Local<v8::Array>(v8::Array::Cast(*v));
		for (int i = 0; i < a->Length(); i++)
		{
			Object o = a->Get(i);
			str += o.as<string>() + ", ";
		}

		if (!str.empty()) str.resize(str.length() - 2);

		return "[" + str + "]";
	}
	else if (v->IsString())
	{
		return *v8::String::Utf8Value(v);
	}
	else if (v->IsObject())
	{
		v8::HandleScope scope;
		return ofxNodejs::Function("JSON", "stringify")(*this).as<string>();
	}
	else
	{
		return *v8::String::Utf8Value(v);
	}
}

template<>
inline Function Object::as() const
{
	v8::HandleScope scope;
	
	if (!v->IsFunction())
	{
		ofLogError("ofxNodejs") << "!function";
		return Function();
	}

	v8::Local<v8::Function> func = v8::Local<v8::Function>(v8::Function::Cast(*v));
	return Function(func);
}

inline std::ostream& operator<<(std::ostream& os, const Object& obj)
{
	os << obj.as<string>();
	return os;
}

OFX_NODEJS_END_NAMESPACE