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
