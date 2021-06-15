#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<tmwp>
using namespace tmwp;

Response::Response(int clientSocketDescriptor)
{
this->clientSocketDescriptor=clientSocketDescriptor;
this->isClosed=false;
this->headerCreated=false;
}

Response::~Response()
{}

void Response::createHeader()
{
char header[51];
strcpy(header,"HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
send(clientSocketDescriptor,header,strlen(header),0);
this->headerCreated=true;
}

void Response::write(const char *to_write)
{
if(to_write==NULL) return;
int len=strlen(to_write);
if(len==0) return;
if(!this->headerCreated) createHeader();
send(clientSocketDescriptor,to_write,len,0);
}

void Response::write(char character)
{
char to_write[2];
to_write[0]=character;
to_write[1]='\0';
if(!this->headerCreated) createHeader();
send(clientSocketDescriptor,to_write,1,0);
}

void Response::write(string to_write)
{
if(!this->headerCreated) createHeader();
send(clientSocketDescriptor,to_write.c_str(),to_write.length(),0);
}

void Response::write(int number)
{
char to_write[11];
sprintf(to_write,"%d",number);
if(!this->headerCreated) createHeader();
send(clientSocketDescriptor,to_write,strlen(to_write),0);
}

void Response::close()
{
if(this->isClosed) return;
closesocket(this->clientSocketDescriptor);
this->isClosed=true;
}
