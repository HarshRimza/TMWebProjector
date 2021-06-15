#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<dirent.h>
int main()
{
int fileSize,toRead,dataRead,i,k,lastIndexOfDot,lineEnded,length;
char chunk[1024],valueOf[251],store[251];
char outputFileName[52];
char *fileName;
DIR *dir;
struct dirent *entry;
FILE *file,*sctCPPFile,*sctFile;
struct stat sctFileStat,sctCPPFileStat;

dir=opendir(".");
if(dir==NULL)
{
printf("Unable to open directory\n");
return 0;
}

sctFile=fopen("sct.h","wb");
if(sctFile==NULL) 
{
printf("Unable to create sct.h file\n");
return 0;
}
fputs("#ifndef ___SCT\r\n",sctFile);
fputs("#define ___SCT\r\n",sctFile);
fputs("#include<tmwp>\r\n",sctFile);
fputs("using namespace tmwp;\r\n",sctFile);
for(entry=readdir(dir);entry!=NULL;entry=readdir(dir))
{
fileName=entry->d_name;
length=entry->d_namlen;
lastIndexOfDot=length-1;
while(fileName[lastIndexOfDot]!='.') lastIndexOfDot--;
if(length-lastIndexOfDot!=4) continue;
if(strcmp(fileName+lastIndexOfDot+1,"sct")!=0) continue;

length=sprintf(store,"void get%.*s_SCT(Request &request,Response &response);\r\n",lastIndexOfDot,fileName);
fwrite(store,length,1,sctFile);

printf("%s\n",entry->d_name);
chunk[0]='\0';
store[0]='\0';
valueOf[0]='\0';
outputFileName[0]='\0';
sprintf(outputFileName,"%s%s",fileName,".cpp");
stat(fileName,&sctFileStat);
stat(outputFileName,&sctCPPFileStat);
if(sctFileStat.st_mtime<sctCPPFileStat.st_mtime) 
{
printf("no need to recreate\n");
continue;
}
printf("need to recreate\n");

file=fopen(fileName,"rb");
sctCPPFile=fopen(outputFileName,"wb");
if(file==NULL || sctCPPFile==NULL) 
{
printf("Unable to open file\n");
continue;
}
fputs("#include<tmwp>\r\n",sctCPPFile);
fputs("using namespace tmwp;\r\n",sctCPPFile);
length=sprintf(store,"void get%.*s_SCT(Request &request,Response &response)\r\n{\r\n",lastIndexOfDot,fileName);
fwrite(store,length,1,sctCPPFile);

fseek(file,0,2);
fileSize=ftell(file);
fseek(file,0,0);
toRead=1023;
dataRead=0;
lineEnded=1;
while(dataRead<fileSize)
{
if(toRead>(fileSize-dataRead)) toRead=fileSize-dataRead;
fread(chunk,toRead,1,file);
dataRead+=toRead;
chunk[toRead]='\0';
for(i=0;chunk[i]!='\0';i++)
{
if(lineEnded) 
{
fputs("response.write(\"",sctCPPFile);
lineEnded=0;
}

if(chunk[i]=='$')
{
if(chunk[i+1]=='{')
{
for(k=0,i+=2;chunk[i]!='}' && chunk[i]!='\0' && k<250;k++,i++) valueOf[k]=chunk[i];
if(chunk[i]=='\0')
{
if(dataRead<fileSize)
{
if(toRead>(fileSize-dataRead)) toRead=fileSize-dataRead;
fread(chunk,toRead,1,file);
dataRead+=toRead;
chunk[toRead]='\0';
i=0;
for(;chunk[i]!='}' && chunk[i]!='\0' && k<250;k++,i++) valueOf[k]=chunk[i];
}
else
{
break;
}
}
valueOf[k]='\0';
if(chunk[i]=='}')
{
fputs("\");\r\n",sctCPPFile);
fputs("response.write(",sctCPPFile);
fputs("request.getValue(\"",sctCPPFile);
fwrite(valueOf,k,1,sctCPPFile);
fputs("\"));\r\n",sctCPPFile);
lineEnded=1;
}
else
{
fputs("${",sctCPPFile);
fwrite(valueOf,250,1,sctCPPFile);
}

}   // if(chunk[i+1]=='{')

else if(chunk[i+1]=='\0')
{
if(dataRead<fileSize)
{
if(toRead>(fileSize-dataRead)) toRead=fileSize-dataRead;
fread(chunk,toRead,1,file);
dataRead+=toRead;
chunk[toRead]='\0';
i=0;
if(chunk[i]=='{')
{
for(k=0,i+=1;chunk[i]!='}' && chunk[i]!='\0' && k<250;k++,i++) valueOf[k]=chunk[i];
valueOf[k]='\0';
if(chunk[i]=='}')
{
fputs("\");\r\n",sctCPPFile);
fputs("response.write(",sctCPPFile);
fputs("request.getValue(\"",sctCPPFile);
fwrite(valueOf,k,1,sctCPPFile);
fputs("\"));\r\n",sctCPPFile);
lineEnded=1;
}
else
{
fputs("${",sctCPPFile);
fwrite(valueOf,250,1,sctCPPFile);
}

}
else
{
i--;
fputc('$',sctCPPFile);
}

}
else
{
break;
}
} //else if(chunk[i+1]=='\0')
else
{
i--;
fputc('$',sctCPPFile);
}
 
}   // if(chunk[i]=='$')

else
{
if(chunk[i]=='"') fputs("\\",sctCPPFile);
if(chunk[i]=='\r' && chunk[i+1]=='\n')
{
fputs("\");\r\n",sctCPPFile);
lineEnded=1;
i++;
continue;
}
fputc(chunk[i],sctCPPFile);
}

} // for(i=0,j=0;chunk[i]!='\0';i++)
} // while(dataRead<fileSize)

if(!lineEnded) fputs("\");\r\n",sctCPPFile);
fputs("response.close();\r\n",sctCPPFile);
fputs("}\r\n",sctCPPFile);
fclose(sctCPPFile);
fclose(file);

} // for(entry=readdir(dir);entry!=NULL;entry=readdir(dir))

fputs("void registerSCTs(TMWebProjector *server)\r\n{\r\n",sctFile);
rewinddir(dir);
for(entry=readdir(dir);entry!=NULL;entry=readdir(dir))
{
lastIndexOfDot=entry->d_namlen-1;
while(entry->d_name[lastIndexOfDot]!='.') lastIndexOfDot--;
if(entry->d_namlen-lastIndexOfDot!=4) continue;
if(strcmp(entry->d_name+lastIndexOfDot+1,"sct")!=0) continue;
length=sprintf(store,"server->onRequest(\"/%s\",get%.*s_SCT);\r\n",entry->d_name,lastIndexOfDot,entry->d_name);
fwrite(store,length,1,sctFile);
}
fputs("}\r\n",sctFile);
fputs("#endif\r\n",sctFile);
fclose(sctFile);
closedir(dir);
return 0;
}