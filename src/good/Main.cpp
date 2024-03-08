#include <QtGui>
#include <QtQml>
#include <QtQuick>
#include <gst/gst.h>

#include <iostream>

class SetPlaying : public QRunnable {
public:
  SetPlaying(GstElement *);
  ~SetPlaying();

  void run();

private:
  GstElement *pipeline_;
};

SetPlaying::SetPlaying(GstElement *pipeline) {
  this->pipeline_ =
      pipeline ? static_cast<GstElement *>(gst_object_ref(pipeline)) : NULL;
}

SetPlaying::~SetPlaying() {
  if (this->pipeline_)
    gst_object_unref(this->pipeline_);
}

void SetPlaying::run() {
  if (this->pipeline_)
    gst_element_set_state(this->pipeline_, GST_STATE_PLAYING);
}

int main(int argc, char *argv[]) {

  QGuiApplication app(argc, argv);

  // GStreamer QML support currently requires OpenGL backend
  QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

  // Parse CLI arguments
  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  QCommandLineOption filesrcOption(
      "filesrc", QCoreApplication::translate("main", "File source filename"),
      "filesrc");
  parser.addOption(filesrcOption);
  // Process the actual command line arguments given by the user
  parser.process(app);

  // Handle filesrc argument
  if (!parser.isSet(filesrcOption)) {
    std::cout << "You must provide a video file for playback" << std::endl;
  }
  auto filesrcPath = parser.value(filesrcOption).toStdString();

  int ret = 0;

  std::cout << "Initializing GStreamer..." << std::endl;
  gst_init(&argc, &argv);
  std::cout << "Initializing GStreamer complete" << std::endl;

  GstElement *pipeline{nullptr};
  {

    // Build the pipeline
    std::stringstream ss;
    // clang-format off
    ss << std::format("filesrc location={} ! qtdemux", filesrcPath);
    ss << " ! " << "queue";
    ss << " ! " << "decodebin";
    ss << " ! " << "queue";
    ss << " ! " << "videoconvert";
    ss << " ! " << "queue";
    ss << " ! " << "glupload";
    ss << " ! " << "qml6glsink name=sink";
    // clang-format on
    std::cout << "Pipeline: " << ss.str() << std::endl;
    pipeline = gst_parse_launch(ss.str().c_str(), NULL);
    auto sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/Video.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
          if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    // Find and set the videoItem on the sink
    QQuickWindow *rootObject =
        qobject_cast<QQuickWindow *>(engine.rootObjects().first());
    QQuickItem *videoItem = rootObject->findChild<QQuickItem *>("videoItem");
    g_assert(videoItem);
    g_object_set(sink, "widget", videoItem, NULL);

    rootObject->scheduleRenderJob(new SetPlaying(pipeline),
                                  QQuickWindow::BeforeSynchronizingStage);

    ret = app.exec();
  }

  if (pipeline) {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
  }

  gst_deinit();

  return ret;
}
