import QtQuick
import QtMultimedia 6.0
import org.freedesktop.gstreamer.Qt6GLVideoItem 1.0

Rectangle {
    id: window
    color: "black"
    onWidthChanged: videoContainer.sizeUpdated(width, height)

    Rectangle {
        id: videoContainer
        anchors.centerIn: parent
        color: "black"
        height: parent.height
        width: height * 16.0 / 9.0

        function sizeUpdated(width, height) {
            var dimensions = calculateDimensions(width, height, 16, 9);
            console.log(dimensions[0], dimensions[1], dimensions[0] / dimensions[1]);
            videoContainer.width = dimensions[0];
            videoContainer.height = dimensions[1];
        }

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

    function calculateDimensions(width, height, aspectRatioWidth, aspectRatioHeight) {
        var aspectRatio = aspectRatioWidth / aspectRatioHeight;
        var containerAspectRatio = width / height;
        var resultWidth, resultHeight;
        if (containerAspectRatio > aspectRatio) {
            // Container is wider than the aspect ratio, so height is the limiting factor
            resultHeight = height;
            resultWidth = height * aspectRatio;
        } else {
            // Container is taller than the aspect ratio, so width is the limiting factor
            resultWidth = width;
            resultHeight = width / aspectRatio;
        }
        return [resultWidth, resultHeight];
    }
}
