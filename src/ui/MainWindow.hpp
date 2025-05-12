//===--  MonitorX/src/ui/MainWindow.hpp - Application Main Window --*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file declares the MainWindow class, which provides the main
// user interface for the application. It connects UI elements with
// the MonitorViewModel and handles interactions.
//
//===-------------------------------------------------------------------------===//

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class MonitorViewModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void setViewModel(MonitorViewModel *vm);

private:
    void setRunning(bool running);

    Ui::MainWindow *ui = nullptr;
    MonitorViewModel *m_vm = nullptr;
};

#endif // MAINWINDOW_HPP
