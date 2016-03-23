#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QString>
#include <string>
#include <QStringListModel>
#include <QMessageBox>
#include <QDebug>
#include "privateclient.h"
#include <QMap>
#include <QMenu>
#include <QUuid>
#include <QCloseEvent>
#include <QImage>
#include <Q_INT64>
#include <QDialogButtonBox>
#include "groupclient.h"
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //Delete the name from the map of private users
    void deleteFromMap(QString userName);
    //parsing all the plain text messages
    void parseProtocol(QString msg);
    //parsing all the private messages
    void parsePvtProtocol(QString uName,QString sName,QString msg);
    //parsing all the group messages
    void parseGrpProtocol(QString groupName,QString sName,QString msg);

private slots:
    //when user click on connect button
    void on_Connect_clicked();
    //ready read event
    void readyRead();
    // This function gets called when the socket tells us it's connected.
    void connected();
    //when user click on send button
    void on_sendButton_clicked();
    //when user close the window
    void closeEvent(QCloseEvent* event);
    //when user list view is double clicke
    void on_userListView_doubleClicked(const QModelIndex &index);
    // when user right click on the userlist view
    void showContextMenuForGroupChat(const QPoint &pos);

private:
    //ui class object
    Ui::MainWindow *ui;
    //Private Client cloass objects
    PrivateClient *pr,*npr;
    //Group client class objects
    GroupClient *gp,*gpr;
    //Map between private client object and the receiver/sender
    QMap<QString,PrivateClient*> pvtClient;
    //Map between group client object and the group name
    QMap<QString,GroupClient*> grpClient;
    // This is the socket that will let us communitate with the server.
    QTcpSocket *socket;
    //List view model
    QStringListModel *model;
    //static variable to be used in the application
    QString userName;
    QStringList grpNames;
    QString grName;
    QString prevMsg;
    int counter;
    int flag;
    QString fileName;
    QString fileSize;
    long receivedSize;

};

#endif // MAINWINDOW_H
