#include "PyVar.hxx"

namespace types{
	PyVar::Pyvar(PyObject *_data){
		data = _data;
		Py_INCREF(data);
	}
	PyVar::~Pyvar(){
		Py_DECREF(data);
	}
	bool Pyvar::hasToString(){
		return true;
	}
	bool PyVar::toString(std::wostringstream& ostr){
		PyObject *str_rep = PyObject_Repr(data);
		ostr << PyUnicode_AsUTF8(str_rep);
		Py_DECREF(str_rep);
		return true;
	}
	bool PyVar::extract(const std::wstring& name, InternalType *& out){
		const wchar_t *winput = name.c_str();
		char *input = new char[wcslen(winput) + 1];
		sprintf(input,"%ws",winput);
		if (!PyObject_HasAttrString(data,input)){
			std::string err = "Python variable has no attribute '";
			err.append(input);
			delete input;
			throw ast::InternalError(err + "'");
		}
		out = new PyVar(PyObject_GetAttrString(data,input));
		delete input;
		return true;
	}
	bool PyVar::isInvokable() const{
		return PyCallable_Check(data);
	}
	bool Pyvar::invoke(types::typed_list & in, types::optional_list & opt, int _iRetCount, types::typed_list & out, const ast::Exp & e){
		PyObject *newTuple = PyTuple_New(in.size());
		for(size_t i=0; i<in.size(); i++){
			if(in[i] -> isBool()){
				int *val = in[i] -> getAs<Bool>() -> get();
				PyObject *item = PyBool_FromLong(*val);
				PyTuple_SetItem(newTuple,i,item);
			}
			else if(in[i] -> isDouble() || in[i] -> isFloat() || in[i] -> isInt()){
				double *val = in[i] -> getAs<Double>() -> get();
				PyObject *item = PyFloat_FromDouble(*val);
				PyTuple_SetItem(newTuple,i,item);
			}
			else if(in[i] -> isString()){
				wchar_t **val = in[i] -> getAs<String>() -> get();
				PyObject *item = PyUnicode_FromWideChar(*val,wcslen(*val));
				PyTuple_SetItem(newTuple,i,item);
			}
			else{
				Py_DECREF(newTuple);
				throw ast::InternalError("Incompatible type");
				return true;
			}
		}
		PyObject *ret = PyObject_Call(data,newTuple,NULL);
		if(ret == NULL){
			throw ast::InternalError("An error occured while calling the function");
			return true;
		}
		PyVar *pout = new PyVar(ret);
		out.push_back(pOut);
		return true;
	}
}