
import QtQuick 6.3
import QtQuick.Window 6.3
import Colorize
import QtQuick.Dialogs
import QtQuick.Controls
import backend


Window {
    id: main
    width: 800
    height: 600

    visible: true
    modality: Qt.ApplicationModal
    flags: Qt.Window
    contentOrientation: Qt.LandscapeOrientation
    minimumHeight: 480
    minimumWidth: 640
    title: "Colorize"

    //visibility: Window.Maximized


    Rectangle{
        anchors.fill: parent
        color: "#262626"

        //menu
        Rectangle {
            z: 1
            id: menuRect
            width: parent.width
            height:100
            color: "#959595"

            Row {

                id: column
                width: parent.width
                height: parent.height
                padding: Constants.gutter
                spacing: Constants.gutter

                RoundButton {
                    id: openButton
                    text: "Open"
                    height: parent.height - 2*Constants.gutter
                    width: parent.height - 2*Constants.gutter
                    onClicked: openDialog.open()
                }

                RoundButton {
                    id: updateButton
                    text: "Fill"
                    height: parent.height - 2*Constants.gutter
                    width: parent.height - 2*Constants.gutter
                    onClicked: workspace.fill_request()
                }

                RoundButton {
                    id: exitButton
                    text: "Exit"
                    height: parent.height - 2*Constants.gutter
                    width: parent.height - 2*Constants.gutter
                    onClicked: main.close()
                }


            }
        }
        //working space
        Rectangle {
            color: "#220022"
            anchors.top: menuRect.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
                Renderer{
                    id: workspace
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
        }

        // Just a simple file dialog to choose an image
        FileDialog {
            id: openDialog
            onAccepted: {
                console.log("Accepted: " + openDialog.fileUrls)
                if (openDialog.currentFile)
                    image.source = openDialog.currentFile
            }
            onRejected: { console.log("Rejected") }
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/
