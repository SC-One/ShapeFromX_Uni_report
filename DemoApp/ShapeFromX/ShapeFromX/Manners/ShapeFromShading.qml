import QtQuick 2.14
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

            v2ProxyData.pModel.heightMapFile = ""
            v2ProxyData.pModel.heightMapFile = myAlgo.fullOutV2FileName()

            v3ProxyData.pModel.heightMapFile = ""
            v3ProxyData.pModel.heightMapFile = myAlgo.fullOutV3FileName()
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
            DefaultSurfaceSeriesShading {
                id: v2ProxyData
                targetVisibleBtn: v2ProxyDataBtn

                targetFlatBtn: flatBtn
                targetGridBtn: gridBtn
                targetImageTextureBtn: textureLoading
                imagePath: rootItem.sourceImagePath
            }
            DefaultSurfaceSeriesShading {
                id: v3ProxyData
                targetVisibleBtn: v3ProxyDataBtn

                targetFlatBtn: flatBtn
                targetGridBtn: gridBtn
                targetImageTextureBtn: textureLoading
                imagePath: rootItem.sourceImagePath
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
            ParentDir.DefaultOptionBtn {
                id: normalBtn
                text: qsTr("Normal")
                checkable: true
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            ParentDir.DefaultOptionBtn {
                id: reverseBtn
                text: qsTr("Reverse")
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            ParentDir.DefaultOptionBtn {
                id: v2ProxyDataBtn
                text: qsTr("V2Proxy")
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            ParentDir.DefaultOptionBtn {
                id: v3ProxyDataBtn
                text: qsTr("V3Proxy")
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
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
            id: textureLoading
            text: qsTr("Load image texture")
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
