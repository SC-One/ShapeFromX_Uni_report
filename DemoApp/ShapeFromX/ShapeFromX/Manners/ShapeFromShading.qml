import QtQuick 2.15
import ir.HCoding.SocietyCleaner 1.0 as Manners
import QtDataVisualization 1.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import "../" as ParentDir

MannerBase {
    id: rootItem
    function recalculate() {
        myAlgo.reCalculate()
    }

    Manners.ShapeFromShading {
        id: myAlgo
        currentImageURL: rootItem.sourceImagePath ? rootItem.sourceImagePath : ""
        onOutputCalculated: {
            heightSeries.pModel.heightMapFile = ""
            heightSeries.pModel.heightMapFile = myAlgo.fullOutNormalFileName()

            reverseHeightSeries.pModel.heightMapFile = ""
            reverseHeightSeries.pModel.heightMapFile = myAlgo.fullOutInverseFileName()
        }
    }
    Item {
        id: prvObject

        ColorGradient {
            id: surfaceGradient

            ColorGradientStop {
                position: 0.0
                color: "darkslategray"
            }
            ColorGradientStop {
                id: middleGradient
                position: 0.25
                color: "peru"
            }
            ColorGradientStop {
                position: 1.0
                color: "red"
            }
        }
    }

    ButtonGroup {
        id: optionsGroup1
        buttons: gp1.children
    }

    ColumnLayout {
        anchors.fill: parent

        Surface3D {
            id: surfacePlot
            Layout.fillHeight: true
            Layout.fillWidth: true
            flipHorizontalGrid: flipHorizontal.checked

            theme: Theme3D {
                type: Theme3D.ThemeQt
                font.family: "STCaiyun"
                font.pointSize: 35
                colorStyle: Theme3D.ColorStyleRangeGradient
                baseGradients: [surfaceGradient]
            }
            shadowQuality: AbstractGraph3D.ShadowQualityMedium
            selectionMode: AbstractGraph3D.SelectionSlice | AbstractGraph3D.SelectionItemAndRow
            scene.activeCamera.cameraPreset: Camera3D.CameraPresetIsometricLeft
            axisY.min: 0
            axisY.max: 500
            axisX.segmentCount: 10
            axisX.subSegmentCount: 2
            axisX.labelFormat: "%i"
            axisZ.segmentCount: 10
            axisZ.subSegmentCount: 2
            axisZ.labelFormat: "%i"
            axisY.segmentCount: 5
            axisY.subSegmentCount: 2
            axisY.labelFormat: "%i"
            axisY.title: "Shape shading"
            axisX.title: "Image - X"
            axisZ.title: "Image - Y"

            DefaultSurfaceSeriesShading {
                id: heightSeries
                targetVisibleBtn: normalBtn

                targetFlatBtn: flatBtn
                targetGridBtn: gridBtn
                targetImageTextureBtn: textureLoading
                imagePath: rootItem.sourceImagePath
            }

            DefaultSurfaceSeriesShading {
                id: reverseHeightSeries
                targetVisibleBtn: reverseBtn

                targetFlatBtn: flatBtn
                targetGridBtn: gridBtn
                targetImageTextureBtn: textureLoading
                imagePath: rootItem.sourceImagePath
            }

            Surface3DSeries {
                visible: proxyDataBtn.checked

                property var targetFlatBtn: flatBtn
                property var targetGridBtn: gridBtn
                property var targetImageTextureBtn: textureLoading
                ItemModelSurfaceDataProxy {
                    itemModel: myAlgo.myProxyData
                    rowRole: "pointRoleX"
                    columnRole: "pointRoleY"
                    yPosRole: "valueRole"
                }
                flatShadingEnabled: targetFlatBtn.checked
                drawMode: targetGridBtn.checked ? Surface3DSeries.DrawSurfaceAndWireframe : Surface3DSeries.DrawSurface

                function takingAbsolutePath(stringPathFileSchema) {
                    var qurl = Qt.resolvedUrl(stringPathFileSchema)
                    return qurl.toLocalFile()
                }

                property var defaultTexture: rootItem.sourceImagePath ? rootItem.sourceImagePath.toString(
                                                                            ).substring(
                                                                            8) : ""
                textureFile: targetImageTextureBtn.checked ? defaultTexture : ""
            }
        }
    }
    RowLayout {
        id: optBtns
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        Item {
            Layout.fillWidth: true
        }
        RowLayout {
            id: gp1
            Layout.fillHeight: true
            Layout.fillWidth: true
            Button {
                id: normalBtn
                text: qsTr("Normal")
                checkable: true
                Layout.fillHeight: true
                Layout.fillWidth: true
                checked: true
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
                background: Rectangle {
                    radius: 7
                    border.color: parent.checked ? "green" : "transparent"
                    border.width: 4
                    color: parent.hovered ? "gray" : "darkGray"
                }
            }
            Button {
                id: reverseBtn
                text: qsTr("Reverse")
                Layout.fillHeight: true
                Layout.fillWidth: true
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

            Button {
                id: proxyDataBtn
                text: qsTr("ProxyData")
                Layout.fillHeight: true
                Layout.fillWidth: true
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
        }
        Button {
            id: flatBtn
            text: qsTr("Flat surface")
            Layout.fillHeight: true
            Layout.fillWidth: true
            checkable: true
            CheckBox {
                anchors.right: parent.right
                checked: parent.checked
                enabled: false
            }
            checked: true
        }
        Button {
            id: gridBtn
            text: qsTr("Grid surface")
            Layout.fillHeight: true
            Layout.fillWidth: true
            checkable: true
            CheckBox {
                anchors.right: parent.right
                checked: parent.checked
                enabled: false
            }
            checked: false
        }

        Button {
            id: flipHorizontal
            text: qsTr("Flip horizontal")
            Layout.fillHeight: true
            Layout.fillWidth: true
            checkable: true
            CheckBox {
                anchors.right: parent.right
                checked: parent.checked
                enabled: false
            }
            checked: false
        }

        Button {
            id: textureLoading
            text: qsTr("Load image texture")
            Layout.fillHeight: true
            Layout.fillWidth: true
            checkable: true
            CheckBox {
                anchors.right: parent.right
                checked: parent.checked
                enabled: false
            }
            checked: false
        }
        Item {
            Layout.fillWidth: true
        }
    }
}
