#!/usr/bin/python

import sys
import random

from pprint import pprint
from PyQt4 import QtCore
from PyQt4 import QtGui

try:
  import libsr
except ImportError:
  add_path = "/usr/local/lib/python2.7"
  sys.path.append(add_path)
  try:
    import libsr
    print 'Warning, had to add "%s" to path for libsr.' % add_path
  except ImportError:
    print 'Could not import libsr!  Did you run make install?'
    sys.exit(-1)


class Canvas(QtGui.QGraphicsView):
  '''Canvas to collect and display strokes.'''

  mouse_down = False

  def __init__(self):
    super(Canvas, self).__init__(QtGui.QGraphicsScene())
    self.setBackgroundBrush(QtCore.Qt.white)
    self.setWindowTitle('Canvas')
    self.show()

  def _append_pos(self, x, y):
    assert self.mouse_down, 'error: mouse not down'
    rect = self.scene().items()[0].add(x, y)

  def mousePressEvent(self, e):
    self.mouse_down = True
    self.scene().addItem(StrokeGraphiscItem(e.x(), e.y()))

  def mouseMoveEvent(self, e):
    self._append_pos(e.x(), e.y())

  def mouseReleaseEvent(self, e):
    self._append_pos(e.x(), e.y())
    self.mouse_down = False


class StrokeGraphiscItem(QtGui.QGraphicsItem):
  DRAW_BUF = 5   # Small buffer to get some ... well ... buffer around rects.

  def __init__(self, x, y):
    super(StrokeGraphiscItem, self).__init__()
    self.stroke = libsr.Stroke()
    self._add_scene_point(x, y)

  def _add_scene_point(self, x, y):
    self.stroke.add(int(x), int(y))
    self.prepareGeometryChange()

  def add(self, x, y):
    self._add_scene_point(x, y)

  def boundingRect(self):
    return QtCore.QRectF(*self.stroke.bbox)

  def paint(self, painter, option, widget):
    pen = QtGui.QPen(QtCore.Qt.SolidLine)
    pen.setColor(QtCore.Qt.black)
    pen.setWidth(5)
    painter.setPen(pen)
    pp = QtGui.QPainterPath()
    pp.moveTo(self.stroke[0].x, self.stroke[0].y)
    for pt in self.stroke[1:]:
      pp.lineTo(pt.x, pt.y)
    painter.drawPath(pp)
    pen.setColor(QtCore.Qt.black)
    painter.setPen(pen)


def main(*args):
  app = QtGui.QApplication(list(args))
  c = Canvas()
  sys.exit(app.exec_())


if __name__ == '__main__':
  main(*sys.argv)

