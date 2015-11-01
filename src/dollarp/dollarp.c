
#include <math.h>
#include <string.h>
#include <values.h>

#include "common/geom.h"
#include "dollarp.h"

static inline void _translate_to_origin(stroke_t* strk, int n) {
  point2d_t c = { 0, 0 };
  for (int i = 0; i < strk->num; i++) {
    c.x += strk->pts[i].x;
    c.y += strk->pts[i].y;
  }

  c.x /= n;
  c.y /= n;

  for (int i = 0; i < strk->num; i++) {
    strk->pts[i].x -= c.x;
    strk->pts[i].y -= c.y;
  }
}

static inline double _scale(stroke_t* strk) {
  point2d_t min = { DBL_MAX, DBL_MAX };
  point2d_t max = { 0, 0 };

  for (int i = 0; i < strk->num; i++) {
    min.x = MIN(min.x, strk->pts[i].x);
    min.y = MIN(min.y, strk->pts[i].y);
    max.x = MAX(max.x, strk->pts[i].x);
    max.y = MAX(max.y, strk->pts[i].y);
  }

  double scale = MAX(max.x - min.x, max.y - min.y);
  for (int i = 0; i < strk->num; i++) {
    strk->pts[i].x = (strk->pts[i].x - min.x) / scale;
    strk->pts[i].y = (strk->pts[i].y - min.y) / scale;
  }
}

static inline double _path_length(const stroke_t* strk) {
  double len = 0;
  for (int i = 1; i < strk->num; i++) {
    len += point2d_distance(&strk->pts[i].p2d, &strk->pts[i-1].p2d);
  }
  return len;
}

static inline stroke_t* _resample(stroke_t* strk, int n) {
  double I = _path_length(strk) / (n - 1);
  double D = 0;
  stroke_t* r_strk = stroke_create(n);
  stroke_add_coords(r_strk, strk->pts[0].x, strk->pts[0].y);
  for (int i = 1; i < strk->num; i++) {
    point2d_t* curr = &strk->pts[i].p2d;
    point2d_t* prev = &strk->pts[i-1].p2d;
    double d = point2d_distance(curr, prev);
    if (D + d >= D) {
      point2d_t q = {
        prev->x + ((I - D) / d) * (curr->x - prev->x),
        prev->y + ((I - D) / d) * (curr->y - prev->y)
      };
      stroke_add_coords(r_strk, q.x, q.y);
      D = 0;

      // Set up for the next curr/prev pair to be q and i-1.
      memcpy(&strk->pts[i-1], &strk->pts[i], sizeof(point2d_t));
      memcpy(&strk->pts[i], &q, sizeof(point2d_t));
      i--;
    } else {
      D += d;
    }
  }

  return r_strk;
}

static inline stroke_t* _normalize(stroke_t* strk, int n) {
  stroke_t* norm_strk = _resample(strk, n);
  _scale(norm_strk);
  _translate_to_origin(norm_strk, n);
  return norm_strk;
}

static inline int
_cloud_dist(const stroke_t* strk, const stroke_t* tmpl, double start, int n) {
  // Effective array of booleans.
  int* matched = calloc(n, sizeof(char));
  double sum = 0;
  int i = 0;
  int index = -1;
  do {
    double min = DBL_MAX;
    for (int j = 0; j < n; j++) {
      if (matched[j]) {
        continue;
      }

      double d = point2d_distance(&strk->pts[i].p2d, &tmpl->pts[j].p2d);
      if (d < min) {
        min = d;
        index = j;
      }
    }

    matched[index] = 1;
    double weight = 1 - ((int)(floor(i - start + n)) % (int)(n)) / n;
    sum += weight * min;
    i = (i + 1) % n;
  } while (i != start);

  return sum;
}

static inline double
_greedy_cloud_match(const dp_context_t* self, const stroke_t* strk,
                    const stroke_t* tmpl) {
  double step = floor(pow(self->norm_size, 1 - self->epsilon));
  double min = DBL_MAX;

  for (double i = 0; i < self->norm_size; i += step) {
    double d[] = {
      _cloud_dist(strk, tmpl, i, self->norm_size),
      _cloud_dist(tmpl, strk, i, self->norm_size)
    };
    min = MIN(d[0], d[1]);
  }
  return min;
}



////////////////////////////////////////////////////////////////////////////////
//                             "Public" Functions                             //
////////////////////////////////////////////////////////////////////////////////

dp_context_t* create_dollarp(size_t norm_size) {
  dp_context_t* self = calloc(1, sizeof(dp_context_t));
  self->norm_size = norm_size;
  return self;
}

dp_result_t dp_recognize(const dp_context_t* self, stroke_t* strk) {
  // Init for recognition.
  _normalize(strk, self->norm_size);

  // Try each template, see which one works.
  template_t* tmpl = NULL;
  double score = DBL_MAX;
  for (int i = 0; i < self->num_tmpls; i++) {
    double d = _greedy_cloud_match(self, strk, self->tmpls[i].strk);
    if (score > d) {
      score = d;
      tmpl = &self->tmpls[i];
    }
  }

  // Normalize score in [0,1] and return the result.
  dp_result_t result = {
    tmpl,
    MAX((2.0 - score) / 2.0, 0)
  };
  return result;
}

void destroy_dollarp(dp_context_t* self) {
  free(self);
}
