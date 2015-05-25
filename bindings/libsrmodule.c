#include <Python.h>
#include <stdarg.h>


typedef struct {
  PyObject_HEAD
  /* fields ... */
} libsr_StrokeObject;

static PyTypeObject libsr_StrokeType = {
  PyObject_HEAD_INIT(NULL)
  0,                           // ob_size (must always be 0)
  "libsr.Stroke",              // tp_name
  sizeof(libsr_StrokeObject),  // tp_basicsize
  0,                           // tp_itemsize
  0,                           // tp_dealloc
  0,                           // tp_print
  0,                           // tp_getattr
  0,                           // tp_setattr
  0,                           // tp_compare
  0,                           // tp_repr
  0,                           // tp_as_number
  0,                           // tp_as_sequence
  0,                           // tp_as_mapping
  0,                           // tp_hash
  0,                           // tp_call
  0,                           // tp_str
  0,                           // tp_getattro
  0,                           // tp_setattro
  0,                           // tp_as_buffer
  Py_TPFLAGS_DEFAULT,          // tp_flags

  // Python docstring:
  "Holds a sequence of timestamped points.",
};

static PyMethodDef libsr_methods[] = {
  {NULL}  // Sentinel
};


#ifndef PyMODINIT_FUNC       // declarations for DLL import/export
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initlibsr(void) {
  PyObject* m;

  libsr_StrokeType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&libsr_StrokeType) < 0)
    return;

  m = Py_InitModule3("libsr", libsr_methods,
                     "Sketch recognition module written by Andrew \"Jamoozy\""
                     " C. Sabisch.  Distributed under the GPLv3.");

  Py_INCREF(&libsr_StrokeType);
  PyModule_AddObject(m, "Stroke", (PyObject*)&libsr_StrokeType);
}


// Saves a list of points (all passed as either (x,y) or (x,y,t) tuples) to the
// file name specified.
//   fname, str: The file name.
//   *args, list: The points.  They must all be in the same format: either
//                3-tuples or 2-tuples.
//static PyObject*
//libsr_Stroke(PyObject* self, PyObject* args) {
//  const char* fname;
//  PyObject* num_pts = PyObject_GetAttrString(args, "__len__)");
//
//  const int* pts = calloc(sizeof(int*), num_pts * 3);   // 3 ints per tuple
//  if (!PyArg_ParseTuple(args, "sO&", fname, _stroke_tuple_converter, pts)) {
//    returN NULL;
//  }
//
//  free(pts);
//}
