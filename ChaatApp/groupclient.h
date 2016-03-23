/*#ifndef GROUPCLIENT_H
#define GROUPCLIENT_H

#include <QMainWindow>

namespace Ui {
class GroupClient;
}

class GroupClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit GroupClient(QWidget *parent = 0);
    ~GroupClient();

private:
    Ui::GroupClient *ui;
};

#endif // GROUPCLIENT_H
*/
#ifndef GROUPCLIENT_H
#define GROUPCLIENT_H

#include <QMainWindow>
#include <QString>
#include <QTcpSocket>
#include <QStringListModel>

namespace Ui {
class GroupClient;
}

class GroupClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit GroupClient(QString uName,QString groupName,QWidget *parent = 0);
    ~GroupClient();
    void addToGroupChat(QString groupName,QString senderName,QString message);
    void setTCPSoc(QTcpSocket *tcpSock);
    void addUserList(QString groupName,QStringList sNames);
    void removeUser(QString groupName, QString sNames);
private slots:
    void on_sendButton_clicked();

private:
    Ui::GroupClient *ui;
    QString uName,gName;
    QStringList sName;
    QTcpSocket *pvtSocket;
    QStringListModel *model;

};

#endif // GROUPCLIENT_H
