#!/usr/bin/python

import sys

from PyQt4 import QtCore
from PyQt4 import QtGui

import libsr


class Canvas(QtGui.QGraphicsView):
  '''Canvas to collect and display strokes.'''
  def __init__(self):
    super(Canvas, self).__init__()
    self.scene = QtGui.QGraphicsScene()

    self.setGeometry(300, 300, 280, 170)
    self.setWindowTitle('Points')
    self.show()

  def _mark_last_seg_dirty(self):
    stroke = self.strokes[-1]
    # Getting min/abs so that we get lowest x/y and positive w/h.
    #    (TODO is this necessary?)
    r = QtGui.QRegion(min(stroke[-1].x, stroke[-1].x),
                      min(stroke[-1].y, stroke[-1].y),
                      abs(stroke[-1].x - stroke[-2].x),
                      abs(stroke[-1].y - stroke[-2].y))

  def mousePressEvent(self, e):
    stroke = libsr.Stroke()
    stroke.add(e.x(), e.y())
    stroke_graphics_item = StrokeGraphicsItem(stroke)
    self.scene.addItem(stroke_graphics_item)

  def paint(self, qp, opt, w):
    self.draw_strokes(qp)

    # Draw the strokes.
    for stroke in self.strokes:
      path = QtGui.QPainterPath()
      pts = list(stroke)
      path.moveTo(*pts[0][:2])
      for pt in pts[:1]:
        path.lineTo(*pt[:2])
      qp.drawPath(path)


class StrokeGraphicsItem(QtGui.QGraphicsItem):
  def __init__(self, stroke):
    super(StrokeGraphicsItem, self).__init__()
    self.mouse_down = True
    self.stroke = stroke

  def boundingRect(self):
    rect = [0, 0, 0, 0]
    for pt in self.stroke:
      rect[0] = min(rect[0], pt[0])
      rect[1] = min(rect[1], pt[1])
      rect[2] = max(rect[2], pt[0])
      rect[3] = max(rect[3], pt[1])
    rect[2] -= rect[0]
    rect[3] -= rect[1]
    return QtCore.QRectF(*rect)

  def _append_pos(self, x, y):
    assert self.mouse_down, 'error: mouse not down'
    self.stroke.add(x, y)

  def mousePressEvent(self, e):
    pass

  def mouseMoveEvent(self, e):
    self._append_pos(e.x(), e.y())
    self._mark_last_seg_dirty()

  def mouseReleaseEvent(self, e):
    self._append_pos(e.x(), e.y())
    self._mark_last_seg_dirty()
    self.mouse_down = False


def main():
  app = QtGui.QApplication(sys.argv)
  c = Canvas()
  sys.exit(app.exec_())


if __name__ == '__main__':
  main()
