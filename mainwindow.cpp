#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&instance, SIGNAL(endOfOneRequest(bool, QString, quint16, int, int, int, QString)), this, SLOT(endOfOneRequestSlot(bool, QString, quint16, int, int, int, QString)));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAboutWindow()));
    connect(ui->actionExportResults, SIGNAL(triggered(bool)), this, SLOT(exportResults()));
    connect(ui->actionSettings, SIGNAL(triggered(bool)), this, SLOT(openSettingsWindow()));
    connect(&settingsW, SIGNAL(timeoutChanged(int)), this, SLOT(setDataInInstance(int)));
    version = "1.1";
    settingsFileName = "UniversalBot_settings";
    setWindowTitle("UniversalBot " + version);
    instance.getSettingsFromFile(settingsFileName);
    settingsW.setTimeoutString(instance.getLoadedSettings()["timeout"].toInt());
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
    if(ui->textEditUrl->text().isEmpty())
    {
        QMessageBox::critical(this, "Critical error.", "Url field is empty.");
        return;
    }
    QString proxiesFileName = "proxies.txt";
    if(instance.loadProxies(proxiesFileName))
    {
        QMessageBox::critical(this, "Critical error.", "Couldn't find file \"" + proxiesFileName + "\". Copy this file to path with program and try again.");
        return;
    }
    ui->textBrowserLogs->clear();
    logs.clear();
    logs += "Started.\n";
    ui->textBrowserLogs->setText(logs);
    //numberOfCurrentRequestsError = numberOfCurrentRequestsSucces = 0;
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
    logs = R"( <font color="blue">Duration: )" + instance.getDuration() + "</font><br>" +
    QString("Done. Succesful requests: ") + QString::number(instance.getNumberOfCurrentRequestsSucces()) + QString(" Failed requests: ") + QString::number(instance.getNumberOfCurrentRequestsError()) + "<br/>" + logs;
    ui->textBrowserLogs->setText(logs);
}
void MainWindow::endOfOneRequestSlot(bool succes, QString proxy, quint16 port, int status, int numberOfCurrentRequestsSucces, int numberOfCurrentRequestsError, QString dateTime)
{
    QString statusMsg;
    if(succes)
        statusMsg = R"( <font color="green">succes</font>)";
    else
        statusMsg = R"( <font color="red">error</font>)";
    logs = QString("IP: <b>") + proxy + QString("</b> Port: <b>") + QString::number(port) + "</b>"
    + statusMsg + QString(" (status ") + QString::number(status) + QString(") ") + QString::number(numberOfCurrentRequestsSucces + numberOfCurrentRequestsError) + QString("/") + QString::number(instance.numberOfProxies())
    + R"( <font color="blue">)" + dateTime + "</font><br>" + logs;
    ui->textBrowserLogs->setHtml(logs);
}

void MainWindow::showAboutWindow()
{
    QMessageBox::information(this, "About", "UniversalBot " + version + ". Written by Arkadiusz97.");
}

void MainWindow::exportResults()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save results."), "", tr("HTML document (*.html)"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << ui->textBrowserLogs->document()->toHtml();
    file.close();
}

void MainWindow::openSettingsWindow()
{
    settingsW.show();
}

void MainWindow::setDataInInstance(int timeout)
{
    instance.setTimeout(timeout);
    instance.saveSettingsToFile(settingsFileName);
}
