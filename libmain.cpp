#if defined(_MSC_BUILD)
#define _CRT_SECURE_NO_WARNINGS
#define DLLEXPORT __declspec (dllexport)
#define STDCALL __stdcall
#else
#define DLLEXPORT __attribute__((dllexport))
#define STDCALL 
#endif

#include <sstream>
#include "sqf-value/sqf-value/value.hpp"
#include "sqf-value/sqf-value/methodhost.hpp"
#include "runtime/src/runtime/logging.h"
#include "runtime/src/runtime/runtime.h"
//#include "runtime/src/fileio/default.h"

#include "runtime/src/parser/sqf/sqf_parser.hpp"
#include "runtime/src/parser/preprocessor/default.h"

extern "C"
{
	//--- Called by Engine on extension load 
	DLLEXPORT void STDCALL RVExtensionVersion(char* output, int outputSize);
	//--- STRING callExtension STRING
	DLLEXPORT void STDCALL RVExtension(char* output, int outputSize, const char* function);
	//--- STRING callExtension ARRAY
	DLLEXPORT int STDCALL RVExtensionArgs(char* output, int outputSize, const char* function, const char** argv, int argc);
}

#define COPY_STREAM(x) strcpy_s(output, x.str().length() + 1, x.str().c_str())

class stream : public std::stringstream
{
public:
	stream& set(const char* chars) noexcept {
		this->clear();
		this->str(chars);
		return *this;
	}
};

using ret_d = sqf::method::ret<sqf::value, std::string>;

void STDCALL RVExtension(char* output, int outputSize, const char* function)
{
	stream sstream;
	sstream.set("Error: Not enough input arguments");
	COPY_STREAM(sstream);
}

int STDCALL RVExtensionArgs(char* output, int outputSize, const char* function, const char** argv, int argc)
{
	auto res = sqf::methodhost::instance().execute(output, outputSize, function, argv, argc);
	return res;
}

void STDCALL RVExtensionVersion(char* output, int outputSize)
{
	stream sstream;
	sstream.set("v1.0");
	COPY_STREAM(sstream);
}


ret_d fnc_check()
{
	return ret_d::ok(sqf::value(true));
}

ret_d fnc_preprocess(std::string text) {
	StdOutLogger logger{};
	sqf::runtime::runtime runtimeObj(logger, {});
	sqf::parser::preprocessor::impl_default preproc(logger);
	auto preprocessed = preproc.preprocess(runtimeObj, text, { std::string(), {} });
	if (preprocessed.has_value())
	{
		return ret_d::ok(preprocessed.value());
	}
	else
	{
		return ret_d::err("Preprocessor failed");
	}
}

sqf::methodhost& sqf::methodhost::instance()
{
	using namespace std::string_literals;
	static sqf::methodhost h({
		{ "c", { sqf::method::create(fnc_check) } },
		{ "p", { sqf::method::create(fnc_preprocess) } }
		});
	return h;
}