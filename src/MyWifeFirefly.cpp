//
// Created by arch_ on 24-10-24.
//
#include "Log.h"
#include <iostream>
#include <QApplication>
#include "FireflyWindow.h"
using std::cerr;
using std::endl;
int main(int argc, char* argv[])
{
    if (!init()) {
        cerr << "Failed to initialize logger!" << endl;
        exit(1);
    }

    QApplication app { argc, argv };
    MainWindow window { &app };
    window.show();
    return QApplication::exec();
}