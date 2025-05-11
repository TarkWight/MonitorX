#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class MonitorViewModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    void setViewModel(MonitorViewModel* vm);

private:
    void setRunning(bool running);

    Ui::MainWindow*      ui;
    MonitorViewModel*    m_vm = nullptr;
};

#endif // MAINWINDOW_HPP
