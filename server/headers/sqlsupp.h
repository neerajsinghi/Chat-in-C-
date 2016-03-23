#include<iostream>
#include "sqlite3.h"

void sql_exec(sqlite3 *db,const char *sql,int(*callback)(void *,int,char **,char**)){
    int rc = sqlite3_exec(db,sql,callback,NULL,NULL);
    std::cout<<sqlite3_errmsg(db)<<std::endl;
}
