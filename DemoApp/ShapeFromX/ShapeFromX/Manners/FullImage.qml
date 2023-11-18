import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14

Item {
    id: rootItem
    property alias source: image.source

    Rectangle {
        anchors.fill: parent
        color: "#000000"
        opacity: 0.5
        visible: rootItem.visible
    }

    Image {
        id: image
        anchors.centerIn: parent
        width: parent.width * 0.8
        height: parent.height * 0.8
        sourceSize.width: parent.width * 0.8
        sourceSize.height: parent.height * 0.8
        visible: rootItem.visible
    }

    MouseArea {
        anchors.fill: parent
        onClicked: rootItem.visible = false
        enabled: rootItem.visible
    }

    Behavior on width {
        NumberAnimation {
            duration: 1000
        }
    }
    Behavior on height {
        NumberAnimation {
            duration: 1000
        }
    }
}
