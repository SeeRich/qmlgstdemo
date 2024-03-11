# qmlgstdemo
This repository is designed to be a minimum reproducible example of a problem noticed when using he Qt/QML/GStreamer qml6glsink element.

## Setup:
* Windows 11 - MSVC 2022
* We will have to build GStreamer from source since none of the prebuilt binaries come with support for QML/Qt Quick.
* To do this requires some dependencies + configuration. Once this is setup, we will build GStreamer.

### Install some dependencies:
* Keep track of where you install these dependencies and update buildvars.bat as you go
1. C++ Compiler: MSVC via Visual Studio
2. Qt6: Download online installer. Make sure you install QML and Qt Quick components if not default selected
3. NASM: https://www.nasm.us/pub/nasm/releasebuilds/2.16.01/win64/
4. Strawberry Perl: https://strawberryperl.com/releases.html
5. Python -> Meson: https://www.python.org/downloads/
6. CMake: https://cmake.org/download/
7. Pkgconfig: choco install pkgconfiglite

### Setup python virtual environment with Meson:
```
python -m venv .\venv
venv\Scripts\activate
python -m pip install meson
```

### Compile/Build GStreamer
1. Clone GStreamer
```
git clone --single-branch --branch 1.24.0 -j6 --recurse-submodules https://gitlab.freedesktop.org/gstreamer/gstreamer.git
```
2. Run build
* This can take awhile > 30 mins...
* Output logs are written to gstreamer-build-log.txt
```
buildgstreamer.bat
```

### Compile build good/bad example:
```
build.bat
```

### Run examples:
* Double slashes are important on Windows:
```
build\good.exe --leftFilesrc C:\\Users\\Cole\\Desktop\\big_buck_bunny.mp4 --rightFilesrc C:\\Users\\Cole\\Desktop\\big_buck_bunny.mp4
build\bad.exe --leftFilesrc C:\\Users\\Cole\\Desktop\\big_buck_bunny.mp4 --rightFilesrc C:\\Users\\Cole\\Desktop\\big_buck_bunny.mp4
```

### Link to GStreamer Qt6 QML example:
https://gitlab.freedesktop.org/gstreamer/gstreamer/-/blob/1.22.0/subprojects/gst-plugins-good/tests/examples/qt6/qmlsink/main.qml