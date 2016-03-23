#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QWidget>

class QPushButton;
class QTcpSocket;
class QAction;

class ChatClient : public QWidget
{
    Q_OBJECT

public:
    Receiver(QWidget *parent = 0);

private slots:
    void processListening();

private:
    QPushButton *quitButton;
    QTcpSocket *tcpSocket;
};

#endif
#endif // CHATCLIENT_H

