import QtQuick
import QtMultimedia 6.0
import org.freedesktop.gstreamer.Qt6GLVideoItem 1.0

Rectangle {
    id: window
    visible: true
    color: "black"

    GstGLQt6VideoItem {
        id: video
        objectName: "videoItem"
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
    }
}
