import QtQuick 2.14
import ir.HCoding.SocietyCleaner 1.0 as Manners
import QtDataVisualization 1.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import "../" as ParentDir

MannerBase {
    id: rootItem

    Manners.ShapeFromMonoCularNN {
        id: myAlgo

        mySurfaceSeries: mySeri
    }

    Item {
        id: prvObject

        ListModel {
            id: dataModel
        }
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
            shadowQuality: shadowOption.checked ? AbstractGraph3D.ShadowQualityMedium : AbstractGraph3D.ShadowQualityNone
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
            axisY.title: "Depth"
            axisX.title: "X"
            axisZ.title: "Y"

            Surface3DSeries {
                id: mySeri
                //                meshSmooth: true
                visible: true
                flatShadingEnabled: flatBtn.checked
                drawMode: gridBtn.checked ? Surface3DSeries.DrawSurfaceAndWireframe : Surface3DSeries.DrawSurface
                //                ItemModelSurfaceDataProxy {
                //                    id: dataModelProxy
                //                    itemModel: dataModel
                //                    rowRole: "y"
                //                    columnRole: "x"
                //                    yPosRole: "depth"
                //                }
                Component.onCompleted: {
                    myAlgo.setSeries(mySeri)
                }
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
        ParentDir.DefaultCheckableBtn {
            id: flatBtn
            text: qsTr("Flat surface")
            Layout.fillHeight: true
            Layout.fillWidth: true
            checked: true
        }
        ParentDir.DefaultCheckableBtn {
            id: gridBtn
            text: qsTr("Grid surface")
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        ParentDir.DefaultCheckableBtn {
            id: flipHorizontal
            text: qsTr("Flip horizontal")
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        ParentDir.DefaultCheckableBtn {
            id: shadowOption
            text: qsTr("Shadows")
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Item {
            Layout.fillWidth: true
        }
    }
}
