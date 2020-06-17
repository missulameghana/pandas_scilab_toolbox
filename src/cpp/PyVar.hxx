#include <Python.h>
#include "alltypes.hxx"
#include "user.hxx"

#ifdef DLL_EXPORTS
#define DLL_IMPEXP __declspec(dllexport)
#else
#define DLL_IMPEXP __declspec(dllimport)
#endif

namespace types{
	class DLL_IMPEXP PyVar : public UserType{
	public:
		PyObject *data;
		PyVar(PyObject *_data);
		~PyVar();
		hasToString();
		toString();
		extract();
		isInvokable();
		invoke(types::typed_list & in, types::optional_list & opt, int _iRetCount, types::typed_list & out, const ast::Exp & e);
	}
}