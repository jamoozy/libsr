from PyQt5 import QtWidgets


##
# Convenience class that creates a QMessageBox with Save, Don't Save, Cancel
# options.
class SaveDestroyCancelBox(QtWidgets.QMessageBox):
  ##
  # \var message
  # (str) The message to print.
  #
  # \var b_save
  # (QButton) The save button.
  #
  # \var b_destroy
  # (QButton) The destroy button.
  #
  # \var b_cancel
  # (QButton) The cancel button.
  #
  # \var cb
  # (method) Callback function.  Will be passed `self` and the pushed button.

  ##
  # Creates a new QMessageBox with a Save, Don't Save, and Cancel button.
  #
  # \param self The same thing `self` always represents.
  # \param parent (QWidget) The parent widget.
  # \param message (str) The message to display.
  # \param cb (callable) The callback to call on a button click.
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

  ##
  # Internal callback delegation.
  #
  # \param button (QButton) The button.
  def _cb(self, button):
    self.cb(self, button)

##
# Pops up a Yes/No question dialog.
#
# \param parent (QWidget) The dialog's parent.
# \param message (str) The message to display.
#
# \return `True` if "Yes" was clicked, `False` otherwise.
def yn_popup(parent, message):
  popup = QtWidgets.QMessageBox(parent)
  popup.setText(message)
  popup.addButton("Yes", QtWidgets.QMessageBox.YesRole)
  popup.addButton("No", QtWidgets.QMessageBox.NoRole)
  return QtWidgets.QMessageBox.YesRole == popup.exec_()

##
# Pops up a small dialog with a message.
#
# \param parent (QWidget) The parent of the popup.
# \param message (str) The message to display.
#
# \return (QButton) The "Okay" button of the popup.
def popup(parent, message):
  popup = QtWidgets.QMessageBox(parent)
  popup.setText(message)
  button = popup.addButton("Okay", QtWidgets.QMessageBox.AcceptRole)
  popup.show()
  return button
