#include <gtest/gtest.h>
#include <qmlbind20/qmlbind20.h>
#include <leaf/logger.h>
#include <qtimer.h>
#include <qguiapplication.h>
#include <qqmlapplicationengine.h>

TEST(Example, Test)
{
  EXPECT_EQ(1, 1);
}

auto main(int argc, char** argv) -> int
{
  leaf::Logger(
    "test-logger",
    leaf::Logger::DefaultPatterns::SimpleWithThreadInfo,
    leaf::Logger::Level::Debug,
    leaf::Logger::Target::Stdout,
    std::nullopt,
    std::nullopt,
    std::nullopt
  );
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine("qrc:/qmlbind20/test.qml");
  QTimer::singleShot(0, [&]()
  {
    ::testing::InitGoogleTest(&argc, argv);
    const auto result = RUN_ALL_TESTS();
    //QGuiApplication::exit(result);
  });
  return QGuiApplication::exec();
}
