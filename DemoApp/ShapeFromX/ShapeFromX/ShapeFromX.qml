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
        convertingWays.sync()
    }
    QtObject {
        id: prvObj
        ListModel {
            id: convertingWays
        }
    }

    ButtonGroup {
        id: radioGroup
        buttons: radioButtonsLayout.children
    }
    PageIndicator {
        id: indicator

        count: view.count
        currentIndex: view.currentIndex

        anchors.bottom: view.bottom
        anchors.horizontalCenter: view.horizontalCenter
    }
    ColumnLayout {
        id: generalLayout
        anchors.fill: parent
        RowLayout {
            id: mainRow

            SwipeView {
                id: view
                currentIndex: 1
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Column {
                id: radioButtonsLayout
                spacing: 5
                Layout.fillHeight: true
                Repeater {
                    model: convertingWays
                    delegate: RadioButton {
                        text: title
                        ButtonGroup.group: radioGroup
                        checked: true
                    }
                }
            }
        }
    }
}
