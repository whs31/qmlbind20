import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import io.qmlbind20.test 1.0

ApplicationWindow
{
    width: 300
    height: 300
    visible: true
    color: "magenta"

    Text {
        QtMock { id: mock }
        text: mock.addPrefix("World", "Hello, ")Z
    }
}