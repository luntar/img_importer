import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

ApplicationWindow {
    id: root

    width: 1200
    height: 800
    visible: true
    title: "Score Application"

    Settings {
        id: app_settings
        property url last_score_folder
        property url last_import_folder
    }

    FolderDialog {
        id: open_score_dialog
        title: "Open Score Folder"
        currentFolder: app_settings.last_score_folder

        onAccepted: {
            app_settings.last_score_folder = selectedFolder
            status_label.text = "Score folder: " + selectedFolder
        }
    }

    FolderDialog {
        id: import_images_dialog
        title: "Import Images Folder"
        currentFolder: app_settings.last_import_folder

        onAccepted: {
            app_settings.last_import_folder = selectedFolder
            status_label.text = "Import folder: " + selectedFolder
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        Label {
            text: "Score Application"
            font.pixelSize: 28
            font.bold: true
        }

        RowLayout {
            spacing: 12

            Button {
                text: "Open Score"
                onClicked: open_score_dialog.open()
            }

            Button {
                text: "Import Images"
                onClicked: import_images_dialog.open()
            }
        }

        Label {
            id: status_label
            Layout.fillWidth: true
            text: "Open an existing score or choose a folder of source images."
            elide: Text.ElideMiddle
            opacity: 0.75
        }

        Rectangle {
            id: touch_viewport

            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            color: palette.midlight
            radius: 8

            Item {
                id: page_surface

                width: 700
                height: 500
                x: (touch_viewport.width - width) / 2
                y: (touch_viewport.height - height) / 2

                transform: [
                    Scale {
                        origin.x: page_surface.width / 2
                        origin.y: page_surface.height / 2
                        xScale: pinch_handler.activeScale
                        yScale: pinch_handler.activeScale
                    },
                    Translate {
                        x: drag_handler.activeTranslation.x
                        y: drag_handler.activeTranslation.y
                    }
                ]

                Rectangle {
                    anchors.fill: parent
                    color: "white"
                    border.width: 1

                    Label {
                        anchors.centerIn: parent
                        text: "Touch surface\n\nOne finger: pan\nTwo fingers: pinch zoom"
                        horizontalAlignment: Text.AlignHCenter
                        color: "black"
                    }
                }

                DragHandler {
                    id: drag_handler
                    target: null
                    acceptedDevices: PointerDevice.TouchScreen
                                     | PointerDevice.TouchPad
                                     | PointerDevice.Mouse
                }

                PinchHandler {
                    id: pinch_handler
                    target: null
                    minimumScale: 0.5
                    maximumScale: 6.0
                    acceptedDevices: PointerDevice.TouchScreen
                                     | PointerDevice.TouchPad
                }
            }
        }
    }
}
