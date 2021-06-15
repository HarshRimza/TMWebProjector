#include<tmwp>
#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<map>
#include<utility>
#include<iostream>
#include<urlencoder>
#include<tmwp_util.h>
using namespace tmwp;
using namespace std;


void send404(int socketDescriptor)
{
char header[1024];
char responseBuffer[1025];
long fileSize;
int bufferSize;
long dataSent;
FILE *file;
file=fopen("404.html","rb");
fseek(file,0,2);
fileSize=ftell(file);
sprintf(header,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",fileSize);
send(socketDescriptor,header,strlen(header),0);
fseek(file,0,0);
bufferSize=1024;
dataSent=0;
while(dataSent<fileSize)
{
if((fileSize-dataSent)<bufferSize) bufferSize=fileSize-dataSent;
fread(responseBuffer,bufferSize,1,file);
dataSent+=send(socketDescriptor,responseBuffer,bufferSize,0);
}
fclose(file);
}


char * getMIMEType(char *resource)
{
char *mimeType;
mimeType=NULL;
int len=strlen(resource);
if(len<4) return mimeType;   
int lastIndexOfDot=len-1;
while(lastIndexOfDot>0 && resource[lastIndexOfDot]!='.') lastIndexOfDot--;
if(lastIndexOfDot==0) return mimeType;
if(extensionEquals(resource+lastIndexOfDot+1,"html"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"text/html");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"css"))
{
mimeType=(char *)malloc(sizeof(char)*9);
strcpy(mimeType,"text/css");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"js"))
{
mimeType=(char *)malloc(sizeof(char)*16);
strcpy(mimeType,"text/javascript");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"png"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"image/png");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"jpg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"jpeg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"bmp"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"image/bmp");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"ico"))
{
mimeType=(char *)malloc(sizeof(char)*13);
strcpy(mimeType,"image/x-icon");
}
/*else if(extensionEquals(resource+lastIndexOfDot+1,"sct"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"text/sct");
}*/
return mimeType;
}

char isClientSideResource(char *resource)
{
int i;
for(i=0;resource[i]!='\0' && resource[i]!='.';i++);
if(resource[i]=='\0') return 'N';
if(strcmp(resource+i+1,"sct")==0) return 'N';
return 'Y';  // this will have to be changed later on
}

Request * parseRequest(char *bytes)
{
char method[11];
char resource[1001];
int i,j;
for(i=0;bytes[i]!=' ';i++) method[i]=bytes[i];
method[i]='\0';
i+=2;  //we don't want '/'
char **data=NULL;
int dataCount=0;
if(strcmp(method,"GET")==0)
{
for(j=0;bytes[i]!=' ';j++,i++) 
{
if(bytes[i]=='?') break;
resource[j]=bytes[i];
}
resource[j]='\0';
if(bytes[i]=='?')
{
i++;
int si=i;
dataCount=0;
while(bytes[i]!=' ')  // for counting '&' only
{
if(bytes[i]=='&') dataCount++;
i++;
}
dataCount++;
data=(char **)malloc(sizeof(char *)*dataCount);
int *pc=(int *)malloc(sizeof(int)*dataCount);
i=si;
int j=0;
while(bytes[i]!=' ') // noting indexes before '&'
{
if(bytes[i]=='&')
{
pc[j]=i;
j++;
}
i++;
}
pc[j]=i;
i=si;
j=0;
int howManyToPick;
while(j<dataCount)
{
howManyToPick=pc[j]-i;
data[j]=(char *)malloc(sizeof(char)*(howManyToPick+1));
strncpy(data[j],bytes+i,howManyToPick);
data[j][howManyToPick]='\0';
i=pc[j]+1;
j++;
}
free(pc);
}
}// method is of GET type
printf("Request arrived for %s\n",resource);
Request *request=new Request();
request->dataCount=dataCount;
request->data=data;
request->method=(char *)malloc((sizeof(char)*strlen(method))+1);
strcpy(request->method,method);
if(resource[0]=='\0')
{
request->resource=NULL;
request->isClientSideTechnologyResource='Y';
request->mimeType=NULL;
}
else
{
request->resource=(char *)malloc((sizeof(char)*strlen(resource))+1);
strcpy(request->resource,resource);
request->isClientSideTechnologyResource=isClientSideResource(resource);
request->mimeType=getMIMEType(resource);
}
return request;
}


TMWebProjector::TMWebProjector(int portNumber)
{
this->portNumber=portNumber;
}
TMWebProjector::~TMWebProjector()
{
}

void TMWebProjector::onRequest(string url,void (*ptrOnRequest)(Request &,Response &))
{
if(url.length()==0 || ptrOnRequest==NULL) return;
this->services.insert(pair<string,void (*)(Request &,Response &)>(url,ptrOnRequest));
}


void TMWebProjector::start()
{
int winsockState;
WORD ver;                  //structure to store version of socket in 2 bytes
WSADATA wsaData;    //structure for information of sockets available in windows
int serverSocketDescriptor;
int clientSocketDescriptor;
int bytesExtracted;
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
int length,successCode;
char requestBuffer[8192];
char responseBuffer[8192];
long i;
int bindState;
int bufferSize;
FILE *file;
long fileSize;
char *mimiType;
Response *response;

ver=MAKEWORD(1,1);
winsockState=WSAStartup(ver,&wsaData);
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create socket\n");
return;
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(this->portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
bindState=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(bindState<0)
{
printf("Unable to bind socket to port %d\n",this->portNumber);
WSACleanup();
return;
}

listen(serverSocketDescriptor,10);
length=sizeof(clientSocketInformation);
while(1)
{
printf("TMServer is ready to accept request on port %d\n",this->portNumber);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&length);
if(clientSocketDescriptor<0)
{
printf("Unable to accept client connection\n");
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}
bytesExtracted=recv(clientSocketDescriptor,requestBuffer,8192,0);
if(bytesExtracted<0)
{
printf("bytesExtracted<0\n");
// what to do is yet to be decided
}
else if(bytesExtracted==0)
{
printf("bytesExtracted==0\n");
// what to do is yet to be decided
}
else                                                     // else start
{
printf("Request arrived\n");
requestBuffer[bytesExtracted]='\0';
Request *request=parseRequest(requestBuffer);

while(1) // infinite loop to enable the forwarding feature
{
if(request->isClientSideTechnologyResource=='Y')
{
if(request->resource==NULL)
{
file=fopen("index.html","rb");
if(file!=NULL) printf("Sending index.html\n");
if(file==NULL)
{
file=fopen("index.htm","rb");
if(file!=NULL) printf("Sending index.htm\n");
}
if(file==NULL)
{
send404(clientSocketDescriptor);
printf("Sending 404 page\n");
closesocket(clientSocketDescriptor);
break;  // introduced because of the forwarding feature
}
else
{
fseek(file,0,2);
fileSize=ftell(file);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",fileSize);
fseek(file,0,0);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
bufferSize=0;
while(i<fileSize)
{
bufferSize=fileSize-i;
if(bufferSize>1024) bufferSize=1024;
fread(responseBuffer,bufferSize,1,file);
send(clientSocketDescriptor,responseBuffer,bufferSize,0);
i+=bufferSize;
}
fclose(file);
closesocket(clientSocketDescriptor);
break;  // introduced because of the forwarding feature
}
}
else                                                  // resource is not NULL
{
file=fopen(request->resource,"rb");
if(file!=NULL) printf("Sending %s\n",request->resource);
if(file==NULL)
{
printf("Sending 404 page\n");
char tmp[501];
sprintf(tmp,"<!DOCTYPE html><html><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h1></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
break;  // introduced because of the forwarding feature
}
/*else if(strcmp(request->mimeType,"text/sct")==0)
{
sendSCT(request,clientSocketDescriptor);
closesocket(clientSocketDescriptor);
break;  // introduced because of the forwarding feature
}*/
else
{
fseek(file,0,2);
fileSize=ftell(file);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type: %s\nContent-Length: %d\nConnection: close\n\n",request->mimeType,fileSize);
fseek(file,0,0);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
bufferSize=0;
while(i<fileSize)
{
bufferSize=fileSize-i;
if(bufferSize>1024) bufferSize=1024;
fread(responseBuffer,bufferSize,1,file);
send(clientSocketDescriptor,responseBuffer,bufferSize,0);
i+=bufferSize;
}
fclose(file);
closesocket(clientSocketDescriptor);
break;  // introduced because of the forwarding feature
}
}
}//if(request->isClientSideTechnologyResource=='Y') close
else                                             // server side technology resource 
{
//only if request of GET type
map<string,void (*)(Request &,Response &)>::iterator iter;
iter=this->services.find(string("/")+string(request->resource));

if(iter==this->services.end())
{
printf("Sending 404 page\n");
char tmp[501];
sprintf(tmp,"<!DOCTYPE html><html><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h1></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
closesocket(clientSocketDescriptor);
break;  // introduced because of the forwarding feature
}
else
{
response=new Response(clientSocketDescriptor);
iter->second(*request,*response);
if(request->forwardTo.length()>0)
{
free(request->resource);
request->resource=(char *)malloc((sizeof(char)*request->forwardTo.length())+1);
strcpy(request->resource,request->forwardTo.c_str());
request->forwardTo="";
request->isClientSideTechnologyResource=isClientSideResource(request->resource);
request->mimeType=getMIMEType(request->resource);
continue;
}
if(request->data!=NULL)
{
for(int k=0;k<request->dataCount;k++) free(request->data[k]);
free(request->data);
}
delete response;
break;  // introduced because of the forwarding feature
}

}
//delete request;

} // the infinite loop introduced because of the forwarding feature ends here

} //else close
} // the infinite loop related to accept method ends here
closesocket(serverSocketDescriptor);
WSACleanup();
}