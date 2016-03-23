#include <iostream>
#include <string>
using namespace std;
/* This Data structure is dedicated to collect and store Information from a given string according to the Chat Application Protocol*/

struct classification
{
	int typeID;
	std::string username;
	std::string message;
	std::string fileName;
	std::string size;
	std::string receiversName;
	std::string sendersName;

};
/*This Function helps in identifying and Figuring out the type of message and returns the whole information in Data Structure*/
classification userType(string message){
	classification cf;
/*setting delimiters*/
			char delimiter1 = ':';
		char delimiter2 = '\0';
/*Very first Message after connection Setup*/
	if(message.find("/NAME") != std::string::npos){
		cf.typeID=1;
		cf.username = message.substr(message.find(delimiter1)+1,message.find(delimiter2));
		cf.message = "\0";
		cf.fileName = "\0";
		cf.size = "\0";
		cf.receiversName = "\0";
		cf.sendersName = "\0";
		


	}
/*A Private Message came*/
	else if(message.find("/PVT") != std::string::npos && message.find(":") != std::string::npos){
        cf.typeID = 3;		
        string delimiter11 = "T:";
        string delimiter22;
		delimiter2='>';
		cf.receiversName = message.substr(message.find(delimiter11)+2,message.find(delimiter2)-message.find(delimiter1)-2);
		cout<<message.substr(message.find(delimiter11)+2,message.find(delimiter2)-message.find(delimiter11)-3)<<endl;
		delimiter1='>';
		delimiter22 = ": ";
		cf.sendersName = message.substr(message.find(delimiter1)+1,message.find(delimiter22)-message.find(delimiter1)-1);
		cout<<message.substr(message.find(delimiter1)+1,message.find(delimiter22)-message.find(delimiter1)-1)<<endl;
		delimiter11=": ";
		delimiter2 = '\0';
		cf.message = message.substr(message.find(delimiter1)+2,message.find(delimiter2)-message.find(delimiter1)-1);
	}
/*Sending an IMage Signal*/
	else if(message.find("/SENDING_IMG") != std::string::npos && message.find(":") != std::string::npos){
		cf.typeID = 4;
		delimiter1 = ':';
		delimiter2 = '|';
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
		cf.message = "\0";
		cf.username = "\0";
/*We are done sending Image*/
	}else if(message.find("/IMG_SENT") != std::string::npos ){
		cf.typeID=5;
		cf.username = "\0";
		cf.message = "\0";
		cf.fileName = "\0";
		cf.size = "\0";
		cf.receiversName = "\0";
		cf.sendersName = "\0";
	}
	else
/*Anything else to be broadcasted*/
	{
		cf.typeID=0;
		cf.username ="\0";
		cf.message="\0";
	}
	return cf;	
}