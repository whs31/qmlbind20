#include <gtest/gtest.h>
#include <qmlbind20/qmlbind20.h>
#include <qtimer.h>
#include <qguiapplication.h>
#include <qqmlapplicationengine.h>

TEST(Example, Test)
{
  EXPECT_EQ(1, 1);
}

auto main(int argc, char** argv) -> int
{
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
