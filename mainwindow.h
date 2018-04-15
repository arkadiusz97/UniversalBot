#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "programManager.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_checkBoxGetPost_clicked(bool checked);

    void on_pushButtonStart_clicked();

    void endOfOneRequestSlot(bool succes, QString proxy, quint16 port, int status);

    void showAboutWindow();
private:
    Ui::MainWindow *ui;
    programManager instance;
    int numberOfCurrentRequestsSucces, numberOfCurrentRequestsError;
    bool paused;
    QString logs;
};

#endif // MAINWINDOW_H
