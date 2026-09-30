import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 1200
    height: 800
    visible: true
    title: "Image Importer"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        Label {
            text: "Score Image Importer"
            font.pixelSize: 28
            font.bold: true
        }

        Label {
            text: "Touch enabled: drag to pan, pinch to zoom"
            opacity: 0.7
        }

        Button {
            text: "Import Folder"
            enabled: false
            ToolTip.visible: hovered
            ToolTip.text: "Folder import is the next implementation step."
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
