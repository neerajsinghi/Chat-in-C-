 #include<sys/socket.h>
 #include<string>
 #include<netinet/in.h>
 #include<arpa/inet.h>
 #include<iostream>
 #include<pthread.h>
 #include "prereq.h"

struct arg_db{
    string userName;
    string passW;
};

struct argu{
          int x;
          string pass_w;
 };


void *process_db(void *arg){
    struct arg_db *cl;
    cl = (struct arg_db*)arg;
    string user_name = cl-userName;
    string pass_w = cl.passW;
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
}


