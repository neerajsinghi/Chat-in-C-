#include "privateclient.h"
#include "ui_privateclient.h"
#include "mainwindow.h"

PrivateClient::PrivateClient(QString uName,QString senderName,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PrivateClient)
{
    //starting the ui of private client
    ui->setupUi(this);
    //sentting the user name and sender name
    this->uName =uName;
    this->sName =senderName;
    //setting the color of the chat
    this->setStyleSheet("color: yellow; background-color: lightgrey;");
    //setting the title
    setWindowTitle("Private Chat with "+sName);
    //setting on enter press calling button event
    connect(ui->msgLineEdit, SIGNAL(returnPressed()), this, SLOT(on_sendButton_clicked()));

}

PrivateClient::~PrivateClient()
{
    this->hide();
}
//adding message to private chat window for showing the message
void PrivateClient::addToPrivateChat(QString senderName,QString message) {
    QString sName = this->sName;
    if(senderName.compare(sName) ==0) {
        QTextCursor cursor = ui->privateRoomTextEdit->textCursor();
        QTextBlockFormat chatTextBlockFormat = cursor.blockFormat();
        chatTextBlockFormat.setAlignment(Qt::AlignLeft);//or another alignment
        cursor.mergeBlockFormat(chatTextBlockFormat);
        ui->privateRoomTextEdit->setTextCursor(cursor);

        ui->privateRoomTextEdit->append("<font color=\"darkgreen\"><b>" + senderName + "</b></font>:   " + message);
    }
}
//Sending a private message
void PrivateClient::on_sendButton_clicked()
{
    QString msg =ui->msgLineEdit->text().trimmed();
    if(!msg.isEmpty()) {
        QString message="/PVT:"+this->sName+">"+this->uName+": "+msg;
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
//setting sockets
void PrivateClient::setTCPSoc(QTcpSocket *socket)
{
    this->pvtSocket=socket;
    this->pvtSocket2=socket;
}
//sending files
void PrivateClient::on_shareFButton_clicked()
{   //starting file open dialog box for getting file name
    QString fName =QFileDialog::getOpenFileName(this,tr("Open File"),"/home/neeraj","All files (*.*)");
    //opening the file
    QFile oldFile(fName);
    //spliting the file name
    QStringList nFiles =fName.split("/");
    //starting message box with buttons
    QMessageBox::StandardButton reply;
    //getting the filename without the directory path
    QString objName;
    foreach(QString fileName,nFiles) {
        if(fileName.contains("."))
            objName=fileName;
    }
    //getting files size
    long filesize=oldFile.size();
    //sending message to server and receive to startlistening for the file
    pvtSocket->write(QString("/SENDING_IMG:"+objName+"|"+QString::number(filesize)+">"+this->sName+"<"+this->uName+"\n").toUtf8());
    pvtSocket->flush();
    //message box execute
    reply = QMessageBox::question(this, "Yes", "No",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        //file transfer happen in this function
       sendFile(fName);
      }

    //file sent complete tag
        pvtSocket->write(QString("/IMG_SENT").toUtf8());

}
//File is transffered over tcp
void PrivateClient::sendFile(QString fName)
{
    //initializing a message box
    QMessageBox *msgBox = new QMessageBox(this);
    //checking type of file if it is not a image
    if(!fName.contains("jpg")){
        //opening a file as readonly and text type
        QFile file(fName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                exit(0);
        //Creating a QByteArray for get line content and transferring it over socket
        QByteArray fileline;
        //sending the file over tcp
        while (!file.atEnd()) {
            fileline= file.readAll();
            pvtSocket2->write(fileline);
            fileline.clear();
        }
    }
    //if file is of jpg type
    else if(fName.contains("jpg")){
        //Starting Qimagewriter to write the file over socket
        QImageWriter writer;
        //opening the image file
        QImage image(fName);
        //setting the device to be written on as the private socket
        writer.setDevice(pvtSocket);
        //setting the format of the file
        writer.setFormat("jpg");
        //Send Image to Server using QImageWriter's 'write'  method over the TCPSocket
        writer.write(image);
    }
    //showing the message box
    msgBox->setText("Done");
    msgBox->exec();
    pvtSocket->flush();
}
