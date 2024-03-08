PUSHD gstreamer
DEL gstreamer-build-log.txt
REM Configure GStreamer
meson setup --prefix %CD%\gstreamer-install -Dgst-plugins-good:qt6=enabled -Dgst-plugins-bad:d3d12=disabled^
    -Dges=disabled^
    --buildtype=release %CD%\gstreamer-build >> gstreamer-build-log.txt
REM Compile the code
ninja -C %CD%\gstreamer-build install >> gstreamer-build-log.txt
POPD