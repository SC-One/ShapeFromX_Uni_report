import QtQuick 2.14
import QtQuick.Controls 2.14
import QtMultimedia 5.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import ir.HCoding.SocietyCleaner 1.0 as Manners

MannerBase {
    id: rootItem
    property var imageList: []

    Manners.ChessCalibrator {
        id: calibrator
    }

    Camera {
        id: camera
        captureMode: Camera.CaptureStillImage
        imageCapture {
            onImageCaptured: {
                calibrator.processImage(preview)
                fullScreen.visible = true
            }
        }
    }
    FullImage {
        id: fullScreen
        source: calibrator.lastImageAddress
        visible: false
        width: visible ? parent.width : photoPreview.width
        height: visible ? parent.height : photoPreview.height
    }
    ColumnLayout {
        anchors.fill: parent
        visible: !fullScreen.visible
        VideoOutput {
            id: viewfinder
            Layout.fillWidth: true
            Layout.fillHeight: true
            source: camera
            focus: visible
            Rectangle {
                id: imageContainer
                anchors.left: parent.left
                anchors.top: parent.top
                border.width: 2
                border.color: "red"
                radius: 3
                width: 200
                height: 200
                Image {
                    id: photoPreview
                    source: calibrator.lastImageAddress
                    sourceSize: Qt.size(parent.width, parent.height)
                    width: imageContainer.width - imageContainer.border.width * 2
                    height: imageContainer.height - imageContainer.border.width * 2
                    anchors.centerIn: parent
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            //TODO
                            fullScreen.visible = true
                        }
                    }
                }
            }

            Rectangle {
                width: 150
                height: 77
                color: msBtnArea.isDown ? "lightblue" : "green"
                radius: 10
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    anchors.centerIn: parent
                    text: "Capture"
                    color: "white"
                    font.pointSize: 18
                }

                MouseArea {
                    id: msBtnArea
                    property bool isHovered: false
                    cursorShape: isHovered ? Qt.PointingHandCursor : Qt.ArrowCursor
                    property bool isDown: false
                    anchors.fill: parent
                    onEntered: isHovered = true
                    onExited: isHovered = false
                    hoverEnabled: true
                    onPressed: isDown = true
                    onReleased: isDown = false
                    onClicked: {
                        camera.imageCapture.capture()
                    }
                }
            }
        }
    }

    function showImageFullscreen(index) {// Create a fullscreen popup to display the image
        // Implement the logic to show the image in a popup with close functionality
    }
}
