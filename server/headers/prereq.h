#ifndef PREREQ
#define PREREQ
#include<iostream>
#include<map>
#include<string>
#include<sys/socket.h>
#include<pthread.h>
#include "sqlsupp.h"
using namespace std;
//pthread_mutex_t mtx;
bool f;
int no_f;
map<string,int> record;

struct trimmessage
{
	int typeID;
	std::string fileName;
	std::string size;
	std::string receiversName;
		std::string sendersName;

};
trimmessage trimMessage(string message){
	trimmessage cf;
			char delimiter1 = ':';
		char delimiter2 = '|';
	
		cf.fileName = message.substr(message.find(delimiter1)+1,message.find(delimiter2)-message.find(delimiter1)-1);
		delimiter1 = '|';
		delimiter2 = '>';
		cf.size = message.substr(message.find(delimiter1)+1,message.find(delimiter2)-message.find(delimiter1)-1);
		delimiter1 = '>';
		delimiter2 = '<';
		cf.receiversName = message.substr(message.find(delimiter1)+1,message.find(delimiter2)-message.find(delimiter1)-1);
		delimiter1 = '<';
		delimiter2 = '\0';
		cf.sendersName = message.substr(message.find(delimiter1)+1,message.find(delimiter2));
	return cf;	
}
void net_dump(const unsigned char *data_buffer, const unsigned int length){
    unsigned char byte;
    unsigned i,j;
    for(i=0;i<length;i++){
        byte = data_buffer[i];
        printf("%02x", data_buffer[i]);     //Display byte in Hexadecimal
        if(((i%16)==15) || (i = length -1)){
            for(j=0;j<15-(i%16);j++)
                printf(" ");
            printf("| ");
            for(j=i-(i%16);j<i;j++){
                byte = data_buffer[j];
                if(byte > 31 && byte < 127)     //Outside printable char range
                    printf("%c",byte);
                else
                    printf(".");
            }
            printf("\n");   //End of Dump line()16 bytes each
        }   //end if 
    }   //end for
}

int err(char *str){
    printf("Sorry Error: %s \n",str);
    return 0;
}

#else

#endif
