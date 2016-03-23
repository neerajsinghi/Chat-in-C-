#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "privateclient.h"
#include "groupclient.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //This is to setup the initial Ui of the application when user starts the application
    ui->setupUi(this);
    //Set the title of the chat window
    setWindowTitle("Chaat Client");
    // Instantiate our socket (but don't actually connect to anything
    // yet until the user clicks the loginButton:
    socket =new QTcpSocket(this);

    //Showing the login page on startup of the application
    ui->stackedWidget->setCurrentWidget(ui->loginPage);

    //Checking how many same messages user has sent in the broadcast
    //to check if user is a spammer
    counter=0;
    //To check if we are receiving any file
    //Initally set to geting plain data
    flag=0;
    //Checking what is the size of file received
    receivedSize=0;
    //Initializing the file name
    fileName="out.jpg";
    //Initializing the prevMsg a string to
    //save the last message sent by the user
    prevMsg="";
    //Initializing the model to setup QListView
    model = new QStringListModel(this);
    //Declairing a list to be set in the model
    QStringList list;
    //Setting the list in the model as when it is delaired it is initialized as emptty
    model->setStringList(list);
    //Setting the model in uiListView element which is a list view
    ui->userListView->setModel(model);
    //Setting the selection model
    ui->userListView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    //Disabling the edit function in the listview
    ui->userListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Setting the context menu policy for getting a menu on right click in the list view
    ui->userListView->setContextMenuPolicy(Qt::CustomContextMenu);
    //Using this we call and show the context menu of the list view
    //We can then click on the context menu to perform different functions
    connect(ui->userListView,SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenuForGroupChat(const QPoint &)));
    // This is how we tell Qt to call our readyRead() and connected()
    // functions when the socket has text ready to be read, and is done
    // connecting to the server (respectively):
    connect(socket,SIGNAL(readyRead()), this,SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    //Setting enter key press as a signal for running on_*_clicked funstions
    connect(ui->sayLineEdit, SIGNAL(returnPressed()), this, SLOT(on_sendButton_clicked()));
    connect(ui->uNameLineEdit, SIGNAL(returnPressed()), this, SLOT(on_Connect_clicked()));
}
//Destructor of the application
MainWindow::~MainWindow()
{
    //Delete the ui
    delete ui;
}
void MainWindow::on_Connect_clicked()
{
    //Checking if user has logged in with Name Admin
    if(ui->uNameLineEdit->text().compare("Admin")==0)
    {
        //Initializing Dialog box
        QDialog dialog(this);
        // Use a layout allowing to have a label next to each field
        QFormLayout form(&dialog);
        //Dinamically creating a line edit to get passwordd from the user
        QLineEdit *uPasswordLineEdit = new QLineEdit(&dialog);
        //Setting the echomode to password so it appears as *** when user is typing it
        uPasswordLineEdit->setEchoMode(QLineEdit::Password);
        //Creating a lable as "Password ?"
        QString label = "Password ?";
        // Add some text above the fields
        form.addRow(label, uPasswordLineEdit);
        //Creating QDialogbutton box to add in the dialog box
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                   Qt::Horizontal, &dialog);
        //Adding the button
        form.addRow(&buttonBox);
        //Declairing of event when the buttons are clicked
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
        //If user click on accepted
        if(dialog.exec() == QDialog::Accepted) {
            //If password is correct
            if(uPasswordLineEdit->text().compare("IAMADMIN")==0) {
                // Start connecting to the chat server (on port 9870).
                // This returns immediately and then works on connecting
                // to the server in the background. When it's done, we'll
                // get a connected() function call (below). If it fails,
                // we won't get any error message because we didn't connect()
                // to the error() signal from this socket.
                socket->connectToHost(ui->sAddressLineEdit->text(), 9870);

            }
            //If password is incorrect
            else {
                //Creating a messagebox with text "Wrong password"
                QMessageBox *msgBox = new QMessageBox(this);
                msgBox->setText("Wrong Password");
                //Executing the message box
                msgBox->exec();
            }
        }
    } else {
        // Start connecting to the chat server (on port 4200).
        // This returns immediately and then works on connecting
        // to the server in the background. When it's done, we'll
        // get a connected() function call (below). If it fails,
        // we won't get any error message because we didn't connect()
        // to the error() signal from this socket.
        socket->connectToHost(ui->sAddressLineEdit->text(), 9870);
    }
}
//This function listen to the socket for any incomming data
//and is called when there is data in the socket.
void MainWindow::readyRead()
{
    //Checking if flag is 0 means there is no file sharing
    if(flag==0) {
        //Reading all the data from the socket
        QString msg=QString::fromUtf8(socket->readAll().trimmed());
        //Creating a regex to check the pattern "^/SENDING_IMG:(.*)|(.*)>(.*)<(.*)$"
        QRegExp photoRegex("^/SENDING_IMG:(.*)|(.*)>(.*)<(.*)$");
        //if there is something matching the pattern
        if(photoRegex.indexIn(msg)!=-1) {
            //setting up delimeters to get the sub string of the string
            string delimiter1 = ":";
            string delimiter2 = "|";
            string message=msg.toStdString();
            //geting and setting the filename
            fileName = QString::fromStdString(message.substr(message.find(delimiter1)+1,message.find(delimiter2)-message.find(delimiter1)-1));
            delimiter1 = "|";
            delimiter2 = ">";
            //setting the size of the file
            fileSize=QString::fromStdString(message.substr(message.find(delimiter1)+1,message.find(delimiter2)-message.find(delimiter1)-1));
            delimiter1 = ">";
            delimiter2 = "<";
            // name of the receiver
            QString uName=QString::fromStdString(message.substr(message.find(delimiter1)+1,message.find(delimiter2)-message.find(delimiter1)-1));
            delimiter1 = "<";
            delimiter2 = "\0";
            //name of the sender
            QString sendersName=QString::fromStdString( message.substr(message.find(delimiter1)+1,message.find(delimiter2)));
            //check if receiver name is equal to the name of the client currently using
            if(uName.compare(userName)==0) {
                //setting the flag to one and dissabling his functions
                flag =1;
                ui->sayLineEdit->blockSignals(true);
                ui->userListView->blockSignals(true);
                ui->sendButton->blockSignals(true);
            }
        }
        //if there is nothing matching the pattern
        else
            //a funtion to parse the incoming message
            parseProtocol(msg);
    }
    //if user is set for receiving a file
    else if(flag==1) {
        //Setting all the data received in a QByteArray
        QByteArray data(socket->readAll());
        //Creating a file with file name
        QFile file(fileName);
        //intializing qint64 bytes
        quint64 bytes = 0;
        //opening the file for writing or appending
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        //Checking if file doesn't containts the end tag
        if(!data.contains("/IMG_SENT"))
            while (bytes < data.size())
                //Writing the data in the file
                bytes += file.write(data);
        //Checking what size of file we received yet
        receivedSize+=data.size();

        //if data contain the end tag
        if(data.contains("/IMG_SENT")) {
            //checking if the file if of type jpg
            if(fileName.contains("jpg")) {
                //if Yes  creating image file with name of the image file
                QImage image(fileName);
                //Creating QPalette
                QPalette palette;
                //setting the image
                palette.setBrush(QPalette::Background,QBrush(image));
                //Starting a new widget
                QWidget* widget = new QWidget();
                //setting the title of the widget to filename
                widget->setWindowTitle(fileName);
                //seting palette in the widget
                widget->setPalette(palette);
                //Showing the widget
                widget->show();
            }
            //Closing the file
            file.close();
            //Notifying the user he have received a file and enabling his functionalities
            ui->chatRoomTextEdit->append("File Received");
            ui->sayLineEdit->blockSignals(false);
            ui->userListView->blockSignals(false);
            ui->sendButton->blockSignals(true);
            //setting the flag to 0 again
            flag =0;
        }
    }
}
//Parsing the message
void MainWindow::parseProtocol(QString msg) {
    //Creating regex for different patterns
    QRegExp usrRegex("^/USERS:(.*)$");
    QRegExp messageRegex("^/MSG:(.*):(.*)$");
    QRegExp pvtRegex("^/PVT:(.*)>(.*):(.*)$");
    QRegExp grpRegex("^/GRP:(.*)>(.*):(.*)$");
    QRegExp grpStartRegex("^/GRPS:(.*)>(.*)");
    QRegExp grpCloseRegex("^/GRPSTOP:(.*)>(.*)");
    QRegExp blkRegex("^/BLOCK:(.*)");
    QRegExp unblkRegex("^/UNBLOCK:(.*)");
    QRegExp kickRegex("^/KICKOUT:(.*)");
    QRegExp getRegex("^/GETLIST:(.*)");
    QRegExp usrListRegex("^/USERLISTS:(.*)$");
    QRegExp msgPvtListRegex("^/MSGPVT:(.*):(.*):(.*)$");
    //If user if getting a user list
    if(usrRegex.indexIn(msg)!=-1) {
        //Getting list of users
        QStringList users = usrRegex.cap(1).split(",");
        //looping through the usernames
        foreach(QString usr, users) {
            if(usr.trimmed().length()>0) {
                //inserting the name in userlist
                model->insertRow(model->rowCount());
                QModelIndex index = model->index(model->rowCount()-1);
                model->setData(index,usr);
                ui->userListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            }
        }
    }
    //If user is getting a message when user is talking to another user
    else if(msgPvtListRegex.indexIn(msg)!=-1) {
        //getting sender name
        QString sName =  msgPvtListRegex.cap(1);
        //getting message
        QString message =  msgPvtListRegex.cap(2);
        //getting receiver name
        QString uName =  msgPvtListRegex.cap(3);
        //checking if current user is not the sender
        if((this->userName.compare(sName)!=0)) {
            QTextCursor cursor = ui->chatRoomTextEdit->textCursor();
            QTextBlockFormat chatTextBlockFormat = cursor.blockFormat();
            chatTextBlockFormat.setAlignment(Qt::AlignLeft);//or another alignment
            cursor.mergeBlockFormat(chatTextBlockFormat);
            ui->chatRoomTextEdit->setTextCursor(cursor);
            ui->chatRoomTextEdit->append("<font color=\"darkgreen\"><b>"+sName+" @"+uName+"</b></font>: "+message);
        }
    }
    //checking if user is getting a private message
    else if(pvtRegex.indexIn(msg)!=-1) {
        //getting username sendername and message
        QString uName =  pvtRegex.cap(1);
        QString sName =  pvtRegex.cap(2);
        QString message =  pvtRegex.cap(3);
        //funtion for parsing the private message
        parsePvtProtocol(uName,sName,message);
    }
    //checking if user is getting a group message
    else if(grpRegex.indexIn(msg)!=-1) {
        //getting group name , senders name , message
        QString groupName =  grpRegex.cap(1);
        QString sName =  grpRegex.cap(2);
        QString message =  grpRegex.cap(3);
        //funtion for parsing the group message
        parseGrpProtocol(groupName,sName,message);
    }
    //Checking if message contains a group start request
    else if(grpStartRegex.indexIn(msg)!=-1) {
        //getting group name and list of users in group
        QString groupName =  grpStartRegex.cap(1);
        QStringList sNames =  grpStartRegex.cap(2).split(",");
        // looping through the list of users
        foreach(QString sName,sNames) {
            //Checking if the list contains the current user name
            if(sName.compare(this->userName)==0) {
                //Check if you created the group or you are already in the group
                if(!grpClient.contains(grName)) {
                    //adding new group name in a lis of groups
                    grpNames.append(groupName);
                    //intializing pointer object to the group client class
                    gp = new GroupClient(this->userName,groupName,this);
                    //inserting the groupName and qbject in the map
                    grpClient.insert(groupName,gp);
                    //getting the value of group name from the map
                    gpr=grpClient.value(groupName);
                    //setting the tcp socket for the group
                    gpr->setTCPSoc(this->socket);
                    //Add the list of users
                    gpr->addUserList(groupName,sNames);
                    //Showing the group chat client
                    gpr->show();
                }
            }
        }
    }
    //If message contains NEW_USER
    else if(msg.contains("NEW_USER")) {
        //getting user's name
        QString newUser =  msg.mid(msg.toStdString().find(":")+1,-1);
        ui->chatRoomTextEdit->append("<b>"+newUser +" Connected</b>");
        //inserting user in the list
        model->insertRow(model->rowCount());
        QModelIndex index = model->index(model->rowCount()-1);
        model->setData(index,newUser);

    }
    //check if user has left
    else if(msg.contains("GONE")) {
        //getting user name
        QString closeUser =  msg.mid(0,msg.toStdString().find("/"));
        //find the user's index from the list using the user's name
        QModelIndexList row=model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(closeUser), -1, Qt::MatchStartsWith);
        foreach (QModelIndex indexes, row) {
            //removing the user
            model->removeRow(indexes.row());
        }
        //checking if user is in private session with the user who left
        if(pvtClient.contains(closeUser)) {
            npr=pvtClient.value(closeUser);
            npr->close();
        }
        QTextCursor cursor = ui->chatRoomTextEdit->textCursor();
        QTextBlockFormat chatTextBlockFormat = cursor.blockFormat();
        chatTextBlockFormat.setAlignment(Qt::AlignLeft);//or another alignment
        cursor.mergeBlockFormat(chatTextBlockFormat);
        ui->chatRoomTextEdit->setTextCursor(cursor);
        ui->chatRoomTextEdit->append("-------------- <font color=\"darkgreen\"><b>"+closeUser+"</b></font> left the chat --------------- ");
    }
    //if the message is a normal message
    else if(messageRegex.indexIn(msg)!=-1) {
        //getting the name and message
        QString uName = messageRegex.cap(1);
        QString uMessage = messageRegex.cap(2);
        //setting the message on text edit
        QTextCursor cursor = ui->chatRoomTextEdit->textCursor();
        QTextBlockFormat chatTextBlockFormat = cursor.blockFormat();
        chatTextBlockFormat.setAlignment(Qt::AlignLeft);//or another alignment
        cursor.mergeBlockFormat(chatTextBlockFormat);
        ui->chatRoomTextEdit->setTextCursor(cursor);
        ui->chatRoomTextEdit->append("<font color=\"darkgreen\"><b>"+uName+"</b></font>: "+uMessage);
    }
    //group user removing message
    else if(grpCloseRegex.indexIn(msg)!=-1) {
        //getting the group name and username
        QString grpName = grpCloseRegex.cap(1);
        QString userName = grpCloseRegex.cap(2);
        gpr=grpClient.value(grpName);
        //removing the user
        gpr->removeUser(grpName,userName);
    }
    //Getting a block message
    else if(blkRegex.indexIn(msg)!=-1) {
        int blocked=0;
        //getting the list of blocked people
        QStringList bName = blkRegex.cap(1).split(",");
        foreach(QString user,bName) {
            //if this user to be blocked
            if(user.compare(this->userName)==0) {
                //blocking user
                ui->sayLineEdit->blockSignals(true);
                ui->userListView->blockSignals(true);
                ui->sendButton->blockSignals(true);
                ui->chatRoomTextEdit->blockSignals(true);
                blocked=1;

            }
        }
        //if this user isnot to be blocked
        if(blocked==0) {
            foreach(QString user,bName) {
                //removing the user from the list
                ui->userListView->selectAll();
                QModelIndexList row=model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(user), -1, Qt::MatchStartsWith);
                foreach (QModelIndex indexes, row) {
                    model->removeRow(indexes.row());
                }//hiding private chat
                npr=pvtClient.value(user);
                npr->hide();
            }

        }
    }
    //if user gets a unblock message
    else if(unblkRegex.indexIn(msg)!=-1) {
        //Getting list of users to be unblocked
        QStringList bName = unblkRegex.cap(1).split(",");
        foreach(QString user,bName) {
            //checking if current user is one of the users
            if(user.compare(this->userName)==0) {
                ui->sayLineEdit->blockSignals(false);
                ui->userListView->blockSignals(false);
                ui->sendButton->blockSignals(false);
                ui->chatRoomTextEdit->blockSignals(true);

            }
            //if not adding the user to the user list
            else if(user.compare(this->userName)!=0) {
                model->insertRow(model->rowCount());
                QModelIndex index = model->index(model->rowCount()-1);
                model->setData(index,user);
            }
        }

    }
    //if user receive a kickout message
    else if(kickRegex.indexIn(msg)!=-1) {
        QStringList bName = kickRegex.cap(1).split(",");
        foreach(QString user,bName) {
            //if current user is in the list we close the socket
            if(user.compare(this->userName)==0) {
                socket->close();
                ui->stackedWidget->setCurrentWidget(ui->loginPage);
            }
        }
    }
    //if user receive a getlist message
    else if(getRegex.indexIn(msg)!=-1) {
        //getting the name of users
        QStringList bName = getRegex.cap(1).split(",");
        foreach(QString user,bName) {
            //checking if current user is in the list
            if(user.compare(this->userName)==0) {
                QString uNameList;
                //extracting and sending the names from the file
                QFile file("UserNames.txt");
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    exit(0);
                QByteArray line;
                while (!file.atEnd()) {
                    uNameList=uNameList+ file.readLine()+",";
                }
                uNameList=uNameList.remove(uNameList.length()-1,1);
                socket->write(QString("/USERLISTS:"+uNameList).toUtf8());
            }

        }
    }
    //if the user is admin user and got the list of a user
    else if(usrListRegex.indexIn(msg)!=-1 && (this->userName.compare("Admin")==0)) {
        //showing the list of user's name in a list
        QStringList bName = usrListRegex.cap(1).split(",");
        QDialog dialog(this);
        // Use a layout allowing to have a label next to each field
        QFormLayout form(&dialog);
        foreach(QString uName,bName) {
            form.addRow(new QLabel(uName + "                                     "));
            dialog.QDialog::setWindowTitle("Users name used by "+uName);
        }
        dialog.exec();

    }

}
//Parsing private messages
void MainWindow::parsePvtProtocol(QString uName,QString sName,QString message) {
    //checking if username compairs to the current username
    if(uName.compare(this->userName)==0) {
        //checking if the senders name is in the private client map
        //if no add it to the map and show the client with the message
        if(!pvtClient.contains(sName)) {
            pr = new PrivateClient(uName,sName,this);
            pvtClient.insert(sName,pr);
            npr=pvtClient.value(sName);
            npr->setTCPSoc(this->socket);
            npr->show();
            npr->addToPrivateChat(sName,message);

        }
        //if yes just showing the message
        else {
            npr=pvtClient.value(sName);
            npr->setTCPSoc(this->socket);
            npr->show();
            npr=pvtClient.value(sName);
            npr->focusWidget();
            npr->addToPrivateChat(sName,message);
        }
    }
}
//Parsing group messages
void MainWindow::parseGrpProtocol(QString groupName,QString sName,QString message) {
    foreach(QString gName,grpNames) {
        //if group name is present in the list of group name of this user show the message
        if(gName.compare(groupName)==0) {
            gpr=grpClient.value(groupName);
            gpr->setTCPSoc(this->socket);
            gpr=grpClient.value(groupName);
            gpr->focusWidget();
            gpr->addToGroupChat(groupName,sName,message);
            gpr->show();
        }
    }
}

// This function gets called when our socket has successfully connected to the chat
// server. (see the connect() call in the MainWindow constructor).
void MainWindow::connected()
{
    // Flip over to the chat page:
    //  stackedWidget->setCurrentWidget(chatPage);
    ui->stackedWidget->setCurrentWidget(ui->chatPage);
    // And send our username to the chat server.
    QString name=ui->uNameLineEdit->text();
    socket->write(QString(name).toUtf8());

    //  socket->write(QString(password).toUtf8());
    userName = ui->uNameLineEdit->text();
    //Creating a file to maintain as a cookie to get all the names used by client on the same system
    QFile file("UserNames.txt");
    QByteArray data(QString(userName+"\n").toUtf8());
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    file.write(data);
    file.close();
}


//When user clicks on send button
void MainWindow::on_sendButton_clicked()
{
    //Getting and setting the message from saylineedit
    QString msg =ui->sayLineEdit->text().trimmed();
    if(!msg.isEmpty()) {
        //check the type of user as admin
        if(userName.compare("Admin")==0) {
            //Check if message contain /KICKOUT keyword
            if(msg.contains("/KICKOUT")) {
                //getting the user id fom the message
                msg= QString::fromStdString(msg.toStdString().substr(msg.toStdString().find(":")+1,msg.toStdString().find("\0")-msg.toStdString().find(":")+1));
                //selecting all the users in listview
                ui->userListView->selectAll();
                //getting index of all the selected users
                QModelIndexList indexes=ui->userListView->selectionModel()->selectedIndexes();
                int i=1;
                QString usName;
                //Getting the name of the user at id
                foreach(QModelIndex index,indexes) {
                    if(i==msg.toInt()) {
                        usName=index.data(Qt::DisplayRole).toString();
                        break;
                    } else
                        i++;
                }
                ui->chatRoomTextEdit->append("<font color=\"red\"><b> /KICKOUT </b></font>: " + usName);
                QString mess="/KICKOUT:" + usName;
                //sending the message to the server
                socket->write(QString(mess).toUtf8());
            }
            //Check if message contain /GETLIST keyword
            else if(msg.contains("/GETLIST")) {
                //getting the user id fom the message
                msg= QString::fromStdString(msg.toStdString().substr(msg.toStdString().find(":")+1,msg.toStdString().find("\0")-msg.toStdString().find(":")+1));
                //selecting all the users in listview
                ui->userListView->selectAll();
                //getting index of all the selected users
                QModelIndexList indexes=ui->userListView->selectionModel()->selectedRows(0);
                int i=1;
                QString usName;
                //Getting the name of the user at id
                foreach(QModelIndex index,indexes) {
                    if(i==msg.toInt()) {
                        usName=index.data(Qt::DisplayRole).toString();
                        break;
                    } else
                        i++;
                }
                ui->chatRoomTextEdit->append("<font color=\"red\"><b> /GETLIST </b></font>: " + usName);
                QString mess="/GETLIST:" + usName;
                //sending the message to the server
                socket->write(QString(mess).toUtf8());
            }
            //If there is no keywords
            else if(!msg.contains("/")) {
                QString message="/MSG:"+userName+": "+msg;
                //sending the message to the server
                socket->write(QString(message).toUtf8());
                // Changing the cursor position
                QTextCursor cursor = ui->chatRoomTextEdit->textCursor();
                QTextBlockFormat chatTextBlockFormat = cursor.blockFormat();
                chatTextBlockFormat.setAlignment(Qt::AlignRight);//or another alignment
                cursor.mergeBlockFormat(chatTextBlockFormat);
                ui->chatRoomTextEdit->setTextCursor(cursor);
                ui->chatRoomTextEdit->append("<font color=\"red\"><b> me </b></font>: " + msg);



            }
        } else {
            //if the message contains /
            if(msg.contains("/")) {
                QString userNo= QString::fromStdString(msg.toStdString().substr(msg.toStdString().find("/")+1,msg.toStdString().find(":")-msg.toStdString().find("/")-1));
                msg= QString::fromStdString(msg.toStdString().substr(msg.toStdString().find(":")+1,msg.toStdString().find("\0")-msg.toStdString().find(":")-2));
                ui->userListView->selectAll();
                QModelIndexList indexes=ui->userListView->selectionModel()->selectedRows(0);
                int i=1;
                QString usName;
                foreach(QModelIndex index,indexes) {
                    if(i==userNo.toInt()) {
                        usName=index.data(Qt::DisplayRole).toString();
                        break;
                    } else
                        i++;
                }
                socket->write(QString("/MSGPVT:"+this->userName+":"+msg+":"+usName).toUtf8());
                ui->chatRoomTextEdit->append("<font color=\"red\"><b> me </b></font>: " + msg+" to "+usName);
            } else if(!msg.contains("/")) {
                QString message="/MSG:"+userName+": "+msg;
                socket->write(QString(message).toUtf8());
                QTextCursor cursor = ui->chatRoomTextEdit->textCursor();
                QTextBlockFormat chatTextBlockFormat = cursor.blockFormat();
                chatTextBlockFormat.setAlignment(Qt::AlignRight);//or another alignment
                cursor.mergeBlockFormat(chatTextBlockFormat);
                ui->chatRoomTextEdit->setTextCursor(cursor);
                //ui->roomTextEdit->setAlignment(Qt::AlignRight);
                ui->chatRoomTextEdit->append("<font color=\"red\"><b> me </b></font>: " + msg);
                //  ui->roomTextEdit->setAlignment(Qt::AlignLeft);
                if(prevMsg.compare(msg)==0   ) {
                    counter++;
                    if(counter==20) {
                        ui->sayLineEdit->blockSignals(true);
                        ui->userListView->blockSignals(true);
                        ui->sendButton->blockSignals(true);
                    }
                }
                else {
                    counter=0;
                }
                prevMsg=msg;

            }
        }
    }
    ui->sayLineEdit->clear();
    ui->sayLineEdit->setFocus();
}

//Starting a private chat session
void MainWindow::on_userListView_doubleClicked(const QModelIndex &index)
{
    QString qs=index.data(Qt::DisplayRole).toString();
    pr = new PrivateClient(this->userName,qs,this);
    pvtClient.insert(qs,pr);
    npr=pvtClient.value(qs);
    npr->setTCPSoc(this->socket);
    npr->show();

}
//Stopping a private session
void MainWindow::deleteFromMap(QString userName) {
    pvtClient.remove(userName);
}

//Right click menu for user and admin
void MainWindow::showContextMenuForGroupChat(const QPoint &pos) {
    //Intializing Qmenu
    QMenu *grpMenu = new QMenu;
    QStringList qs;
    QString groupList="";
    //Checking if user is admin
    if(userName.compare("Admin")==0) {
        QModelIndex indexs = ui->userListView->indexAt(pos);
        //If the index pos contains Blocked
        if(indexs.data(Qt::DisplayRole).toString().contains("Blocked")) {
            //Group menu action is unblock
            grpMenu->addAction(tr("Unblock"), this, SLOT(test_slot()));
            //starting the action
            QAction* startAction=grpMenu->exec(QCursor::pos());
            //unblocking the user
            if(startAction) {
                QModelIndexList indexes=ui->userListView->selectionModel()->selectedIndexes();
                foreach(QModelIndex index,indexes) {
                    if(index.data(Qt::DisplayRole).toString().contains("Blocked")) {
                        QString uName=index.data(Qt::DisplayRole).toString().remove("(Blocked)");
                        model->setData(index,uName);
                        groupList+=uName+",";
                    }
                }
                //sending the message to all users to unblock the user
                socket->write(QString("/UNBLOCK:"+groupList).toUtf8());
            }
        }
        else {
            //Action to block the users
            grpMenu->addAction(tr("Block"), this, SLOT(test_slot()));
            QAction* startAction=grpMenu->exec(QCursor::pos());
            if(startAction) {
                QModelIndexList indexes=ui->userListView->selectionModel()->selectedIndexes();
                foreach(QModelIndex index,indexes) {
                    if(!index.data(Qt::DisplayRole).toString().contains("Blocked")) {
                        groupList+=index.data(Qt::DisplayRole).toString()+",";

                        model->setData(index,index.data(Qt::DisplayRole).toString()+"(Blocked)");
                    }
                }
                //sending message to all the user to block the user
                socket->write(QString("/BLOCK:"+groupList).toUtf8());
            }
        }

    }
    else {
        //start group chat action
        grpMenu->addAction(tr("Start GroupChat"), this, SLOT(test_slot()));
        QAction* startAction=grpMenu->exec(QCursor::pos());
        if(startAction) {
            QModelIndexList indexes=ui->userListView->selectionModel()->selectedIndexes();
            foreach(QModelIndex index,indexes) {
                qs.append(index.data(Qt::DisplayRole).toString());
                groupList+=index.data(Qt::DisplayRole).toString()+",";
            }
            //creating group name using Quuid
            //start the group chat
            grName=QUuid::createUuid();
            //adding the name of the group in a list
            grpNames.append(grName);
            //appending the current user name in the group
            qs.append(this->userName);
            //sending all group member indication that a group has been started
            socket->write(QString("/GRPS:"+grName+">"+groupList+this->userName).toUtf8());
            //Starting and adding a group object in  a map
            gp = new GroupClient(this->userName,grName,this);
            grpClient.insert(grName,gp);
            gpr=grpClient.value(grName);
            //setting the tcpsocket for the group
            gpr->setTCPSoc(this->socket);
            //starting the client
            gpr->show();
            //adding users to the client
            gp->addUserList(grName,qs);

        }
    }

}
//When user close the window
void MainWindow::closeEvent (QCloseEvent* event)
{
    event->ignore();
    //send all groups message that this user is leaving
    foreach(QString grpName,grpNames) {
        QString message="/GRPSTOP:"+grpName+">"+this->userName;
        socket->write(QString(message).toUtf8());
    }
    socket->close();
    event->accept();
}
