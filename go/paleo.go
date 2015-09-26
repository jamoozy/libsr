package paleo

import (
  "errors"
)

// #cgo CFLAGS: -I../src
// #cgo LDFLAGS: -lsr -lm -lcblas -llapacke
// #include "paleo/paleo.h"
import "C"
// Got to add pkg-config stuff!

type Stroke struct {
  Pts [][3]int64
}

// Creates a new stroke.
func NewStroke() Stroke {
  return Stroke{Pts: make([][3]int64, 0, 100)}
}

func (s *Stroke) Recognize() (int, error) {
  // Build stroke.
  cStrk := C.stroke_create(C.int(len(s.Pts)))
  defer C.stroke_destroy(cStrk)
  for _, pt := range s.Pts {
    C.stroke_add_timed(cStrk, C.long(pt[0]), C.long(pt[1]), C.long(pt[2]))
  }

  // Recognize it.
  palType := C.pal_recognize(cStrk)
  if palType <= C.PAL_TYPE_UNRUN || C.PAL_TYPE_NUM <= palType {
    return 0, errors.New("Unknown error.  Sorry!")
  }
  return int(palType), nil
}
