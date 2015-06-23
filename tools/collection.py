#!/usr/bin/python

from pprint import pprint
import random
import sys

from PyQt5 import QtCore
from PyQt5 import QtGui
from PyQt5 import QtWidgets

try:
  import libsr
except ImportError:
  add_path = "/usr/local/lib/python2.7"
  sys.path.append(add_path)
  try:
    import libsr
    print 'Warning, had to add "%s" to path for libsr.' % add_path
  except ImportError as e:
    print 'Could not import libsr!'
    print 'Error message:', e.message
    print 'Did you run make install?'
    sys.exit(-1)


class Canvas(QtWidgets.QGraphicsView):
  '''Canvas to collect and display strokes.'''

  def __init__(self):
    '''Initializes the canvas.'''
    super(Canvas, self).__init__(QtWidgets.QGraphicsScene())
    self.scene().setSceneRect(0, 0, 800, 600)
    self.setBackgroundBrush(QtCore.Qt.white)
    self.setWindowTitle('Canvas')
    self.show()

  def resizeEvent(self, e):
    '''See :meth:`QGraphicsView.mouseResizeEvent`.'''
    self.scene().setSceneRect(0, 0, e.size().width(), e.size().height())

  def _append_pos(self, x, y):
    '''Append the position to the most-recently drawn stroke.

    Args:
      x, float: X-position of mouse event.
      y, float: Y-position of mouse event.
    '''
    rect = self.scene().items()[0].add(x, y)

  def mousePressEvent(self, e):
    '''See :meth:`QGraphicsView.mousePressEvent`.'''
    self.scene().addItem(StrokeGraphiscItem(e.x(), e.y()))

  def mouseMoveEvent(self, e):
    '''See :meth:`QGraphicsView.mouseMoveEvent`.'''
    self._append_pos(e.x(), e.y())

  def mouseReleaseEvent(self, e):
    '''See :meth:`QGraphicsView.mouseReleaseEvent`.'''
    self._append_pos(e.x(), e.y())


class StrokeGraphiscItem(QtWidgets.QGraphicsItem):
  '''Draws an underlying libsr.Stroke object.'''
  DRAW_BUF = 5   # Small buffer to get some ... well ... buffer around rects.

  def __init__(self, x, y):
    '''Creates a new graphics item (and underlying stroke) starting at (x, y).

    Args:
      x, float: X-position of mouse event.
      y, float: Y-position of mouse event.
    '''
    super(StrokeGraphiscItem, self).__init__()
    self.stroke = libsr.Stroke()
    self._add_scene_point(x, y)
    self.pen = QtGui.QPen(QtCore.Qt.SolidLine)
    self.pen.setColor(QtCore.Qt.black)
    self.pen.setWidth(1)

  def _add_scene_point(self, x, y):
    '''Adds a scene point to the underlying libsr.Stroke object.

    Args:
      x, float: X-coordinate of mouse event.
      y, float: Y-coordinate of mouse event.
    '''
    self.stroke.add(int(x), int(y))
    self.prepareGeometryChange()

  def add(self, x, y):
    '''Adds a new point to the underlying strokes.  Also registers that this has
    added a new point, and thus needs to be redrawn.

    Args:
      x, float: X-coordinate of mouse event.
      y, float: Y-coordinate of mouse event.
    '''
    pt = self.mapToScene(x, y)
    self._add_scene_point(int(pt.x()), int(pt.y()))

  def boundingRect(self):
    '''The rectangle bounding all the points in the underlying stroke.

    Returns:
      The rectangle containing the underlying stroke.
    '''
    return QtCore.QRectF(*self.stroke.bbox)

  def paint(self, painter, option, widget):
    '''See QGraphicsItem.paint().'''
    painter.setPen(self.pen)
    painter.setRenderHint(QtGui.QPainter.Antialiasing)
    pp = QtGui.QPainterPath()
    pp.moveTo(self.stroke[0].x, self.stroke[0].y)
    for pt in self.stroke[1:]:
      pp.lineTo(pt.x, pt.y)
    painter.drawPath(pp)


def main(*args):
  '''Proverbial "main()" method!'''
  app = QtWidgets.QApplication(list(args))
  c = Canvas()
  sys.exit(app.exec_())


if __name__ == '__main__':
  main(*sys.argv)
