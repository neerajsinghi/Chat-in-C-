
#include "groupclient.h"
#include "ui_groupclient.h"
#include "mainwindow.h"
GroupClient::GroupClient(QString uName,QString groupName,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GroupClient)
{
    //Setting up groupclient ui
    ui->setupUi(this);
    //setting the username and group name
    this->uName =uName;
    this->gName =groupName;
    //setting the title
    setWindowTitle("Group Chat...");
    //setting the enter press event
    connect(ui->msgLineEdit, SIGNAL(returnPressed()), this, SLOT(on_sendButton_clicked()));

}

GroupClient::~GroupClient()
{
    this->hide();
}
void GroupClient::addToGroupChat(QString groupName,QString sName,QString message) {
    //setting the group name
    QString gName = this->gName;
    //comparing the group name and the current user name
    if((groupName.compare(gName) ==0) && (sName.compare(uName)!=0)) {
        //Setting the message on the text edit window
        QTextCursor cursor = ui->privateRoomTextEdit->textCursor();
        QTextBlockFormat chatTextBlockFormat = cursor.blockFormat();
        chatTextBlockFormat.setAlignment(Qt::AlignLeft);//or another alignment
        cursor.mergeBlockFormat(chatTextBlockFormat);
        ui->privateRoomTextEdit->setTextCursor(cursor);

        ui->privateRoomTextEdit->append("<font color=\"darkblue\"><b>" + sName + "</b></font>:   " + message);
    }
}
//sending the message
void GroupClient::on_sendButton_clicked()
{
    QString msg =ui->msgLineEdit->text().trimmed();
    //checking if message is not empty
    if(!msg.isEmpty()) {
        //writing the message on the socket and text edit
        QString message="/GRP:"+gName+">"+this->uName+": "+msg;
        pvtSocket->write(QString(message).toUtf8());
        pvtSocket->socketDescriptor();
        QTextCursor cursor = ui->privateRoomTextEdit->textCursor();
        QTextBlockFormat chatTextBlockFormat = cursor.blockFormat();
        chatTextBlockFormat.setAlignment(Qt::AlignRight);//or another alignment
        cursor.mergeBlockFormat(chatTextBlockFormat);
        ui->privateRoomTextEdit->setTextCursor(cursor);
        ui->privateRoomTextEdit->append("<font color=\"red\"><b> me </b></font>: " + msg);
    }
    ui->msgLineEdit->clear();
    ui->msgLineEdit->setFocus();
}
//Adding new users in the user list
void GroupClient::addUserList(QString groupName, QStringList sNames) {
    this->gName=groupName;
    model = new QStringListModel(this);
    this->sName=sNames;
    QStringList list =sNames;
    model->setStringList(list);
    ui->userListView->setModel(model);
    ui->userListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->userListView->setStyleSheet("color: white; background-color: blue;");
}
//removing user from the user list
void GroupClient::removeUser(QString groupName, QString sNames) {
    if(groupName.compare(gName)==0){
        QModelIndexList row=model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(sNames), -1, Qt::MatchStartsWith);
        foreach (QModelIndex indexes, row) {
            model->removeRows(indexes.row(), 1);
        }
    }
}
//setting the socket
void GroupClient::setTCPSoc(QTcpSocket *socket)
{
    this->pvtSocket=socket;
}
