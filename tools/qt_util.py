from PyQt5 import QtWidgets


class SaveDestroyCancelBox(QtWidgets.QMessageBox):
  def __init__(self, parent, message, cb):
    super(QtWidgets.QMessageBox, self).__init__(parent)
    self.message = message
    self.setText(message)
    self.b_save = self.addButton("Save", QtWidgets.QMessageBox.AcceptRole)
    self.b_destroy = self.addButton(
        "Don't Save", QtWidgets.QMessageBox.DestructiveRole)
    self.b_cancel = self.addButton("Cancel", QtWidgets.QMessageBox.RejectRole)
    self.cb = cb
    self.buttonClicked.connect(self._cb)

  def _cb(self, button):
    self.cb(self, button)

def popup(parent, message):
  '''Pops up a small dialog with a message.

  Params:
    parent, QWidget: The parent of the popup.
    message, str: The message to display.

  Returns, QButton:
    The "Okay" button of the popup.
  '''
  popup = QtWidgets.QMessageBox(parent)
  popup.setText(message)
  button = popup.addButton("Okay", QtWidgets.QMessageBox.AcceptRole)
  popup.show()
  return button
