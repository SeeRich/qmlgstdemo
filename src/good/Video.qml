import QtQuick
import QtMultimedia 6.0
import org.freedesktop.gstreamer.Qt6GLVideoItem 1.0

Window {
    id: window
    visible: true
    title: qsTr("Live Video")
    color: "black"

    GstGLQt6VideoItem {
        id: video
        objectName: "videoItem"
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
    }
}
