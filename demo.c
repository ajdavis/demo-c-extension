#include <Python.h>

/* A global variable. */
static long gvar = 0;

/* A class declared in Python, cached here. */
static PyObject* MyClass;

static PyObject* in_sub_interpreter(PyObject* self, PyObject* args)
{
    static PyInterpreterState* main_interpreter = NULL;
    PyInterpreterState* interpreter;

    if (main_interpreter == NULL) {
        interpreter = PyInterpreterState_Head();
        while (interpreter->next)
            interpreter = interpreter->next;
        main_interpreter = interpreter;
    }

    if (main_interpreter == PyThreadState_Get()->interp) {
        Py_RETURN_FALSE;
    } else {
        Py_RETURN_TRUE;
    }
}

static PyObject* inc_and_get_gvar(PyObject* self, PyObject* args)
{
    gvar++;
    return PyInt_FromLong(gvar);
}

static PyObject* is_myclass(PyObject* self, PyObject* args)
{
    int outcome;
    PyObject* obj;
    if (!PyArg_ParseTuple(args, "O", &obj)) return NULL;

    outcome = PyObject_IsInstance(obj, MyClass);
    if (outcome) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

static PyObject* create_myclass(PyObject* self, PyObject* args)
{
    return PyObject_CallObject(MyClass, NULL);
}

static PyMethodDef Methods[] =
{
     {"in_sub_interpreter", in_sub_interpreter, METH_NOARGS,
      "True if running in a Python sub interpreter."},
     {"inc_and_get_gvar", inc_and_get_gvar, METH_NOARGS,
      "Increment an internal integer and get its value."},
     {"is_myclass", is_myclass, METH_VARARGS,
      "Returns isinstance(obj, MyClass)."},
     {"create_myclass", create_myclass, METH_NOARGS,
      "Returns an instance of MyClass."},
     {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initdemo(void)
{
    PyObject* mymodule = PyImport_ImportModule("mymodule");
    if (!mymodule) return;

    MyClass = PyObject_GetAttrString(mymodule, "MyClass");
    Py_DECREF(mymodule);
    if (!MyClass) return;

    Py_InitModule("demo", Methods);
}
