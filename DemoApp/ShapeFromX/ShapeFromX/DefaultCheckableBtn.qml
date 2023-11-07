import QtQuick 2.0
import QtQuick.Controls 2.14

Button {
    id: defaultBtn
    text: qsTr("DefaultCheckableBtn")
    checkable: true
    checked: false
    CheckBox {
        anchors.right: parent.right
        checked: parent.checked
        enabled: false
    }
}
