#include "ofxNodejsUtils.h"

OFX_NODEJS_BEGIN_NAMESPACE

const char* ToCString(const v8::String::Utf8Value& value)
{
	return *value ? *value : "<string conversion failed>";
}

void ReportException(v8::TryCatch* try_catch)
{
	v8::HandleScope handle_scope;
	v8::String::Utf8Value exception(try_catch->Exception());
	const char* exception_string = ToCString(exception);
	v8::Handle<v8::Message> message = try_catch->Message();
	if (message.IsEmpty())
	{
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		printf("%s\n", exception_string);
	}
	else
	{
		// Print (filename):(line number): (message).
		v8::String::Utf8Value filename(message->GetScriptResourceName());
		const char* filename_string = ToCString(filename);
		int linenum = message->GetLineNumber();
		printf("%s:%i: %s\n", filename_string, linenum, exception_string);
		// Print line of source code.
		v8::String::Utf8Value sourceline(message->GetSourceLine());
		const char* sourceline_string = ToCString(sourceline);
		printf("%s\n", sourceline_string);
		// Print wavy underline (GetUnderline is deprecated).
		int start = message->GetStartColumn();
		for (int i = 0; i < start; i++)
		{
			printf(" ");
		}
		int end = message->GetEndColumn();
		for (int i = start; i < end; i++)
		{
			printf("^");
		}
		printf("\n");
		v8::String::Utf8Value stack_trace(try_catch->StackTrace());
		if (stack_trace.length() > 0)
		{
			const char* stack_trace_string = ToCString(stack_trace);
			printf("%s\n", stack_trace_string);
		}
	}
}

v8::Handle<v8::Value> ExecuteString(v8::Handle<v8::String> source, v8::Handle<v8::Value> name)
{
	v8::HandleScope handle_scope;
	v8::TryCatch try_catch;
	v8::Handle<v8::Script> script = v8::Script::Compile(source, name);
	
	if (script.IsEmpty())
	{
		// Print errors that happened during compilation.
		ReportException(&try_catch);
		return v8::Undefined();
	}
	else
	{
		v8::Handle<v8::Value> result = script->Run();
		if (result.IsEmpty())
		{
			assert(try_catch.HasCaught());
			// Print errors that happened during execution.
			ReportException(&try_catch);
			return v8::Undefined();
		}
		else
		{
			assert(!try_catch.HasCaught());
			if (!result->IsUndefined())
			{
				return result;
			}
			return v8::Undefined();
		}
	}
}

OFX_NODEJS_END_NAMESPACE