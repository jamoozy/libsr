import libsrbindings as b

import datetime

UTC_0 = datetime.datetime(1970, 1, 1)

class Stroke(object):
  def __init__(self, _stroke=None):
    self._stroke = _stroke or b.stroke_create(40)

  def add(self, x, y, t=None):
    t = t or int((datetime.datetime.utcnow() - UTC_0).total_seconds())
    b.stroke_add_timed(self._stroke, x, y, t)

  def __iter__(self):
    return StrokeIter(self)

  def save(self, fname):
    b.stroke_save(self._stroke, fname)

  @classmethod
  def load(cls, fname):
    return Stroke(b.stroke_from_file(fname))


class StrokeIter(object):
  def __init__(self, stroke):
    self._stroke = stroke._stroke
    self.i = 0

  def __iter__(self):
    return self

  def next(self):
    if self.i >= self._stroke.num:
      raise StopIteration()
    p = b.stroke_get(self._stroke, self.i)
    self.i += 1
    return p.x, p.y, p.t

if __name__ == '__main__':
  stroke = Stroke()
  stroke.add(1, 2)
  stroke.add(3, 4, 5)
  for p in stroke:
    print p

