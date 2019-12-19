#include "mainwindow.h"

int main(int argc, char* argv[])
{
    MainWindow   *mainWindow;
    QApplication app(argc, argv);

    mainWindow = new MainWindow;
    auto *widget = mainWindow->gnuplotWidget();
    mainWindow->show();
    widget->plot("set grid; plot x**2 w l lt 2 lw 4");

    app.exec();

    return 0;
}
