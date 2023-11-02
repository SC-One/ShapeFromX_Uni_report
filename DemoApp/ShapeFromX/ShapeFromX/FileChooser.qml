import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.14

Item {
    property string currentPath: filePathLineEdit.text
    FileDialog {
        id: fileDialog
        selectMultiple: false
        selectFolder: false

        onAccepted: {
            filePathLineEdit.text = fileDialog.fileUrls.length > 0 ? fileDialog.fileUrls[0] : ""
        }

        onRejected: {
            filePathLineEdit.text = ""
        }
    }
    RowLayout {
        anchors.fill: parent
        spacing: 10

        Label {
            text: "Selected File:"
            font.bold: true
            width: 80
        }

        TextField {
            id: filePathLineEdit
            Layout.fillWidth: true
            placeholderText: "No file selected"
            readOnly: true
        }

        Button {
            id: control
            text: qsTr("Choose File")
            width: 120
            onClicked: {
                fileDialog.open()
            }
        }
    }
}
