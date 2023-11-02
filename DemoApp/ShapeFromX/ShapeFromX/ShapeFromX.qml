import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {
    id: rootLayout
    function addNewUI(title, qmlCompAddress) {
        var data = {
            "title": title,
            "source": qmlCompAddress
        }
        convertingWays.append(data)
    }
    function removeUI(title) {
        for (var i = 0; i < convertingWays.count; ++i) {
            if (title === convertingWays.get(i).title) {
                convertingWays.remove(i)
                return true
            }
        }
        return false
    }
    ListModel {
        id: convertingWays
    }
    QtObject {
        id: prvObj
    }

    ButtonGroup {
        id: radioGroup
        buttons: radioButtonsLayout.children
    }
    ColumnLayout {
        id: generalLayout
        anchors.fill: parent
        FileChooser {
            id: fileChooser
            property int fixedWidth: view.item ? view.item.width : 350
            Layout.maximumWidth: fixedWidth
            Layout.minimumWidth: fixedWidth
            Layout.minimumHeight: 70
            onCurrentPathChanged: {
                view.item.sourceImagePath = currentPath
            }
        }

        RowLayout {
            id: mainRow
            Loader {
                id: view
                Layout.fillWidth: true
                Layout.fillHeight: true
                asynchronous: true
                active: true
                source: mainRptr.whichoneSelected
                        !== undefined ? mainRptr.whichoneSelected : "qrc:/Manners/MannerBase.qml"
                onStateChanged: {
                    console.log(view.state)
                }
            }

            ColumnLayout {
                id: radioButtonsLayout
                spacing: 5
                Layout.fillHeight: true
                Item {
                    Layout.fillHeight: true
                }

                Repeater {
                    id: mainRptr
                    property var whichoneSelected

                    model: convertingWays
                    delegate: RadioButton {
                        text: model.title
                        property var sourceComp: model.source
                        ButtonGroup.group: radioGroup

                        onCheckedChanged: {
                            if (checked) {
                                mainRptr.whichoneSelected = sourceComp
                            }
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
}
