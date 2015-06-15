#!/usr/bin/python

import sys
import random

from pprint import pprint
from PyQt4 import QtCore
from PyQt4 import QtGui

import libsr


#class Canvas(QtGui.QGraphicsView):
#  mouse_down = False
#
#  '''Canvas to collect and display strokes.'''
#  def __init__(self):
#    super(Canvas, self).__init__(QtGui.QGraphicsScene())
##    self.sgis = []
#    self.setWindowTitle('Canvas')
#    self.show()
#
##  def paintEvent(self, *args, **kwargs):
##    print 'args:', args
##    print 'kwargs:', kwargs
##    return
##    # Draw the strokes.
##    painter = QtGui.QPainter()
##    for sgi in self.sgis:
##      sgi.paint(painter, None, None)
#
#  def _append_pos(self, x, y):
#    assert self.mouse_down, 'error: mouse not down'
##    rect = self.sgis[-1].add(x, y)
#    rect = self.scene().items()[0].add(x, y)
#    self.invalidateScene(rect)
#    print 'H:', self.isHidden(), 'V:', self.isVisible()
#
#  def mousePressEvent(self, e):
##    self.sgis.append(StrokeGraphiscItem(e.x(), e.y()))
##    self.scene().addItem(self.sgis[-1])
#    self.scene().addItem(StrokeGraphiscItem(e.x(), e.y()))
#    self.mouse_down = True
#
#  def mouseMoveEvent(self, e):
#    self._append_pos(e.x(), e.y())
#
#  def mouseReleaseEvent(self, e):
#    self._append_pos(e.x(), e.y())
##    self.sgis[-1].path().closeSubpath()
#    self.scene().items()[0].path().closeSubpath()
#    self.mouse_down = False
#
#
#class StrokeGraphiscItem(QtGui.QGraphicsPathItem):
#  DRAW_BUF = 5   # Small buffer to get some ... well ... buffer around rects.
#
#  def __init__(self, x, y):
#    super(StrokeGraphiscItem, self).__init__()
#    self.path().moveTo(x, y)
#    self.stroke = libsr.Stroke()
#    self.stroke.add(x, y)
#
#  def add(self, x, y):
#    self.path().lineTo(x, y)
#    self.stroke.add(x, y)
#    x, y, w, h = (min(self.stroke[-1].x, self.stroke[-2].x) - self.DRAW_BUF,
#                  min(self.stroke[-1].y, self.stroke[-2].y) - self.DRAW_BUF,
#                  max(1, abs(self.stroke[-1].x - self.stroke[-2].x)) +
#                    2 * self.DRAW_BUF,
#                  max(1, abs(self.stroke[-1].y - self.stroke[-2].y)) +
#                    2 * self.DRAW_BUF)
#    return QtCore.QRectF(x, y, w, h)
#
#  def __iter__(self):
#    return iter(self.stroke)
#
#  def __len__(self):
#    return len(self.stroke)
#
#  def paint(self, painter, option, widget):
#    print 'paint', painter, option, widget
#    painter.drawPath(self.path())


def main(*args):
  app = QtGui.QApplication(list(args))

  scene = QtGui.QGraphicsScene()
  scene.setBackgroundBrush(QtCore.Qt.white)
  pp = QtGui.QPainterPath()
  for n in xrange(5):
    coords = zip((random.randrange(100) for i in xrange(20)),
                 (random.randrange(100) for i in xrange(20)))
    pp.moveTo(*coords[0])
    for x, y in coords[:1]:
      pp.lineTo(x, y)

  pen = QtGui.QPen(QtCore.Qt.SolidLine)
  pen.setColor(QtCore.Qt.red)
  scene.addPath(pp, pen)

  view = QtGui.QGraphicsView(scene)
  view.show()

  sys.exit(app.exec_())


if __name__ == '__main__':
  main(*sys.argv)

