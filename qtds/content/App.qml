
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
            anchors.top: menuRect.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            // Important things!

                Renderer{
                     anchors.fill: parent
                }

                Flickable {
                    anchors.fill: parent
                    anchors.topMargin: 100

                    contentWidth: Math.max(image.width * image.scale, parent.width)
                    contentHeight: Math.max(image.height * image.scale, parent.height)
                    clip: true

                    Image {
                        id: image

                        property real zoom: 0.0
                        property real zoomStep: 0.1

                        asynchronous: true
                        cache: false
                        smooth: true
                        antialiasing: true
                        mipmap: true

                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        transformOrigin: Item.Center
                        scale: Math.min(parent.width / width, parent.height / height, 1) + zoom

                        //source: openDialog.fileUrl
                    }
                }


                // Mouse zoom
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.NoButton

                    onWheel: {
                        if (wheel.angleDelta.y > 0)
                            image.zoom = Number((image.zoom + image.zoomStep).toFixed(1))
                        else
                            if (image.zoom > 0) image.zoom = Number((image.zoom - image.zoomStep).toFixed(1))

                        wheel.accepted=true
                    }
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
