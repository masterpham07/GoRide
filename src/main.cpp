#include <QApplication>
#include <QFile>

#include "ui/qt/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QFile styleFile(
        QStringLiteral(":/styles/goride.qss")
    );

    if (styleFile.open(QFile::ReadOnly | QFile::Text))
    {
        app.setStyleSheet(
            QString::fromUtf8(styleFile.readAll())
        );
    }

    MainWindow window;
    window.show();

    return app.exec();
}
