#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>

namespace Ui {
class settingsWindow;
}

class settingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit settingsWindow(QWidget *parent = nullptr);
    void setTimeoutString(int timeout);
    ~settingsWindow();
signals:
    void timeoutChanged(int timeout);
private slots:
    void on_pushButtonApply_clicked();
    void on_lineEditTimeout_textEdited(const QString &arg1);

private:
    Ui::settingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
