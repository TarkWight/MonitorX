//===-- MonitorX/src/app/main.cpp - Application Entry Point -----*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file defines the main() function, responsible for launching the Qt
// application, initializing the dependency injection container, and displaying
// the main window.
//
//===----------------------------------------------------------------------===//

#include <QApplication>
#include <QCoreApplication>

#include "DependencyInjection.hpp"
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const QString configPath =
        QCoreApplication::applicationDirPath() + "/config.json";

    DI di(configPath);

    MainWindow window;
    window.setViewModel(di.monitorViewModel());
    window.show();

    return app.exec();
}
