#ifndef PRIVATECLIENT_H
#define PRIVATECLIENT_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QTcpSocket>
#include <QFileDialog>
#include <QImage>
#include <QImageWriter>
#include <QFile>
#include <QRegExp>
#include <QThread>
namespace Ui {
class PrivateClient;
}

class PrivateClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrivateClient(QString uName,QString senderName,QWidget *parent = 0);
    ~PrivateClient();
    //Adding messages in the private clients textedit
    void addToPrivateChat(QString senderName,QString message);
    //getting the socket from main window
    void setTCPSoc(QTcpSocket *tcpSock);
    //sending the file on tcp socket
    void sendFile(QString fileName);

private slots:
    //send button for sending plain messages
    void on_sendButton_clicked();
    //sharing files over tcp
    void on_shareFButton_clicked();

private:
    Ui::PrivateClient *ui;
    //static variable to be used
    QString uName,sName;
    QTcpSocket *pvtSocket,*pvtSocket2;

};

#endif // PRIVATECLIENT_H
