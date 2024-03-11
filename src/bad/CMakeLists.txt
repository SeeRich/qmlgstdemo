set(TARGET bad)

qt_add_executable(${TARGET}
    Main.cpp
)

if(MSVC)
    # Set the warning level to W4 and treat all warnings as errors
    target_compile_options(${TARGET} PRIVATE /W4 /WX)
endif()

target_link_libraries(${TARGET} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Quick Qt6::QuickWidgets)
target_link_libraries(${TARGET} PRIVATE GStreamer::GStreamer)

qt6_add_resources(${TARGET} "video"
    PREFIX
        "/"
    FILES
        Video.qml
)