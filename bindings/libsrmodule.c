#include <Python.h>
#include <structmember.h>

#include <stdarg.h>

#include "stroke.h"


typedef struct {
  PyObject_HEAD
  stroke_t* stroke;   // The underlying libsr stroke_t in src/common/
} libsr_Stroke;

static PyObject *
Stroke_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  libsr_Stroke* self = (libsr_Stroke*)type->tp_alloc(type, 0);
  if (self != NULL) {
    self->stroke = stroke_create(40);
  }
  return (PyObject*)self;
}

static void
Stroke_dealloc(libsr_Stroke* self) {
    stroke_destroy(self->stroke);
    self->ob_type->tp_free((PyObject*)self);
}

static int
Stroke_init(libsr_Stroke* self, PyObject* args, PyObject* kwargs) {
  return 0;
}


static int
Stroke_add(libsr_Stroke* self, PyObject* args, PyObject* kwargs) {
  long x = 0, y = 0, t = 0;
  static char* kwlist[] = {"x", "y", "t", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii|i", kwlist, &x, &y, &t)) {
    return -1;
  }
  stroke_add_timed(self->stroke, x, y, t);
  return 0;
}

static int
Stroke_save(libsr_Stroke* self, PyObject* args, PyObject* kwargs) {
  const char* fname = NULL;
  static char* kwlist[] = {"fname", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &fname)) {
    return -1;
  }
  stroke_save(self->stroke, fname);
  return 0;
}

static PyMethodDef Stroke_methods[] = {
  {"from_file", (PyCFunction)Stroke_add, METH_VARARGS, ""},
  {"add", (PyCFunction)Stroke_add, METH_VARARGS, ""},
  {"save", (PyCFunction)Stroke_save, METH_VARARGS,
   "Saves the stroke to disk."},
  {NULL, NULL, 0, NULL}
};


static PyTypeObject libsr_StrokeType = {
  PyObject_HEAD_INIT(NULL)
  0,                     // ob_size (must always be 0)
  "libsr.Stroke",        // tp_name
  sizeof(libsr_Stroke),  // tp_basicsize
  0,                     // tp_itemsize
  (destructor)Stroke_dealloc, // tp_dealloc
  0,                     // tp_print
  0,                     // tp_getattr
  0,                     // tp_setattr
  0,                     // tp_compare
  0,                     // tp_repr
  0,                     // tp_as_number
  0,                     // tp_as_sequence
  0,                     // tp_as_mapping
  0,                     // tp_hash
  0,                     // tp_call
  0,                     // tp_str
  0,                     // tp_getattro
  0,                     // tp_setattro
  0,                     // tp_as_buffer
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/

  // Python docstring:
  "Holds a sequence of timestamped points.",

  0,                      // tp_traverse
  0,                      // tp_clear
  0,                      // tp_richcompare
  0,                      // tp_weaklistoffset
  0,                      // tp_iter
  0,                      // tp_iternext
  Stroke_methods,         // tp_methods
  0,                      // tp_members
  0,                      // tp_getset
  0,                      // tp_base
  0,                      // tp_dict
  0,                      // tp_descr_get
  0,                      // tp_descr_set
  0,                      // tp_dictoffset
  (initproc)Stroke_init,  // tp_init
  0,                      // tp_alloc
  Stroke_new,             // tp_new
};



////////////////////////////////////////////////////////////////////////////////
//                               Module Methods                               //
////////////////////////////////////////////////////////////////////////////////

static PyMethodDef libsr_methods[] = {
  {NULL, NULL, 0, NULL}  // Sentinel
};


#ifndef PyMODINIT_FUNC       // declarations for DLL import/export
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initlibsr(void) {
  if (PyType_Ready(&libsr_StrokeType) < 0)
    return;

  PyObject* m = Py_InitModule3(
      "libsr", libsr_methods,
      "Sketch recognition module written by Andrew \"Jamoozy\" C. Sabisch. "
      "Distributed under the GPLv3.");

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
