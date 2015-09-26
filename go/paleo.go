package paleo

import (
  "errors"
)

// #cgo CFLAGS: -I../src
// #cgo LDFLAGS: -L../build/src/.libs -lsr
// #include "paleo/paleo.h"
import "C"

type Stroke struct {
  c *C.stroke_t
}

func recognize(stroke Stroke) (int, error) {
  palType := C.pal_recognize(stroke.c)
  if palType <= C.PAL_TYPE_UNRUN || C.PAL_TYPE_NUM <= palType {
    return 0, errors.New("Unknown error.  Sorry!")
  }
  return int(palType), nil
}
