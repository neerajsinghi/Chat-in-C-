#include<sys/socket.h>
#include<string>
//#include "process_db.h"
#include<netinet/in.h>
#include<arpa/inet.h>
#include<iostream>
#include<pthread.h>
#include "prereq.h"
#include "info.h"

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
/*structure to hold address and socket descriptor of Client*/
using namespace std;
struct pass{
    int fd;
    sockaddr_in cl_addr;
};


struct argu{
    int x;
    string pass_w;
};
/*Modified Send to display any error also*/
int Send(int fd,const char *st,int size,int flag){
    int l;
    while(f);
    l=send(fd,st,size,flag);
    if(l<=0){
        cout<<"Major Blunder of SEND\n";
    }
    return l;
}
/*Modified recieve to display any error*/
int Recv(int fd,char *st, int size,int flag){
    int l;
    l = recv(fd,st,size,flag);
    if(l<=0)
        cout<<"Major Blunder of RECV\n";
    return l;
}


/* Function which execute in new thread for new client connection*/

void *process_conn(void *new_sock){
           int length;
            int x;
            struct classification cls;
            struct argu ar;

           map<string,int>::iterator it;
/*to Keep the info in data structure*/
           struct pass *cl;
           cl = (struct pass*)new_sock;
           cout<<"ID of new user is "<<cl->fd<<endl;
           //Buffers for normal messages and Image transfer
           char msg[1024];
           char img[1024];
            //First Message which is name of the user
           length = Recv(cl->fd,msg,1024,0);
            //Display of the message
           cout<<"USER CAME: "<<msg<<endl;     //function that gives the name of the person

           cout<<"NAME IS "<<msg<<endl;
            //storing username of New user
           string user_name(msg,strlen(msg));
/* This whole block is for offline message keeping and has to be moved to processdb.h*/
 /*          Recv(cl->fd,msg,1024,0);
           ar.pass_w = string(msg);
           ar.x = 0;
           Recv(cl->fd,msg,1024,0);
           string pass_w(msg,strlen(msg));
           struct arg_db arg_th;
           arg_th.userName = user_name; arg_th.passW = pass_w;
           pthread_t d_th;
           pthread_create(&d_th,NULL,process_db,arg_th);
           pthread_detach(th);
        //   *pass user_name and string
           //Check in server
           string query_check("select * from USERS where username='");
           query_check.append(user_name); query_check.append("')");
           sql_exec(db,query_check.c_str(),&cb_check,&ar);

           if(x == 4){
                string query_ins("insert into USERS values('");
                query_ins.append(user_name); query_ins.append("','");
                query_ins.append(pass_w); query_ins.append("')");
                sql_exec(db,query_ins.c_str(),NULL,NULL);
            }
            if(x==2){
                send(cl->fd,"NO MATCH",8,0);
                pthread_exit(NULL);
                close(cl->fd);
            }
           
           string query_msg("select * from MESSAGE where username='");
           query_msg.append(user_name); query_msg.append("')");
           sql_exec(db,query_msg.c_str(),&cb_msg,&cl->fd);
           cout<<user_name<<endl;
           string query_del("delete from MESSAGE where username='");
           query_del.append(user_name); query_del.append("')");
           sql_exec(db,query_del.c_str(),NULL,NULL);
           //all sql do */
           
           string new_user;
           new_user.append("/NEW_USER:");
           new_user.append(msg);

           
           //Sending New User details to everyone
           for(it=record.begin();it!=record.end();it++)
                Send(it->second,new_user.c_str(),new_user.size(),0);
           bzero(msg,sizeof(msg));
           string s; s.append("/USERS:");
           //Sending list of users to new user
           cout<<"Sending user Details"<<endl;
           for(it=record.begin();it!=record.end();it++){
                s.append((it->first).c_str());
                cout<<it->first<<endl;
                s.append(",");
           }

            Send(cl->fd,s.c_str(),s.size(),0);    //Illegal to write on broken connection
            cout<<"Sent user details "<<s<<endl;
           //sent the list of users to everybody
          if(record.find(user_name) == record.end()){ 
                record.insert(pair<string,int>(string(user_name),cl->fd));
                cout<<"New Entry"<<user_name<<" id"<<cl->fd<<endl;
          }else{
              cout<<"Can't have same ip again"<<endl;
              close(cl->fd);
              pthread_exit(NULL);
          }

           bzero(msg,sizeof(msg));
           
            
           
           cout<<"Printing the list "<<endl;
           for(it=record.begin();it!=record.end();it++)
                cout<<it->first<<it->second<<endl;
           
           cout<<"Printed the list"<<endl;
            

          bzero(msg,sizeof(msg));
          string sp="";    int sp_t=0;
           while(1){
//Recive message
                length = Recv(cl->fd,msg,1024,0);
                if(length <= 0){
                    cout<<length<<" END OF THE DAY"<<endl;
                    break;
                }
                cout<<"Length of the msg \n"<<length<<" "<<msg<<endl;
                //extracting user info 
                cls = userType(string(msg));
                //if a file
                if(cls.typeID == 5)
                    cout<<"AA GAYI IMAGE"<<endl;
            //if a File being transferred
                if(cls.typeID == 4){
                    cout<<cls.receiversName<<"recieveing "<<cls.size<<endl;
                    string rc_ver = cls.receiversName;
                    send(record[rc_ver],msg,1024,0);
                    int sz = atoi(cls.size.c_str());
                    int rcvd = 0;
                    length= recv(cl->fd,(unsigned char*)img,1024,0);
                    cout<<"this is the Message 1 "<<img<<endl;
                    rcvd += length;
                    cls = userType(string(img));
                    cout<<rc_ver<<"is reciveing"<<endl;
                    
                    while(cls.typeID != 5 && rcvd < sz){
                        send(record[rc_ver],(unsigned char*)img,length,0);
                        bzero(img,sizeof(msg));
                        length = recv(cl->fd,(unsigned char*)img,1024,0);
                        cout<<"This is the MEssage "<<img<<endl;
                        cls = userType(img);
                        rcvd += length; 
                    } 
        //Sending Final signal
                for(it=record.begin();it!=record.end();it++)
                          Send(it->second,img,1024,0);  
        //For Offline message storage purpose
                }else if(cls.typeID == 10){
         /*               //for private attempt    
                        if(record.find(cls.receiversName) != record.end()){
                            Send(record[cls.receiversName],msg,1024,0);
                        }else{
                            //store PVT message
                            string query_st("insert into MESSAGE values('");
                            query_st.append(cls.receiversName); query_st.append("','");
                            query_st.append(msg); query_st.append("')");
                            sql_exec(db,query_st.c_str(),NULL,NULL);
                        }*/
                }else{
    //Broadcast rest of the message
                    for(it=record.begin();it!=record.end();it++){
                          Send(it->second,msg,1024,0);    //Illegal to write on broken connection
                    }
                }
                bzero(msg,sizeof(msg));
           }
          cout<<"OK BYE"<<user_name<<" "<<cl->fd<<endl;    //debug check to see if msg reach here or not
         cout<<"All users were"<<endl; 
          for(it=record.begin();it!=record.end();it++)
              cout<<"I am "<<it->first<<" with id "<<it->second<<endl;
          cout<<"WAS IT SUPPOSE TO BE FOR "<<user_name<<" "<<cl->fd<<endl;
          record.erase(user_name);
          user_name.append("/GONE");
    //Printing user's list
          for(it=record.begin();it!=record.end();it++)
                Send(it->second,user_name.c_str(),user_name.size(),0);    //Illegal to write on broken connection
          
          close(cl->fd);
          pthread_exit(NULL);
}