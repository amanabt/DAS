#ifndef __DAS_RUNTIME_ERROR__
#define __DAS_RUNTIME_ERROR__

#include <stdexcept>

namespace DAS {

class RuntimeError : public std::runtime_error
{
public:
	RuntimeError (const char* what) :
		std::runtime_error (what)
	{}
};
}; //DAS namespace
#endif
