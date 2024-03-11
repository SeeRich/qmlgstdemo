import QtQuick
import QtMultimedia 6.0
import org.freedesktop.gstreamer.Qt6GLVideoItem 1.0

Rectangle {
    id: window
    visible: true
    color: "black"

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width / 2
        height: parent.height

        GstGLQt6VideoItem {
            id: videoLeft
            objectName: "leftVideoItem"
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
        }
    }

    Rectangle {
        anchors.top: parent.top
        anchors.right: parent.right
        width: parent.width / 2
        height: parent.height

        GstGLQt6VideoItem {
            id: videoRight
            objectName: "rightVideoItem"
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
        }
    }
}
