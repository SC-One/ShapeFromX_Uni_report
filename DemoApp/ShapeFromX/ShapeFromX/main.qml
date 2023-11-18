import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Shape From X")
    ShapeFromX {
        anchors.margins: 7
        anchors.fill: parent
        Component.onCompleted: {
            addNewUI("Shape from Shading", "qrc:/Manners/ShapeFromShading.qml")
            addNewUI("Monocular NN", "qrc:/Manners/ShapeFromMCNN.qml")
        }
    }
}
