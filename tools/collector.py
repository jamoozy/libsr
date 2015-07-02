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

from qt_util import SaveDestroyCancelBox, popup


class Collector(QtWidgets.QMainWindow):
  '''The top level widget.  All hail the top level widget!'''
  def __init__(self, ui_file):
    super(QtWidgets.QMainWindow, self).__init__()
    uic.loadUi(ui_file, self)

    ##########################
    # Message & File Dialogs #
    ##########################

    self.save_file_dialog = QtWidgets.QFileDialog(self, 'Save to Directory')
    self.save_file_dialog.setAcceptMode(QtWidgets.QFileDialog.AcceptSave)
    self.save_file_dialog.setFileMode(QtWidgets.QFileDialog.AnyFile)
    self.save_file_dialog.setViewMode(QtWidgets.QFileDialog.List)
    self.save_file_dialog.fileSelected.connect(self.save_dir_selected)

    self.load_file_dialog = QtWidgets.QFileDialog(self, 'Load from Directory')
    self.load_file_dialog.setAcceptMode(QtWidgets.QFileDialog.AcceptOpen)
    self.load_file_dialog.setFileMode(QtWidgets.QFileDialog.Directory)
    self.load_file_dialog.setViewMode(QtWidgets.QFileDialog.List)
    self.load_file_dialog.fileSelected.connect(self.load_dir_selected)

    self.new_dialog = SaveDestroyCancelBox(
        self, "Save before creating new?", self._handle_new_dialog_click)

    self.quit_dialog = SaveDestroyCancelBox(
        self, "Save before exiting?", self._handle_quit_dialog_click)

    #############
    # File Menu #
    #############

    def save():
      if self.dirty():
        self.save_file_dialog.show()
    self.actionSave.triggered.connect(save)

    def load():
      if self.dirty():
        self.save_file_dialog.show()
      else:
        self.load_file_dialog.show()
    self.actionLoad.triggered.connect(load)

    def new():
      if self.dirty():
        self.new_dialog.show()
      else:
        self._clear_strokes()
    self.actionNew.triggered.connect(new)

    def quit():
      if self.dirty():
        self.quit_dialog.show()
      else:
        sys.exit(0)
    self.actionQuit.triggered.connect(quit)

    ########
    # Pens #
    ########

    self.stroke_pen = QtGui.QPen(QtCore.Qt.SolidLine)
    self.stroke_pen.setColor(QtCore.Qt.black)
    self.stroke_pen.setWidth(1)

    self.canvases = []
    self._add_canvas()
    self.setCentralWidget(self.canvases[0])
    self.setWindowTitle('Collector')
    self.show()

  def _add_canvas(self):
    '''Adds a canvas to this tool.'''
    self.canvases.append(Canvas(self.stroke_pen))

  def dirty(self):
    '''Determines whether any of the canvases in this Collector are dirty.
    Dirty canvases are those with unsaved changes.

    Returns, bool:
      Whether any canvas is dirty.
    '''
    for canvas in self.canvases:
      if canvas.dirty():
        return True
    return False

  def _clear_strokes(self):
    '''Clears all strokes in all canvases without doing any checks.'''
    for c in self.canvases:
      c.clear()

  def get_strokes(self):
    '''Gets all the strokes in all the canvases in this collector.

    Returns, list:
      The list of strokes.
    '''
    return [s for c in self.canvases for s in c.get_strokes()]

  def set_strokes(self, strokes):
    '''Clears all the canvases and sets the strokes as the contents of a newly
    created only canvas.

    Params:
      strokes, list: The list of strokes to set as the contents of a new, sole
      canvas.
    '''
    self._clear_strokes()
    self._add_canvas()
    self.canvases[0].set_strokes(strokes)

  @classmethod
  def _make_fname(cls, dname, i, j):
    '''What's the 

    Params:
      dname, str: Directory name.
      i, int: Index of canvas.
      j, int: Index of stroke within canvas.

    Returns, str:
      The constructed name of the directory.
    '''
    return os.path.join(dname, "stroke-%02d-%03d.sr" % (i, j))

  def save_dir_selected(self, dname):
    '''Saves all the strokes into separate files in the passed directory.

    Params;
      dname, str: The name of the directory to save to.
    '''
    if not os.path.exists(dname):
      print 'making dirs:', dname
      os.makedirs(dname)
    elif os.path.isfile(dname):
      popup("File exists!  Choose another location.")
      self.save_file_dialog.show()
      return

    for i, canvas in enumerate(self.canvases):
      for j, stroke in enumerate(canvas.get_strokes()):
        fname = self._make_fname(dname, i, j)
        print 'saving to:', fname
        stroke.save(fname)
      canvas.is_dirty = False

  def load_dir_selected(self, dname):
    '''Loads the strokes in the directory into

    Params:
      dname, str: The name of the directory to load from.
    '''
    if not os.path.exists(dname):
      popup(self, "No such directory!")
      return

    if not os.path.isdir(dname):
      popup(self, "Please select a directory, not a file!")
      return

    dname, dirs, files = os.walk(dname).next()
    if len(files) <= 0:
      popup(self, "Directory is empty!")
      return

    fdata = [(os.path.join(dname, fname), int(e[1]), int(e[2][:3]))
             for e in [fname.split('-') for fname in files]
             if len(e) == 3]

    if len(fdata) <= 0:
      popup(self, "Directory has no strokes!")
      return

    self._clear_strokes()
    for fname, i, j in fdata:
      # Ensure we have the canvas with this index.
      while len(self.canvases) <= i:
        self._add_canvas()
      self.canvases[i].add(libsr.Stroke.load(fname))

  def check_reset_scene(self, dname):
    '''Resets the collector with strokes from the directory, but does a check
    that this isn't dirty, first.

    Params:
      dname, str: The name of the directory to read stroke files from.
    '''
    if self.dirty():
      # save / discard / cancel dialog
      self.save_file_dialog.show()
    self._clear_strokes()
    self._add_canvas()
    self.canvases[0].set_strokes(strokes)

  def _handle_new_dialog_click(self, dialog, button):
    '''Handles the results of the new dialog.

    Params:
      button, QButton: The button clicked.

    Raises:
      Exception: if there's an unrecognized button.
    '''
    if button == dialog.b_save:
      def reset():
        self.save_file_dialog.fileSelected.disconnect(reset)
        self._clear_strokes()
      self.save_file_dialog.fileSelected.connect(reset)
      self.save_file_dialog.show()
    elif button == dialog.b_destroy:
      self._clear_strokes()
    elif button != dialog.b_cancel:
      raise Exception('Unrecognized button:' + button.text())

  def _handle_quit_dialog_click(self, dialog, button):
    '''Handles the results of the quit dialog.

    Params:
      dialog, SaveDestroyCancelBox: The dialog.
      button, QButton: The button clicked.

    Raises:
      Exception: if there's an unrecognized button.
    '''
    if button == dialog.b_save:
      self.save_file_dialog.fileSelected.connect(lambda: sys.exit(0))
      self.save_file_dialog.show()
    elif button == dialog.b_destroy:
      sys.exit(0)
    elif button != dialog.b_cancel:
      raise Exception('Unrecognized button:' + button.text())


class Canvas(QtWidgets.QGraphicsView):
  '''Canvas to collect and display strokes.'''

  def __init__(self, stroke_pen, rect=(0, 0, 800, 600)):
    '''Initializes the canvas.'''
    super(Canvas, self).__init__(StrokeScene())
    self.scene().setSceneRect(*rect)

    self.is_dirty = False
    self.stroke_pen = stroke_pen

    # Generate pen for background drawing.
    self.bg_pen = QtGui.QPen(QtCore.Qt.SolidLine)
    self.bg_pen.setColor(QtCore.Qt.black)
    self.bg_pen.setWidth(2)

  def add(self, stroke):
    '''Adds a stroke to this canvas.

    Params:
      stroke, libsr.Stroke: The stroke to add.
    '''
    self.scene().addItem(StrokeGraphiscItem(self.stroke_pen, 0, 0))
    self.scene().items()[0].set_stroke(stroke)

  def clear(self):
    '''Clears all the strokes in this canvas.'''
    for sgi in self.scene().items()[:]:
      self.scene().removeItem(sgi)

  def dirty(self):
    '''Sets or returns the current `dirty` state.

    Returns:
      The dirty state.
    '''
    return self.is_dirty

  def get_strokes(self):
    '''Gets all the strokes in this canvas.

    Returns, list:
      The list of strokes.
    '''
    return [sgi.stroke for sgi in self.scene().items()
                       if isinstance(sgi, StrokeGraphiscItem)]

  def set_strokes(self, strokes):
    '''Sets the strokes in this canvas to `strokes`.

    Params:
      strokes, list(libsr.Stroke): The strokes to set this to.
    '''
    self.is_dirty = False
    for stroke in strokes:
      self.add(stroke)

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


if __name__ == '__main__':
  app = QtWidgets.QApplication(sys.argv)
  dir_name = os.path.join(
      os.path.dirname(os.path.realpath(__file__)), 'collector.ui')
  c = Collector(dir_name)
  sys.exit(app.exec_())
