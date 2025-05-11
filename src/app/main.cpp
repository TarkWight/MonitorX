#include <QApplication>
#include "DependencyInjection.hpp"
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    const QString configPath = QCoreApplication::applicationDirPath() + "/config.json";

    DI di(configPath);
    MainWindow window;
    window.setViewModel(di.monitorViewModel());
    window.show();

    return app.exec();
}
