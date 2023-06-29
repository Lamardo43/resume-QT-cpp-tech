#include "mainwindow.h"
#include "singletonClient.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setTrayIconActions();
    this->showTrayIcon();

    ui->last_doing->setVisible(false);
    ui->last_doing_label->setVisible(false);

    connect(SingletonClient::getInstance(), &SingletonClient::get_client,
            this, &MainWindow::slot_on_get_client);

    SingletonClient::getInstance()->send_msg_to_server("get_clients");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTrayIcon()
{
    // Create an instance of QSystemTrayIcon and set its properties...
    trayIcon = new QSystemTrayIcon(this);
    QIcon trayImage(":/images/fresh-idea.jpg");
    trayIcon->setIcon(trayImage);
    trayIcon->setContextMenu(trayIconMenu);

    // Connect the tray icon's activation event to the appropriate slot
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    // Show the tray icon
    trayIcon->show();
}

void MainWindow::trayActionExecute()
{
    QMessageBox::information(this, "TrayIcon", "За вами следит начальство.");
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            this->trayActionExecute();
            break;
        default:
            break;
    }
}

void MainWindow::setTrayIconActions()
{
    // Set actions...
    minimizeAction = new QAction("Свернуть", this);
    restoreAction = new QAction("Восстановить", this);
    quitAction = new QAction("Выход", this);

    // Connect actions to slots...
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(minimizeToTray()));
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(restoreFromTray()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Set system tray's icon menu...
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(quitAction);
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange)
    {
        if (isMinimized())
        {
            minimizeToTray();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon && trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void MainWindow::minimizeToTray()
{
    hide();
}

void MainWindow::restoreFromTray()
{
    showNormal();
    activateWindow();
}

void MainWindow::on_pushButton_clicked()
{

}


void MainWindow::on_pushButton_2_clicked()
{
    ui->listWidget->clear();

    ui->last_doing->setVisible(false);
    ui->last_doing_label->setVisible(false);

    SingletonClient::getInstance()->send_msg_to_server("get_clients");
}

void MainWindow::slot_on_get_client(QString message) {
    parts = message.split(" ");

    if(parts[0] == "send_clients") {

        for (int i = 1; i < parts.size(); i++) {
            QList<QString> parts_of_part = parts[i].split("&");

            clients[parts_of_part[0]] = parts_of_part[1];

            ui->listWidget->addItem(parts_of_part[0]);
        }
    }
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{

    foreach(QString part, parts) {
        QList<QString> parts_of_part = part.split("&");

        if (item->text() == parts_of_part[0]) {
            ui->last_doing_label->setText(parts_of_part[1].replace("_", " "));
        }
    }

    ui->last_doing->setVisible(true);
    ui->last_doing_label->setVisible(true);
}

