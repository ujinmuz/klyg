#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <glwi.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/res/press_calc.png"));
    MainWindow w;

    GLWi* gl=w.findChild<GLWi*>();

    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(format);

    w.show();

    return a.exec();
}
