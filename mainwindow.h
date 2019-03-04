#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include "programManager.h"
#include "settingswindow.h"
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
    void endOfOneRequestSlot(bool succes, QString proxy, quint16 port, int status, int numberOfCurrentRequestsSucces, int numberOfCurrentRequestsError, QString dateTime);
    void showAboutWindow();
    void exportResults();
    void openSettingsWindow();
    void setDataInInstance(int timeout);
private:
    Ui::MainWindow *ui;
    programManager instance;
    bool paused;
    QString logs, settingsFileName, version;
    settingsWindow settingsW;
};

#endif // MAINWINDOW_H
