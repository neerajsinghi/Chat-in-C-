#include<cstring> 
#include<cstdio> 
#include<cstdlib> 
#include<string> 
#include<map>
/*Necessary Headers for Threading function and other required Data structures*/
#include "sock3.h"
#include "prereq.h"
#include<string>


#define PORT 9870   //connection port

using namespace std;

    //strerror, perror
int main(){
    no_f = -1;
    f=false;
    //For Database Entries
    sqlite3_open("whois",&db);
    sqlite3_errmsg(db);
    sql_exec(db,"create table USERS(username varchar(255),password varchar(255), PRIMARY KEY(username))",NULL,NULL);
    sql_exec(db,"create table MESSAGE(username varchar(255), message varchar(1024), FOREIGN KEY(username)REFERENCES USERS(username))",NULL,NULL);

    int sockfd,new_sockfd;  //listen on sock_fd, new connection on new_fd
//  Host and Client Addresses
    struct sockaddr_in host_addr,client_addr;
    socklen_t sin_size;
    int recv_length=1,yes=1;
    char buff[1024];
    //getting a new socket descriptor
    if((sockfd = socket(PF_INET, SOCK_STREAM,0))==-1)
       return err("wrong socket\n\0");
    //Making socket reusable
    if(-1 ==setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)))
        return err("SO_REUSEADDR");
    //assign address parameters to Server
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = 0;  //get default address
    memset(&(host_addr.sin_zero), '\0',8);
    //Binding socket to address
    if(-1 == bind(sockfd, (struct sockaddr*)&host_addr,sizeof(sockaddr)))
        return err("binding");

    //Start listening on socket
    if(-1 == listen(sockfd, 10))
        return err("listening");
    cout<<"I am Server:IP "<<inet_ntoa(host_addr.sin_addr)<<" "<<ntohs(host_addr.sin_port)<<endl;
    //Start the server
       cout<<"Hi This is Server\n";
    while(1){
        cout<<"HI"<<endl;
        sin_size = sizeof(sockaddr_in);
    //Store address of the incoming client
        new_sockfd = accept(sockfd,(sockaddr*)&client_addr,&sin_size);
        cout<<"\033[1;31mGot connection from\033[0m "<<inet_ntoa(client_addr.sin_addr)<<" "<<ntohs(client_addr.sin_port)<<endl;
    //Allocate dynamic Memory to the structure which will store necessary info about current connection like socket Descriptor and Address       
        struct pass *arg_thread = (struct pass *) malloc(sizeof(struct pass));
        arg_thread->fd = new_sockfd;
        arg_thread->cl_addr = client_addr;

        //create a new thread for new connection
        pthread_t th;
        pthread_create(&th,NULL,process_conn,arg_thread);
        pthread_detach(th);
    }
    //Putting up bye to know when came out of loop and closing the database
    cout<<"OK BYE"<<"Server"<<endl;
    sqlite3_close(db);

    return 0;
}

    
    