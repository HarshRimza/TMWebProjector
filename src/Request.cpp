#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<map>
#include<utility>
#include<urlencoder>
#include<tmwp_util.h>
#include<tmwp>
using namespace tmwp;


Request::Request()
{
}
Request::~Request()
{
delete this->method;
if(this->mimeType) delete this->mimeType;
if(this->resource) delete this->resource;
}

// setKeyValue and getValue is for at a time purpose
void Request::setKeyValue(string k,string v)
{
this->keyValues.insert(pair<string,string>(k,v));
}

string Request::getValue(string k)
{
map<string,string>::iterator i=this->keyValues.find(k);
if(i!=this->keyValues.end())
{
return (*i).second;
}
return string("");
}


// when request arrives the data in the request will be extracted by get
string Request::get(string name)
{
int i,e;
string val;
for(i=0;i<this->dataCount;i++)
{
for(e=0;data[i][e]!='\0' && data[i][e]!='=';e++);
if(data[i][e]!='=') continue;
if(strncmp(data[i],name.c_str(),e)==0) break;
}
if(i==this->dataCount)
{
val="";
}
else
{
val=string(data[i]+(e+1));
}
return URLEncoder::decode((char *)val.c_str());
}

void Request::forward(string forwardTo)
{
this->forwardTo=forwardTo;
}

void Request::setInt(const char *variable,int value)
{
this->numbers.insert(pair<string,int>(variable,value));
}
int Request::getInt(const char *to_get)
{
map<string,int>::iterator iter;
int value;
value=0;
iter=this->numbers.find(to_get);
if(iter!=this->numbers.end()) 
{
value=(*iter).second;
return value;
}
return value;
}