'''Syntactic sugar for all of libpaleo.  Wraps a Stroke and implements an
iterator for it.'''

import bindings as b

import datetime
import pickle
import random
import sys


# Used to get utime -- seconds since the epoc.
UTC_0 = datetime.datetime(1970, 1, 1)

class Stroke(object):
  # Store this here so we can call it when Python starts shutting down.
  stroke_destroy = b.stroke_destroy

  '''A wrapped stroke_t (from C) object.'''
  def __init__(self, _stroke=None):
    '''Creates a new Stroke.  Will either wrap ``_stroke`` or a newly-created
    stroke_t object.

    Args:
      _stroke, stroke_t: (optional) The stroke to wrap.  Defaults to a new
                         stroke.  Takes ownership of the stroke_t object, so it
                         will destroy it in its :meth:`__del__` method.
    '''
    self._stroke = _stroke or b.stroke_create(40)
    self.bbox = (sys.maxint, sys.maxint, -sys.maxint, -sys.maxint)

  def __del__(self):
    '''Destroys the underlying stroke_t object.'''
    self.stroke_destroy(self._stroke)

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
    if isinstance(i, slice):
      return (Point(b.stroke_get(self._stroke, j))
              for j in xrange(i.start or 0,
                              i.stop or self._stroke.num,
                              i.step or 1))
    if i < 0:
      i += self._stroke.num
    if i < 0 or self._stroke.num <= i:
      raise IndexError('%d \\nin [0,%d)' % (i, self._stroke.num))
    return Point(b.stroke_get(self._stroke, i))

  def add(self, x, y, t=None):
    '''Adds a point to this stroke.

    Args:
      x, int: X coordinate.
      y, int: Y coordinate.
      t, int: (default: now) The time the point was made (in micro-s).
    '''
    t = t or int((datetime.datetime.utcnow() - UTC_0).total_seconds() * 1e6)
    self.bbox = (
      min(self.bbox[0], x),
      min(self.bbox[1], y),
      max(self.bbox[2], x),
      max(self.bbox[3], y),
    )
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
  def load(cls, fname):
    '''Loads a stroke stroke in a libsr-specific way and returns it.

    Args:
      fname, str: The file name to save to.

    Returns:
      The stroke.
    '''
    return Stroke(b.stroke_from_file(fname))

  @classmethod
  def unpickle(self, fname):
    '''Unpickles the Stroke in ``fname``.

    Returns:
      The ``Stroke`` object in ``fname``.
    '''
    with open(fname, 'r') as f:
      return pickle.load(f)


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
    return Point(p)


class Point(object):
  # Store this here so we can call it when Python starts shutting down.
  point_destroy = b.point_destroy

  '''Wraps a point_t object (from C).'''
  def __init__(self, _point=None):
    '''Creates a new point_t object wrapped around ``_point`` or a new `point_t`
    object.

    Args:
      _point, point_t: (default: new point_t) The `point_t` to wrap.
    '''
    if _point:
      self.created_point = False
      self._point = _point
    else:
      self.created_point = True
      self._point = b.point_create()

  def __del__(self):
    '''Destroys the underlying point_t object.'''
    if self.created_point:
      self.point_destroy(self._point)

  def __repr__(self):
    return '#%d: (%d,%d) @%d' % (self.i, self.x, self.y, self.t)

  def __str__(self):
    return 'Point(%d, %d, %d, %d)' % (self.x, self.y, self.t, self.i)

  def pos(self):
    '''Returns the ``(x, y)`` tuple of this point.

    Returns:
      The ``(x, y)`` coordinates of this point.
    '''
    return self._point.x, self._point.y

  def __getattr__(self, attr):
    '''Passed attribute access to the underlying `point_t` object.'''
    return getattr(self._point, attr)


# Simple test to ensure everything seems to work.
if __name__ == '__main__':
  stroke = Stroke()
  for i in xrange(10, 110, 10):
    stroke.add(i, i, i)
  for i in xrange(10, 110, 10):
    stroke.add(i, i)

  for p in stroke:
    print p

  for i in xrange(len(stroke)):
    pt = stroke[i]
    print pt
    print pt._point.x, pt._point.y, pt._point.t, pt._point.i
    print pt.x, pt.y, pt.t, pt.i
    for attr in ('X', 'Y', 'T', 'I', 'foo', 'bar'):
      try:
        print getattr(stroke[i], attr)
        assert False, 'Did not raise error.'
      except AttributeError:
        pass


# Debugging help

def _make_random_stroke(num_pts=20, max_int=100):
  stroke = Stroke()
  for x, y in ((random.randrange(max_int), random.randrange(max_int))
               for i in xrange(num_pts)):
    stroke.add(x, y)
  return stroke
