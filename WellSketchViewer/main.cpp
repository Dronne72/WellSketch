#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;

    QFile styleFile(":/data/styles.css");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(styleSheet);
    }
    // } else {
    //     // Или применение встроенных стилей
    //     qApp->setStyleSheet(darkThemeStylesheet());
    // }

    w.showMaximized();
    w.init();
    return a.exec();
}
