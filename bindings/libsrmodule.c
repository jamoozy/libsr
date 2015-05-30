#include <Python.h>
#include <structmember.h>

#include <stdarg.h>
#include <time.h>

#include "stroke.h"


typedef struct {
  PyObject_HEAD
  stroke_t* stroke;   // The underlying libsr stroke_t in src/common/
} libsr_Stroke;

static PyObject*
Stroke_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  libsr_Stroke* self = (libsr_Stroke*)type->tp_alloc(type, 0);
  if (self != NULL) {
    self->stroke = stroke_create(40);
  }
  return (PyObject*)self;
}

static int
Stroke_init(libsr_Stroke* self, PyObject* args, PyObject* kwargs) {
  return 0;
}

static void
Stroke_dealloc(libsr_Stroke* self) {
  stroke_destroy(self->stroke);
  self->ob_type->tp_free((PyObject*)self);
}

static int
Stroke_compare(libsr_Stroke* a, libsr_Stroke* b) {
  if (a->stroke->num != 0 || b->stroke->num != 0) {
    PyErr_SetString(PyExc_ValueError, "Cannot compare empty stroke!.");
    return -1;
  }

  // Compare them temporally.
  int diff = b->stroke->pts[0].t - a->stroke->pts[0].t;
  return diff > 0 ? 1 : diff < 0 ? -1 : 0;
}

static PyObject*
Stroke_add(libsr_Stroke* self, PyObject* args, PyObject* kwargs) {
  long x = 0, y = 0, t = 0;

  static char* kwlist[] = {"x", "y", "t", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii|i", kwlist, &x, &y, &t)) {
    PyErr_SetString(PyExc_ValueError, "Expected (x,y) or (x,y,t).");
    return NULL;
  }

  // If no time, just use the current time.
  if (!t) {
    time_t raw_time;
    time(&raw_time);
    t = raw_time;
  }

  stroke_add_timed(self->stroke, x, y, t);
  Py_RETURN_NONE;
}

static PyObject*
Stroke_save(libsr_Stroke* self, PyObject* args, PyObject* kwargs) {
  const char* fname = NULL;
  static char* kwlist[] = {"fname", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", kwlist, &fname)) {
    PyErr_SetString(PyExc_ValueError, "Need a string argument.");
    return NULL;
  }
  stroke_save(self->stroke, fname);
  Py_RETURN_NONE;
}

// Stroke iterators.  These return each of the points in a stroke as an (x,y,t)
// tuple.
typedef struct {
  PyObject_HEAD
  libsr_Stroke* libsr_stroke;   // The stroke we're iterating over.
  int next;                     // The next point to return.
} libsr_StrokeIter;

static PyObject*
StrokeIter_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  libsr_StrokeIter* self = (libsr_StrokeIter*)type->tp_alloc(type, 0);
  if (!self) {
    return NULL;
  }

  self->next = 0;

  libsr_Stroke* libsr_stroke;
  static char* kwlist[] = {"stroke", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", kwlist, &libsr_stroke)) {
    self->libsr_stroke = libsr_stroke;
    Py_INCREF(self->libsr_stroke);
  }
  return (PyObject*)self;
}

static int
StrokeIter_init(libsr_StrokeIter* self, PyObject* args, PyObject* kwargs) {
  return 0;
}

static PyObject*
StrokeIter_iter(libsr_StrokeIter* self) {
  Py_INCREF(self);
  return (PyObject*)self;
}

static PyObject*
StrokeIter_iternext(libsr_StrokeIter* self) {
  if (self->next >= self->libsr_stroke->stroke->num) {
    PyErr_SetString(PyExc_StopIteration, "Done.");
    return NULL;
  }

  // Convert to python (x,y,t,i) tuple.
  point_t* p = &self->libsr_stroke->stroke->pts[self->next++];
  return Py_BuildValue("(iiii)", &p->x, &p->y, &p->t, &p->i);
}

static void
StrokeIter_dealloc(libsr_StrokeIter* self) {
  Py_CLEAR(self->libsr_stroke);
  self->ob_type->tp_free((PyObject*)self);
}


// Iterator over stroke objects.  These iterators return one point at a time,
// in tuple representation.
static PyTypeObject libsr_StrokeIterType = {
  PyObject_HEAD_INIT(NULL)
  0,                          // ob_size (must always be 0)
  "libsr.StrokeIter",         // tp_name
  sizeof(libsr_StrokeIter),   // tp_basicsize
  0,                          // tp_itemsize
  (destructor)StrokeIter_dealloc, // tp_dealloc
  0,                          // tp_print
  0,                          // tp_getattr
  0,                          // tp_setattr
  0,                          // tp_compare
  0,                          // tp_repr
  0,                          // tp_as_number
  0,                          // tp_as_sequence
  0,                          // tp_as_mapping
  0,                          // tp_hash
  0,                          // tp_call
  0,                          // tp_str
  0,                          // tp_getattro
  0,                          // tp_setattro
  0,                          // tp_as_buffer
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/

  // Python docstring:
  "Holds a sequence of timestamped points.",

  0,                      // tp_traverse
  0,                      // tp_clear
  0,                      // tp_richcompare
  0,                      // tp_weaklistoffset
  (getiterfunc)StrokeIter_iter,       // tp_iter
  (iternextfunc)StrokeIter_iternext,  // tp_iternext
  0,                      // tp_methods
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

static PyObject*
Stroke_iter(libsr_Stroke* self) {
  // Create libsr_StrokeIter
  // inc ref?
  return StrokeIter_new(&libsr_StrokeIterType, (PyObject*)self, NULL);
}

static PyMethodDef Stroke_methods[] = {
  {"add", (PyCFunction)Stroke_add, METH_VARARGS,
   "Adds an (x,y,t) coordinate to this stroke."},
  {"save", (PyCFunction)Stroke_save, METH_VARARGS,
   "Saves the stroke to disk."},
  {NULL, NULL, 0, NULL}
};

static PyTypeObject libsr_StrokeType = {
  PyObject_HEAD_INIT(NULL)
  0,                          // ob_size (must always be 0)
  "libsr.Stroke",             // tp_name
  sizeof(libsr_Stroke),       // tp_basicsize
  0,                          // tp_itemsize
  (destructor)Stroke_dealloc, // tp_dealloc
  0,                          // tp_print
  0,                          // tp_getattr
  0,                          // tp_setattr
  (cmpfunc)Stroke_compare,    // tp_compare
  0,                          // tp_repr
  0,                          // tp_as_number
  0,                          // tp_as_sequence
  0,                          // tp_as_mapping
  0,                          // tp_hash
  0,                          // tp_call
  0,                          // tp_str
  0,                          // tp_getattro
  0,                          // tp_setattro
  0,                          // tp_as_buffer
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/

  // Python docstring:
  "Holds a sequence of timestamped points.",

  0,                      // tp_traverse
  0,                      // tp_clear
  0,                      // tp_richcompare
  0,                      // tp_weaklistoffset
  (getiterfunc)Stroke_iter, // tp_iter
  0,                      // tp_iternext
  Stroke_methods,         // tp_methods
  0,                      // tp_members
  0,                      // tp_getset
  0,                      // tp_base
  0,                      // tp_dict
  0,                      // tp_descr_get
  0,                      // tp_descr_set
  0,                      // tp_dictoffset
  (initproc)StrokeIter_init,  // tp_init
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
