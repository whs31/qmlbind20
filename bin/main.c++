#include <leaf/logger.h>
#include <qmlbind20/qmlbind20.h>
#include <qtimer.h>
#include <qguiapplication.h>
#include <qqmlapplicationengine.h>

auto main(int argc, char** argv) -> int
{
  const auto logger = leaf::Logger(
    "test-logger",
    leaf::Logger::DefaultPatterns::SimpleWithThreadInfo,
    leaf::Logger::Level::Debug,
    leaf::Logger::Target::Stdout,
    std::nullopt,
    std::nullopt,
    std::nullopt
  );
  QGuiApplication app(argc, argv);
  const QQmlApplicationEngine engine("qrc:/qmlbind20/test.qml");
  QObject::connect(&engine, &QQmlEngine::quit, qApp, &QCoreApplication::quit);
  return QGuiApplication::exec();
}
