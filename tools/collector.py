#!/usr/bin/python

import os
import sys

from PyQt5 import QtCore
from PyQt5 import QtGui
from PyQt5 import QtWidgets
from PyQt5 import uic

# Do a bit extra for importing libsr, just in case the user installed it.
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


class Collector(QtWidgets.QMainWindow):
  '''The top level widget.  All hail the top level widget!'''
  def __init__(self, ui_file):
    super(QtWidgets.QMainWindow, self).__init__()
    uic.loadUi(ui_file, self)

    # Dialogs.
    self.save_dialog = QtWidgets.QFileDialog(self, 'Save to Directory')
    self.save_dialog.setAcceptMode(QtWidgets.QFileDialog.AcceptSave)
    self.save_dialog.setViewMode(QtWidgets.QFileDialog.List)
    self.save_dialog.fileSelected.connect(self.save_dir_selected)

    self.load_dialog = QtWidgets.QFileDialog(self, 'Load from Directory')
    self.load_dialog.setAcceptMode(QtWidgets.QFileDialog.AcceptOpen)
    self.load_dialog.setViewMode(QtWidgets.QFileDialog.List)
    self.load_dialog.fileSelected.connect(self.load_dir_selected)

    # Connect action clicks.
    self.actionSave.triggered.connect(self.save_dialog.show)
    self.actionLoad.triggered.connect(self.load_dialog.show)
    self.actionNew.triggered.connect(self.check_reset_scene)
    self.actionQuit.triggered.connect(self.show_quit_dialog)

    # Generate pen for all the strokes.
    self.stroke_pen = QtGui.QPen(QtCore.Qt.SolidLine)
    self.stroke_pen.setColor(QtCore.Qt.black)
    self.stroke_pen.setWidth(1)

    self.canvases = [Canvas(self.stroke_pen)]
    self.setCentralWidget(self.canvases[0])
    self.setWindowTitle('Collector')
    self.show()

  def dirty(self):
    for canvas in self.canvases:
      if canvas.dirty():
        return True
    return False

  def _clear_strokes(self):
    self.canvases = []

  def get_strokes(self):
    return [s for c in self.canvases for s in c.get_strokes()]

  def set_strokes(self, strokes):
    self._clear_strokes()
    self.canvases = [Canvas(self.stroke_pen)]
    self.canvases[0].set_strokes(strokes)

  def save_dir_selected(self, dname):
    strokes = self.get_strokes()
    for i, stroke in enumerate(strokes):
      fname = os.path.join(dname, "stroke%03d.sr")
      stroke.save(fname)

  def load_dir_selected(self, dname):
    strokes = []
    while os.path.isfile(os.path.join(dname, "stroke%03d.sr")):
      fname = os.path.isfile(os.path.join(dname, "stroke%03d.sr"))
      strokes.append(libsr.Stroke.load(fname))
    self.set_strokes(strokes)

  def check_reset_scene(self, dname):
    if self.dirty():
      # save / discard / cancel dialog
      self.save_dialog.show()
    self._clear_strokes()
    self.canvases = [Canvas(self.stroke_pen)]
    self.canvases[0].set_strokes(strokes)

  def show_quit_dialog(self):
    # Quit?
    sys.exit(0)


class Canvas(QtWidgets.QGraphicsView):
  '''Canvas to collect and display strokes.'''

  def __init__(self, stroke_pen):
    '''Initializes the canvas.'''
    super(Canvas, self).__init__(StrokeScene())
    self.scene().setSceneRect(0, 0, 800, 600)

    self.is_dirty = False
    self.stroke_pen = stroke_pen

    # Generate pen for background drawing.
    self.bg_pen = QtGui.QPen(QtCore.Qt.SolidLine)
    self.bg_pen.setColor(QtCore.Qt.black)
    self.bg_pen.setWidth(2)

  def dirty(self, d=None):
    if d is None:
      return self.is_dirty
    self.is_dirty = bool(d)

  def set_strokes(self, strokes):
    self.is_dirty = False
    for stroke in strokes:
      self.scene().addItem(StrokeGraphiscItem(self.strokes, 0, 0))
      self.scene().items()[0].set_stroke(stroke)

  def resizeEvent(self, e):
    '''See :meth:`QGraphicsView.mouseResizeEvent`.'''
    self.scene().setSceneRect(0, 0, e.size().width(), e.size().height())

  def _append_pos(self, x, y):
    '''Append the position to the most-recently drawn stroke.

    Args:
      x, float: X-position of mouse event.
      y, float: Y-position of mouse event.
    '''
    self.is_dirty = True
    rect = self.scene().items()[0].add(x, y)

  def mousePressEvent(self, e):
    '''See :meth:`QGraphicsView.mousePressEvent`.'''
    self.scene().addItem(StrokeGraphiscItem(self.stroke_pen, e.x(), e.y()))

  def mouseMoveEvent(self, e):
    '''See :meth:`QGraphicsView.mouseMoveEvent`.'''
    self._append_pos(e.x(), e.y())

  def mouseReleaseEvent(self, e):
    '''See :meth:`QGraphicsView.mouseReleaseEvent`.'''
    self._append_pos(e.x(), e.y())

  def drawBackground(self, painter, rect):
    '''Draws a border.'''
    painter.setPen(self.bg_pen)
    painter.drawRect(0, 0, self.scene().width() - 1, self.scene().height() - 1)


class StrokeScene(QtWidgets.QGraphicsScene):
  '''A graphics scene that holds a bunch of strokes.'''
  def __init__(self):
    super(QtWidgets.QGraphicsScene, self).__init__()


class StrokeGraphiscItem(QtWidgets.QGraphicsItem):
  '''Draws an underlying libsr.Stroke object.'''
  DRAW_BUF = 5   # Small buffer to get some ... well ... buffer around rects.

  def __init__(self, pen, x, y):
    '''Creates a new graphics item (and underlying stroke) starting at (x, y).

    Args:
      pen, QtGui.QPen: The pen to use to draw.
      x, float: X-position of mouse event.
      y, float: Y-position of mouse event.
    '''
    super(StrokeGraphiscItem, self).__init__()
    self.stroke = libsr.Stroke()
    self._add_scene_point(x, y)
    self.pen = pen

  def _add_scene_point(self, x, y):
    '''Adds a scene point to the underlying libsr.Stroke object.

    Args:
      x, float: X-coordinate of mouse event.
      y, float: Y-coordinate of mouse event.
    '''
    self.stroke.add(int(x), int(y))
    self.prepareGeometryChange()

  def set_stroke(self, stroke):
    self.stroke = stroke

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


if __name__ == '__main__':
  app = QtWidgets.QApplication(sys.argv)
  dir_name = os.path.join(
      os.path.dirname(os.path.realpath(__file__)), 'collector.ui')
  c = Collector(dir_name)
  sys.exit(app.exec_())
