#include <Python.h>
#include "datatable.h"
#include "py_datawindow.h"
#include "py_rowindex.h"
#include "dtutils.h"
#include "rows.h"



//------------------------------------------------------------------------------
// Module definition
//------------------------------------------------------------------------------

static PyMethodDef DatatableModuleMethods[] = {
    {"select_row_slice", (PyCFunction)select_row_slice, METH_VARARGS,
        "Row selector constructed from a slice of rows"},
    {"select_row_indices", (PyCFunction)select_row_indices, METH_VARARGS,
        "Row selector constructed from a list of row indices"},

    {NULL, NULL, 0, NULL}  /* Sentinel */
};

static PyModuleDef datatablemodule = {
    PyModuleDef_HEAD_INIT,
    "_datatable",  /* name of the module */
    "module doc",  /* module documentation */
    -1,            /* size of per-interpreter state of the module, or -1
                      if the module keeps state in global variables */
    DatatableModuleMethods
};

/* Called when Python program imports the module */
PyMODINIT_FUNC
PyInit__datatable(void) {
    PyObject *m;

    // Sanity checks
    assert(sizeof(char) == sizeof(unsigned char));

    dt_Coltype_size[DT_AUTO] = 0;
    dt_Coltype_size[DT_DOUBLE] = sizeof(double);
    dt_Coltype_size[DT_LONG] = sizeof(long);
    dt_Coltype_size[DT_BOOL] = sizeof(char);
    dt_Coltype_size[DT_STRING] = sizeof(char*);
    dt_Coltype_size[DT_OBJECT] = sizeof(PyObject*);

    Py_int0 = PyLong_FromLong(0);
    Py_int1 = PyLong_FromLong(1);

    dt_DatatableType.tp_new = PyType_GenericNew;
    DataWindow_PyType.tp_new = PyType_GenericNew;
    RowIndex_PyType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&dt_DatatableType) < 0 ||
        PyType_Ready(&DataWindow_PyType) < 0 ||
        PyType_Ready(&RowIndex_PyType) < 0)
        return NULL;

    m = PyModule_Create(&datatablemodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&dt_DatatableType);
    Py_INCREF(&DataWindow_PyType);
    Py_INCREF(&RowIndex_PyType);
    PyModule_AddObject(m, "DataTable", (PyObject*) &dt_DatatableType);
    PyModule_AddObject(m, "DataWindow", (PyObject*) &DataWindow_PyType);
    PyModule_AddObject(m, "RowIndex", (PyObject*) &RowIndex_PyType);
    return m;
}
