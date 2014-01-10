#include "ofxNodejs.h"

#include "uv.h"
#include "v8_typed_array.h"

#include "ofxNodejsUtils.h"

#ifdef USE_JS_BINDING
extern "C" void of_initialize(v8::Handle<v8::Object> global_obj);
#endif

using namespace v8;

namespace node
{
	extern char** Init(int argc, char *argv[]);
	extern Handle<Object> SetupProcessObject(int argc, char *argv[]);
	extern void Load(Handle<Object> process_l);
	extern void EmitExit(v8::Handle<v8::Object> process_l);
	extern void RunAtExit();
}

OFX_NODEJS_BEGIN_NAMESPACE

static v8::Context::Scope *context_scope;
static v8::Persistent<v8::Context> context;
static v8::Handle<v8::Object> process_l;

static bool inited = false;

class NodeEventListener
{
public:
	
	NodeEventListener() {}
	
	void onUpdate(ofEventArgs&)
	{
		uv_run(uv_default_loop(), UV_RUN_NOWAIT);
	}

	void onExit(ofEventArgs&)
	{
		{
			node::EmitExit(process_l);
			node::RunAtExit();
		}
		
		context.Dispose();
		
		delete context_scope;
		context_scope = NULL;
		
		v8::V8::Dispose();
	}
	
} listener;

void init(string node_modules_path)
{
	node_modules_path = ofToDataPath(node_modules_path);

	//
	// setup node.js
	//

	char *argv[] = { "node", "" };
	int argc = 1;

	vector<string> paths;
	paths.push_back(ofToDataPath(node_modules_path));
	
	const char *NODE_PATH_cstr = getenv("NODE_PATH");
	if (NODE_PATH_cstr)
	{
		paths.push_back(NODE_PATH_cstr);
	}
	
	string NODE_PATH;
	for (int i = 0; i < paths.size(); i++)
	{
#ifdef TARGET_WIN32
		NODE_PATH += paths[i] + ";";
#else
		NODE_PATH += paths[i] + ":";
#endif
	}
	
	setenv("NODE_PATH", NODE_PATH.c_str(), 1);
	setenv("NODE_DISABLE_COLORS", "1", 1);
	setenv("NODE_NO_READLINE", "1", 1);

	{
		using namespace node;
		
		Init(argc, argv);
		
		V8::Initialize();
		
		v8::HandleScope handle_scope;
		
		context = Context::New();
		context_scope = new Context::Scope(context);

		process_l = node::SetupProcessObject(argc, argv);
		v8_typed_array::AttachBindings(context->Global());
		
		Load(process_l);
		
		uv_run(uv_default_loop(), UV_RUN_NOWAIT);
	}

	//
	// setup of events
	//

	ofAddListener(ofEvents().update, &listener, &NodeEventListener::onUpdate);
	ofAddListener(ofEvents().exit, &listener, &NodeEventListener::onExit);
	
#ifdef USE_JS_BINDING
	{
		v8::HandleScope handle_scope;
		v8::Local<v8::Object> global = context->Global();
		of_initialize(global);
	}
#endif
	
	inited = true;
}

//
	
Object eval(const string& source, const string& source_name)
{
	assert(inited);

	v8::HandleScope scope;
	return ExecuteString(v8::String::New(source.c_str()),
						 v8::String::NewSymbol(source_name.c_str()));
}

Object run(const string& path)
{
	assert(inited);
	
	ofBuffer buffer;

	if (path.substr(0, 7) == "http://")
		buffer = ofLoadURL(path).data;
	else
		buffer = ofBufferFromFile(path);

	return $(buffer.getText(), path);
}

Function registerFunc(string funcname, v8::InvocationCallback function)
{
	assert(inited);
	
	v8::HandleScope scope;
	v8::Local<v8::Object> global = context->Global();
	
	v8::Local<v8::Function> func = v8::FunctionTemplate::New(function)->GetFunction();
	global->Set(v8::String::NewSymbol(funcname.c_str()), func);
	
	return Function(func);
}

OFX_NODEJS_END_NAMESPACE
