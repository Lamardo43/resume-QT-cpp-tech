#include "mainwindow.h"
#include "singletonClient.h"
#include "ui_mainwindow.h"
#include "../symbols.h"
#include <zlib.h>
#include <QMenu>
#include <QMessageBox>
#include <QImageWriter>
#include <QImageReader>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setTrayIconActions();
    this->showTrayIcon();

    ui->last_doing->setVisible(false);
    ui->last_doing_label->setVisible(false);

    connect(SingletonClient::getInstance(), &SingletonClient::get_client,
            this, &MainWindow::slot_on_get_client);
    connect(SingletonClient::getInstance(), &SingletonClient::get_scr,
            this, &MainWindow::slot_on_get_scr);
    connect(SingletonClient::getInstance(), &SingletonClient::set_scr,
            this, &MainWindow::slot_on_set_scr);

    SingletonClient::getInstance()->send_msg_to_server("get_clients");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    QIcon trayImage(":/images/fresh-idea.jpg");
    trayIcon->setIcon(trayImage);
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

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
    restoreAction = new QAction("Восстановить", this);
    quitAction = new QAction("Выход", this);


    connect(restoreAction, SIGNAL(triggered()), this, SLOT(restoreFromTray()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Set system tray's icon menu...
    trayIconMenu = new QMenu(this);
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
    QListWidgetItem *item = ui->listWidget->currentItem();

    if (item) {
        SingletonClient::getInstance()->send_msg_to_server("get_screenshot" + SPLIT_SYMBOL + item->text().toUtf8());
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->listWidget->clear();

    ui->last_doing->setVisible(false);
    ui->last_doing_label->setVisible(false);

    SingletonClient::getInstance()->send_msg_to_server("get_clients");
}

void MainWindow::slot_on_get_client(QString message) {
    parts = message.split(SPLIT_SYMBOL);
    parts.removeOne(END_SYMBOL);

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

void MainWindow::slot_on_get_scr(QByteArray owner) {

    QScreen *screen = QGuiApplication::primaryScreen();

    if (screen) {
        QPixmap screenshot = screen->grabWindow(0);

        QByteArray byteArray = convertPixmapToJSONString(screenshot).toUtf8();

        qDebug() << byteArray.size();
        SingletonClient::getInstance()->send_msg_to_server("send_screenshot_to" + SPLIT_SYMBOL + owner + SPLIT_SYMBOL  + byteArray);
    }
}

void MainWindow::slot_on_set_scr(QByteArray byteArray) {
    qDebug() << byteArray.size();
    ui->picture->setPixmap(convertJSONStringToPixmap(byteArray));
}

QString  MainWindow::convertPixmapToJSONString(const QPixmap& pixmap) {
    QJsonObject jsonObject;
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "PNG");

        QString base64Data = byteArray.toBase64();
        jsonObject["imageData"] = base64Data;

        QJsonDocument jsonDoc(jsonObject);
        return jsonDoc.toJson();
}

QPixmap MainWindow::convertJSONStringToPixmap(const QString& jsonString) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonObject jsonObject = jsonDoc.object();

        QString base64Data = jsonObject["imageData"].toString();
        QByteArray byteArray = QByteArray::fromBase64(base64Data.toUtf8());

        QPixmap pixmap;
        pixmap.loadFromData(byteArray, "PNG");
        return pixmap;
}
