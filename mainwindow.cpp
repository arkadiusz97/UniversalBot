#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&instance, SIGNAL(endOfOneRequest(bool, QString, quint16, int)), this, SLOT(endOfOneRequestSlot(bool, QString, quint16, int)));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAboutWindow()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBoxGetPost_clicked(bool checked)
{
    ui->textEditContentPost->setEnabled(checked);
}

void MainWindow::on_pushButtonStart_clicked()
{
    if(instance.loadProxies("proxies.txt"))
    {
        QMessageBox::critical(this, "Critical error.", "Couldn't find file \"proxies.txt\". Copy this file to path with program and try again.");
        return;
    }
    ui->textBrowserLogs->clear();
    logs.clear();
    logs += "Started.\n";
    ui->textBrowserLogs->setText(logs);
    numberOfCurrentRequestsError = numberOfCurrentRequestsSucces = 0;
    paused = false;
    ui->pushButtonStart->setEnabled(false);
    ui->checkBoxGetPost->setEnabled(false);
    ui->textEditContentPost->setEnabled(false);
    ui->textEditUrl->setEnabled(false);
    if(ui->checkBoxGetPost->isChecked())
    {
        instance.runRequest(ui->textEditUrl->text(), true, QByteArray(ui->textEditContentPost->toPlainText().toStdString().c_str()));
    }
    else
    {
        instance.runRequest(ui->textEditUrl->text(), false, QByteArray(""));
    }
    ui->pushButtonStart->setEnabled(true);
    ui->checkBoxGetPost->setEnabled(true);
    ui->textEditUrl->setEnabled(true);
    if(ui->checkBoxGetPost->isChecked())
    {
        ui->textEditContentPost->setEnabled(true);
    }
    logs = QString("Done. Succesful requests: ") + QString::number(numberOfCurrentRequestsSucces) + QString(" Failed requests: ") + QString::number(numberOfCurrentRequestsError) + "<br/>" + logs;
    ui->textBrowserLogs->setText(logs);
}
void MainWindow::endOfOneRequestSlot(bool succes, QString proxy, quint16 port, int status)
{
    QString statusMsg;
    if(succes)
    {
        ++numberOfCurrentRequestsSucces;
        statusMsg = R"( <font color="green">succes</font>)";
    }
    else
    {
        ++numberOfCurrentRequestsError;
        statusMsg = R"( <font color="red">error</font>)";
    }
    logs = QString("Proxy: <b>") + proxy + QString("</b> Port: <b>") + QString::number(port) + "</b>" + statusMsg + QString(" (status ") + QString::number(status) + QString(") ") + QString::number(numberOfCurrentRequestsSucces + numberOfCurrentRequestsError) + QString("/") + QString::number(instance.numberOfProxies()) + QString("<br\>") + logs;
    ui->textBrowserLogs->setHtml(logs);
}

void MainWindow::showAboutWindow()
{
    QMessageBox::information(this, "About", "UniversalBot. Written by Arkadiusz97.");
}
