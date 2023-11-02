import QtQuick 2.15
import ir.HCoding.SocietyCleaner 1.0 as Manners
import QtDataVisualization 1.14
import QtQuick.Layouts 1.14

import "../" as ParentDir

MannerBase {
    id: rootItem
    function recalculate() {
        myAlgo.reCalculate()
    }

    Manners.ShapeFromShading {
        id: myAlgo
        currentImageURL: rootItem.sourceImagePath
        onOutputCalculated: {
            proxyModel.heightMapFile = ""
            proxyModel.heightMapFile = myAlgo.outputFileName()
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

    ColumnLayout {
        anchors.fill: parent

        Surface3D {
            id: surfacePlot
            Layout.fillHeight: true
            Layout.fillWidth: true

            theme: Theme3D {
                type: Theme3D.ThemeStoneMoss
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

            Surface3DSeries {
                id: heightSeries
                flatShadingEnabled: false
                drawMode: Surface3DSeries.DrawSurface
                visible: true

                HeightMapSurfaceDataProxy {
                    id: proxyModel
                    heightMapFile: "qrc:/test.png"
                }
            }
        }
    }

    Component.onCompleted: {
        rootItem.sourceImagePath = "qrc:/test.png"
    }
}
