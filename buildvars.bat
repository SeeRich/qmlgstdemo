REM Set up Microsoft Visual Studio 2022, where <arch> is amd64, x86, etc.
CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64

REM Set up Qt6 variables
SET _QT_ROOT=C:\Tools\Qt6.5.1
SET _QT_VERSION=6.5.1
SET PATH=%_QT_ROOT%\%_QT_VERSION%\msvc2019_64\bin;%PATH%

REM Add NASM to the path
SET PATH=C:\Program Files\NASM;%PATH%
REM Add Strawberry Perl to the path
SET PATH=C:\Strawberry\perl\bin;%PATH%

REM Set up GStreamer variables
SET PATH=%CD%\gstreamer\gstreamer-install\bin;%PATH%
SET GST_PLUGIN_PATH=%CD%\gstreamer\gstreamer-install\lib\gstreamer-1.0
