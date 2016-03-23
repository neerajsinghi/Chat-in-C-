#ifndef SQLSUPP
#define SQLSUPP
#include<iostream>
#include "sqlite3.h"

sqlite3 *db;

int callback(void *arg,int argc, char **argv, char **columns){
    int i;
    for(i=0;i<argc;i++)
        std::cout<<columns[i]<<" |";
    std::cout<<std::endl;
    if(argv[0])
        std::cout<<argv[0]<<argv[1]<<std::endl;
    
        
        //tell there is nothing
}
void sql_exec(sqlite3 *db,const char *sql,int(*cb)(void *,int,char **,char**),void *ag){
    int rc = sqlite3_exec(db,sql,cb,ag,NULL);
    std::cout<<sqlite3_errmsg(db)<<std::endl;
}
#else

#endif
