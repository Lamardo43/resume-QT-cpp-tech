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
        SingletonClient::getInstance()->send_msg_to_server("get_screenshot" + SPLIT_SYMBOL + item->text());
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

void MainWindow::slot_on_get_scr(QString owner) {

    QScreen *screen = QGuiApplication::primaryScreen();

    if (screen) {

        QPixmap screenshot = screen->grabWindow(0);

        QByteArray byteArray = convertPixmapToByteArray(screenshot);

//        int part_len = sqrt(byteArray.size());

//        for (int i = part_len; i > 0; i--) {
//            if (byteArray.size() % i == 0) {
//                part_len = i;
//                break;
//            }
//        }

//        int parts_count = byteArray.size()/part_len;

//        for (int i = 0; i < parts_count; i++){
//            SingletonClient::getInstance()->
//                    send_msg_to_server("send_screenshot_to" + SPLIT_SYMBOL + owner.toUtf8() + SPLIT_SYMBOL
//                                       + QByteArray::number(parts_count) + SPLIT_SYMBOL + QByteArray::number(i)
//                                       + SPLIT_SYMBOL + byteArray.mid(i*part_len, part_len));
//        }

        SingletonClient::getInstance()->send_msg_to_server("send_screenshot_to" + SPLIT_SYMBOL + owner.toUtf8() + SPLIT_SYMBOL  + byteArray);

        //qDebug() << byteArray.size();
    }
}

void MainWindow::slot_on_set_scr(QString byteArray) {

    qDebug() << "IM TRY";
    QList<QString> parts = byteArray.split(SPLIT_SYMBOL);
    parts.removeOne(END_SYMBOL);

    if(parts[2] != parts[3]) {
        for (int i = 4; i < parts.size(); i++) {
            picture.append(parts[i].toUtf8());
        }
    }
    else
    {
        ui->picture->setPixmap(convertByteArrayToPixmap(picture).scaled(ui->picture->width(), ui->picture->height(), Qt::KeepAspectRatio));
    }
}

QByteArray MainWindow::convertPixmapToByteArray(const QPixmap& pixmap) {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");  // Можете выбрать другой формат сохранения, например, "JPEG"
    buffer.close();
    return byteArray;
}

QPixmap MainWindow::convertByteArrayToPixmap(const QByteArray& byteArray) {
    QPixmap pixmap;
    pixmap.loadFromData(byteArray);
    return pixmap;
}
