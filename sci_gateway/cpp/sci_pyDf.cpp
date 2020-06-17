#include "PyVar.hxx"

extern "C" {
	#include "Scierror.h"
}
using namespace types;

Function::ReturnValue sci_pyDf(typed_list& in, int _iRetCount, typed_list& out){ //first two elements of in will be shape of dataframe next follow m*n elements
	if(_iRetCount > 1){
		Scierror(999, "pyDf: Wrong number of output arguments, 1 expected");
		return Function::Error;
	}
	if(!Py_IsInitialized()){
		Py_Initialize();
	}
	if(in.size() < 2){
		Scierror(999, "pyDf: Wrong number of input arguments, at least 2 expected");
		return Function::Error;
	}
	else{
		if(!(in[0] -> isInt() || in[1] -> isInt())){
			Scierror(999, "pyDf: First two arguments are expected to be of type Int");
			return Function::Error;
		}
		else{
			int a = in[0];
			int b = in[1];
			PyObject *newDf =  PyList_New(0);//constructor for dataframe-I'm thinking of lists of lists representation
			for(size_t j=1; j<a; j++){
				PyObject *temp = PyList_New(0);
				for(size_t i=0;i<b;i++){
				if(in[i+2+(j-1)*b] -> isBool()){
					int *val = in[i+2+(j-1)*b] -> getAs<Bool>() -> get();
					PyObject *item = PyBool_FromLong(*val);
					PyList_Append(temp,item);
				}
				else if(in[i+2+(j-1)*b] -> isDouble() || in[i+2+(j-1)*b] -> isFloat() || in[i+2+(j-1)*b] -> isInt()){
					double *val = in[i+2+(j-1)*b] -> getAs<Double>() -> get();
					PyObject *item = PyFloat_FromDouble(*val);
					PyList_Append(temp,item);
				}
				else if(in[i+2+(j-1)*b] -> isString()){
					wchar_t **val = in[i+2+(j-1)*b] -> getAs<String>() -> get();
					PyObject *item = PyUnicode_FromWideChar(*val,wcslen(*val));
					PyList_Append(temp,item);
				}
				else{
					Py_DECREF(newDf);
					Scierror(999, "pyDf: Incompatible type for argument %zu",i+1);
					return Function::Error;
				}
			}
			PyList_Append(newDf,temp);
		}
			PyVar *pOut = new PyVar(newDf);
			out.push_back(pOut);
			return Function::OK;

		}
	}
	

}