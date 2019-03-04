#include "settingswindow.h"
#include "ui_settingswindow.h"

settingsWindow::settingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::settingsWindow)
{
    ui->setupUi(this);
}

settingsWindow::~settingsWindow()
{
    delete ui;
}

void settingsWindow::on_pushButtonApply_clicked()
{
    emit timeoutChanged(ui->lineEditTimeout->text().toInt());
    ui->pushButtonApply->setEnabled(false);
}

void settingsWindow::setTimeoutString(int timeout)
{
    ui->lineEditTimeout->setText(QString::number(timeout));
}

void settingsWindow::on_lineEditTimeout_textEdited(const QString &arg1)
{
    ui->pushButtonApply->setEnabled(true);
}
