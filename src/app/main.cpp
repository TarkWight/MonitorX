// src/app/main.cpp
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "MonitorViewModel.hpp"
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 1) Загрузить перевод Qt (для стандартных виджетов)
    QTranslator qtTrans;
    qtTrans.load(QLocale::system(),
                 "qtbase",
                 "_",
                 QLibraryInfo::path(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTrans);

    // 2) Загрузить перевод самого приложения
    QTranslator appTrans;
    if (appTrans.load(":/i18n/MonitorX_ru_RU.qm"))
        app.installTranslator(&appTrans);

    // 3) Создать ViewModel (он внутри сам создаст ConfigManager и Service)
    MonitorViewModel *vm = new MonitorViewModel("config/config.json", &app);

    // 4) Создать главное окно и «привязать» к нему VM
    MainWindow w(nullptr);
    // Предполагаем, что в MainWindow есть метод setViewModel()
    // либо вы прям в конструкторе MainWindow создаете VM.
    w.setViewModel(vm);

    w.show();
    return app.exec();
}
