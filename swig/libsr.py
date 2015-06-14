'''Syntactic sugar for all of libpaleo.  Wraps a Stroke and implements an
iterator for it.'''

import libsrbindings as b

import datetime
import pickle


# Used to get utime -- seconds since the epoc.
UTC_0 = datetime.datetime(1970, 1, 1)

class Stroke(object):
  '''A wrapped stroke_t (from C) object.'''
  def __init__(self, _stroke=None):
    '''Creates a new Stroke.  Will either wrap ``_stroke`` or a newly-created
    stroke_t object.

    Args:
      _stroke, stroke_t: (optional) The stroke to wrap.  Defaults to a new
                         stroke.
    '''
    self._stroke = _stroke or b.stroke_create(40)

  def __len__(self):
    '''Gets the number of points in this stroke.

    Returns:
      The number of points in the stroke.
    '''
    return self._stroke.num

  def __getitem__(self, i):
    '''Gets the ``i``th point from this stroke.

    Args:
      i, int: The index to the stroke to get.
    Returns:
      The ``i``th point wrapped as a `Point`.
    '''
    return Point(b.stroke_get(self._stroke, i))

  def add(self, x, y, t=None):
    '''Adds a point to this stroke.

    Args:
      x, int: X coordinate.
      y, int: Y coordinate.
      t, int: (default: now) The time the point was made.
    '''
    t = t or int((datetime.datetime.utcnow() - UTC_0).total_seconds())
    b.stroke_add_timed(self._stroke, x, y, t)

  def __iter__(self):
    '''Creates a ``StrokeIter`` over this ``Stroke``.

    Returns:
      The iterator.
    '''
    return StrokeIter(self)

  def save(self, fname):
    '''Saves the underlying stroke to file in a libsr-specific way.

    Args:
      fname, str: The file name to save to.
    '''
    b.stroke_save(self._stroke, fname)

  def pickle(self, fname):
    '''Pickles the Stroke.'''
    with open(fname, 'w') as f:
      pickle.dump(self, f)

  @classmethod
  def unpickle(self, fname):
    '''Unpickles the Stroke in ``fname``.

    Returns:
      The ``Stroke`` object in ``fname``.
    '''
    with open(fname, 'r') as f:
      return pickle.load(f)

  @classmethod
  def load(cls, fname):
    '''Loads a stroke stroke in a libsr-specific way and returns it.

    Args:
      fname, str: The file name to save to.

    Returns:
      The stroke.
    '''
    return Stroke(b.stroke_from_file(fname))


class StrokeIter(object):
  '''An iterator over a Stroke object.'''
  def __init__(self, stroke):
    '''Creates a new iterator over a stroke.

    Args:
      stroke, Stroke: The stroke to iterate over.
    '''
    self._stroke = stroke._stroke
    self.i = 0

  def __iter__(self):
    '''Returns itself.

    Returns:
      ``self``
    '''
    return self

  def next(self):
    '''Gets the next point in the underlying stroke as an ``(x, y, t)`` tuple.

    Returns:
      The next point as an ``(x, y, t)`` tuple.

    Raises:
      :exc:`StopIteration` when there are no more points.
    '''
    if self.i >= self._stroke.num:
      raise StopIteration()
    p = b.stroke_get(self._stroke, self.i)
    self.i += 1
    return p.x, p.y, p.t


class Point(object):
  '''Wraps a point_t object (from C).'''
  def __init__(self, _point=None):
    '''Creates a new point_t object wrapped around ``_point`` or a new `point_t`
    object.

    Args:
      _point, point_t: (default: new point_t) The `point_t` to wrap.
    '''
    self._point = _point or b.point_create()

  def __getattr__(self, attr):
    '''Passed attribute access to the underlying `point_t` object.'''
    return getattr(self._point, attr)


# Simple test to ensure everything seems to work.
if __name__ == '__main__':
  stroke = Stroke()
  stroke.add(1, 2)
  stroke.add(3, 4, 5)
  for p in stroke:
    print p

  for i in xrange(len(stroke)):
    print stroke[i]
    print stroke[i]._point.x, stroke[i]._point.y, stroke[i]._point.t, stroke[i]._point.i
    print stroke[i].x, stroke[i].y, stroke[i].t, stroke[i].i
    for attr in ('X', 'Y', 'T', 'I', 'foo', 'bar'):
      try:
        print getattr(stroke[i], attr)
        assert False, 'Did not raise error.'
      except AttributeError:
        pass
