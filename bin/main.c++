#include <leaf/logger.h>
#include <qmlbind20/qmlbind20.h>
#include <qtimer.h>
#include <qguiapplication.h>
#include <qqmlapplicationengine.h>

auto main(int argc, char** argv) -> int
{
  const auto _ = leaf::Logger(
    "test-logger",
    leaf::Logger::DefaultPatterns::SimpleWithThreadInfo,
    leaf::Logger::Level::Trace,
    leaf::Logger::Target::Stdout,
    std::nullopt,
    std::nullopt,
    std::nullopt
  );
  QGuiApplication app(argc, argv);

  /* # tests # */
  namespace qml = qmlbind20;
  const auto m = qml::module_("test", 1, 0);

  const QQmlApplicationEngine engine("qrc:/qmlbind20/test.qml");
  QObject::connect(&engine, &QQmlEngine::quit, qApp, &QCoreApplication::quit);
  return QGuiApplication::exec();
}
