#include <gst/gst.h>
#include <QApplication>
#include <QDockWidget>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QQuickWidget>
#include <QSplitter>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QtGui>
#include <QtQml>
#include <QtQuick>

#include <iostream>
#include <type_traits>

class SetPlaying : public QRunnable
{
public:
    SetPlaying(GstElement*);
    ~SetPlaying();

    void run();

private:
    GstElement* pipeline_;
};

SetPlaying::SetPlaying(GstElement* pipeline)
{
    this->pipeline_ = pipeline ? static_cast<GstElement*>(gst_object_ref(pipeline)) : NULL;
}

SetPlaying::~SetPlaying()
{
    if(this->pipeline_)
        gst_object_unref(this->pipeline_);
}

void SetPlaying::run()
{
    if(this->pipeline_) {
        std::cout << "SbsVideoWidget: set playing pipeline" << std::endl;
        gst_element_set_state(this->pipeline_, GST_STATE_PLAYING);
    }
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const std::string& leftFilesrc, const std::string& rightFilesrc)
        : _leftVideoFilepath(leftFilesrc), _rightVideoFilepath(rightFilesrc)
    {
        setCentralWidget(createCentralWidget());
    }

    QWidget* createCentralWidget()
    {
        // Create a new widget to encapsulate the video with splitters
        auto w = new QWidget(this);
        auto layout = new QVBoxLayout;
        w->setLayout(layout);

        auto splitter = new QSplitter(Qt::Horizontal);
        layout->addWidget(splitter);

        // Left Video dock widget
        auto leftPipeline = createVideoPipeline(_leftVideoFilepath);
        auto leftVideoWidget = createVideoWidget(leftPipeline);
        if(leftPipeline && leftVideoWidget) {
            leftVideoWidget->resize(720, 405);
            splitter->addWidget(leftVideoWidget);
            _leftPipeline = leftPipeline;
        } else {
            std::cout << "Failed to create left video pipelines + widget" << std::endl;
        }

        // Right Video dock widget
        auto rightPipeline = createVideoPipeline(_rightVideoFilepath);
        auto rightVideoWidget = createVideoWidget(rightPipeline);
        if(rightPipeline && leftVideoWidget) {
            rightVideoWidget->resize(720, 405);
            splitter->addWidget(rightVideoWidget);
            _rightPipeline = rightPipeline;
        } else {
            std::cout << "Failed to create right video pipelines + widget" << std::endl;
        }

        return w;
    }

    auto createVideoWidget(GstElement* pipeline) -> QWidget*
    {
        // Construct quick widget
        auto qw = new QQuickWidget(this);
        qw->setResizeMode(QQuickWidget::SizeRootObjectToView);

        connect(qw, &QQuickWidget::statusChanged, this, [](QQuickWidget::Status status) {
            std::cout << std::format(
                "SbsVideoWidget::quickWidgetStatusChanged: {}\n",
                static_cast<std::underlying_type_t<QQuickWidget::Status>>(status));
        });
        connect(qw,
                &QQuickWidget::sceneGraphError,
                this,
                [](QQuickWindow::SceneGraphError, const QString& message) {
                    std::cout << std::format("SbsVideoWidget::sceneGraphError: {}\n",
                                             message.toStdString());
                });

        // Must set the URL after the pipeline has been parsed/instantiated. Once the pipeline is
        // loaded, GstGLQt6VideoItem will be available inside QML
        const QUrl url(QStringLiteral("qrc:/Video.qml"));
        qw->setSource(url);

        // Connect pipeline to QML
        auto rootObject = qw->rootObject();
        if(rootObject) {
            QQuickItem* videoItem = rootObject->findChild<QQuickItem*>("videoItem");
            g_assert(videoItem);
            auto sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
            g_object_set(sink, "widget", videoItem, NULL);
        }

        qw->quickWindow()->scheduleRenderJob(new SetPlaying(pipeline),
                                             QQuickWindow::BeforeSynchronizingStage);

        return qw;
    }

    auto createVideoPipeline(const std::string& filepath) -> GstElement*
    {
        // Build the pipeline
        std::stringstream ss;
        // clang-format off
        ss << std::format("filesrc location={} ! qtdemux", filepath);
        ss << " ! " << "queue";
        ss << " ! " << "decodebin";
        ss << " ! " << "queue";
        ss << " ! " << "videoconvert";
        ss << " ! " << "queue";
        ss << " ! " << "glupload";
        ss << " ! " << std::format("qml6glsink name=sink");
        // clang-format on
        std::cout << "Pipeline: " << ss.str() << std::endl;
        // clang-format on
        auto pipeline = gst_parse_launch(ss.str().c_str(), NULL);
        if(!pipeline) {
            std::cout << "SbsVideoWidget: failed to parse Gstreamer Pipeline" << std::endl;
            return nullptr;
        }
        // Increase the ref count on the pipeline
        gst_object_ref(pipeline);
        return pipeline;
    }

public:
    std::string _leftVideoFilepath;
    std::string _rightVideoFilepath;
    GstElement* _leftPipeline{nullptr};
    GstElement* _rightPipeline{nullptr};
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // GStreamer QML support currently requires OpenGL backend
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    // Parse CLI arguments
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption leftFilesrcOption(
        "leftFilesrc",
        QCoreApplication::translate("main", "Left video file source filename"),
        "leftFilesrc");
    QCommandLineOption rightFilesrcOption(
        "rightFilesrc",
        QCoreApplication::translate("main", "Right video file source filename"),
        "rightFilesrc");
    parser.addOption(leftFilesrcOption);
    parser.addOption(rightFilesrcOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    // Handle filesrc arguments
    if(!parser.isSet(leftFilesrcOption) || !parser.isSet(rightFilesrcOption)) {
        std::cout << "You must provide left and right video files for playback" << std::endl;
    }
    auto leftFilesrc = parser.value(leftFilesrcOption).toStdString();
    auto rightFileSrc = parser.value(rightFilesrcOption).toStdString();

    std::cout << "Initializing GStreamer..." << std::endl;
    gst_init(&argc, &argv);
    std::cout << "Initializing GStreamer complete" << std::endl;

    MainWindow mainWindow(leftFilesrc, rightFileSrc);
    mainWindow.show();

    auto ret = app.exec();

    if(mainWindow._leftPipeline) {
        gst_element_set_state(mainWindow._leftPipeline, GST_STATE_NULL);
        gst_object_unref(mainWindow._leftPipeline);
    }

    if(mainWindow._rightPipeline) {
        gst_element_set_state(mainWindow._rightPipeline, GST_STATE_NULL);
        gst_object_unref(mainWindow._rightPipeline);
    }

    gst_deinit();

    return ret;
}

#include "Main.moc"