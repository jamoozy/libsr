#include <stdarg.h>
#include "mock_stroke.h"

stroke_t* mock_stroke(int num, ...) {
  va_list argp;

  stroke_t* strk = malloc(sizeof(stroke_t));
  strk->num = strk->size = num;
  strk->pts = calloc(num, sizeof(point_t));

  va_start(argp, num);
  for (int i = 0; i < num; i++) {
    strk->pts[i].x = va_arg(argp, double);
    strk->pts[i].y = va_arg(argp, double);
    strk->pts[i].t = va_arg(argp, long);
    strk->pts[i].i = i;
  }
  va_end(argp);

  return strk;
}
