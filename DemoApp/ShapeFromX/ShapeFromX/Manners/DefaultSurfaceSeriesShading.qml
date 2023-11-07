import QtQuick 2.15
import QtDataVisualization 1.14

Surface3DSeries {
    property var targetVisibleBtn
    property var targetFlatBtn
    property var targetGridBtn
    property var targetImageTextureBtn
    property var imagePath
    property alias pModel: reverseProxyModel

    visible: targetVisibleBtn.checked
    flatShadingEnabled: targetFlatBtn.checked
    drawMode: targetGridBtn.checked ? Surface3DSeries.DrawSurfaceAndWireframe : Surface3DSeries.DrawSurface
    HeightMapSurfaceDataProxy {
        id: reverseProxyModel
    }

    property var defaultTexture: imagePath ? imagePath.toString(
                                                 ).substring(8) : ""
    textureFile: targetImageTextureBtn.checked ? defaultTexture : ""
}
