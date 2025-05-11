#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "MonitorViewModel.hpp"

#include <QString>
#include <QFileDialog>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setViewModel(MonitorViewModel* vm)
{
    m_vm = vm;
    ui->lineEditPath->setText(vm->watchDirectory());
    ui->labelDeathsValue->setText(QString::number(vm->deaths()));
    ui->labelSavesValue->setText(QString::number(vm->saves()));
    setRunning(vm->isRunning());
    ui->listViewEvents->setModel(vm->eventModel());

    connect(ui->buttonBrowse, &QPushButton::clicked, this, [this](){
        QString dir = QFileDialog::getExistingDirectory(
            this,
            tr("Выберите папку с сохранениями"),
            m_vm->watchDirectory()
            );
        if (!dir.isEmpty()) {
            m_vm->setWatchDirectory(dir);
        }
    });
    connect(ui->buttonStart, &QPushButton::clicked,
            vm, &MonitorViewModel::start);
    connect(ui->buttonStop, &QPushButton::clicked,
            vm, &MonitorViewModel::stop);
    connect(ui->buttonLogs, &QPushButton::clicked,
            vm, &MonitorViewModel::openLogs);

    connect(vm, &MonitorViewModel::watchDirectoryChanged, this, [this] {
        ui->lineEditPath->setText(m_vm->watchDirectory());
    });
    connect(vm, &MonitorViewModel::statsChanged, this, [this] {
        ui->labelDeathsValue->setText(QString::number(m_vm->deaths()));
        ui->labelSavesValue->setText(QString::number(m_vm->saves()));
    });
    connect(vm, &MonitorViewModel::runningChanged, this, [this] {
        setRunning(m_vm->isRunning());
    });
}

void MainWindow::setRunning(bool running)
{
    ui->frameRunIndicator->setStyleSheet(QString(
                                             "QFrame { border-radius:8px; background-color:%1; }"
                                             ).arg(running ? "green" : "darkgreen"));
    ui->frameStopIndicator->setStyleSheet(QString(
                                              "QFrame { border-radius:8px; background-color:%1; }"
                                              ).arg(running ? "darkred" : "red"));

    ui->buttonStart->setEnabled(!running);
    ui->buttonStop->setEnabled(running);
}
