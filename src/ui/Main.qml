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
            text: "Qt 6 + OpenCV baseline"
            opacity: 0.7
        }

        Button {
            text: "Import Folder"
            enabled: false
            ToolTip.visible: hovered
            ToolTip.text: "Folder import is the next implementation step."
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
