import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick 2.0

Button {
    id: defaultBtn
    text: qsTr("defaultBtn")
    onCheckedChanged: {
        font.bold = checked
    }
    contentItem: Text {
        text: parent.text
        font: parent.font
        opacity: enabled ? 1.0 : 0.3
        color: "black"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    checkable: true
    background: Rectangle {
        radius: 5
        border.color: parent.checked ? "green" : "transparent"
        border.width: 4
        color: parent.hovered ? "gray" : "darkGray"
    }
}
