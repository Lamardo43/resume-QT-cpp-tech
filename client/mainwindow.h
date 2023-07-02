#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlistwidget.h"
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMessageBox>
#include <QCloseEvent>

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
    void changeEvent(QEvent*);
    void closeEvent(QCloseEvent*);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void trayActionExecute();
    void setTrayIconActions();
    void showTrayIcon();
    void minimizeToTray();
    void restoreFromTray();

    void slot_on_get_client(QString message);
    void slot_on_get_scr(QByteArray);
    void slot_on_set_scr(QByteArray);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    QByteArray picture = "";
    Ui::MainWindow *ui;
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QHash<QString, QString> clients;
    QList<QString>parts;

    QString  convertPixmapToJSONString(const QPixmap&);
    QPixmap convertJSONStringToPixmap(const QString&);
};

#endif // MAINWINDOW_H
