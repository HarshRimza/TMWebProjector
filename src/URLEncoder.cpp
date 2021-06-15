#include<urlencoder>
using namespace tmwp;

int search(char *actual,const char *to_find)
{
char *start;
start=(char *)to_find;
while(*actual)
{
if(*actual==*to_find)
{
for(;*actual && *to_find && *actual==*to_find;actual++,to_find++);
if(!(*to_find)) return 0;
to_find=start;
}
actual++;
}
return 1;
}

int search(char *actual,char to_find)
{
while(*actual)
{
if(*actual==to_find) return 0;
actual++;
}
return 1;
}


string replace_all(char *actual,const char *to_replace,char to_replace_with)
{
char *to_remember;
string replaced;
string portion;
to_remember=(char *)to_replace;
replaced="";
portion="";
while(*actual)
{
if(*actual==*to_replace)
{
while(*actual && *to_replace && *actual==*to_replace)
{
portion+=*actual;
actual++;
to_replace++;
}
if(!(*to_replace)) replaced+=to_replace_with;
else replaced+=portion;
portion="";
to_replace=to_remember; 
}
replaced+=*actual;
actual++;
}
return replaced;
}

string replace_all(char *actual,char to_replace,const char *to_replace_with)
{
string replaced;
replaced="";
while(*actual)
{
if(*actual==to_replace) 
{
replaced+=to_replace_with;
actual++;
continue;
}
replaced+=*actual;
actual++;
}
return replaced;
}

string replace_all(char *actual,char to_replace,char to_replace_with)
{
string replaced;
replaced="";
while(*actual)
{
if(*actual==to_replace) 
{
replaced+=to_replace_with;
actual++;
continue;
}
replaced+=*actual;
actual++;
}
return replaced;
}

URLEncoder::URLEncoder()
{}

string URLEncoder::encode(char *toEncode)
{
string encoded;
encoded="";
encoded+=toEncode;

if(search(toEncode,'\n')==0) 
{
encoded=replace_all(toEncode,'\n',"%0A");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'\r')==0) 
{
encoded=replace_all(toEncode,'\r',"%0D");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,' ')==0)
{
encoded=replace_all(toEncode,' ',"%20");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'!')==0) 
{
encoded=replace_all(toEncode,'!',"%21");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'#')==0) 
{
encoded=replace_all(toEncode,'#',"%23");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'$')==0) 
{
encoded=replace_all(toEncode,'$',"%24");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'%')==0) 
{
encoded=replace_all(toEncode,'%',"%25");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'&')==0) 
{
encoded=replace_all(toEncode,'&',"%26");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'\'')==0) 
{
encoded=replace_all(toEncode,'\'',"%27");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'(')==0) 
{
encoded=replace_all(toEncode,'(',"%28");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,')')==0) 
{
encoded=replace_all(toEncode,')',"%29");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'+')==0) 
{
encoded=replace_all(toEncode,'+',"%2B");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,',')==0) 
{
encoded=replace_all(toEncode,',',"%2C");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'/')==0) 
{
encoded=replace_all(toEncode,'/',"%2F");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,':')==0) 
{
encoded=replace_all(toEncode,':',"%3A");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,';')==0) 
{
encoded=replace_all(toEncode,';',"%3B");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'=')==0) 
{
encoded=replace_all(toEncode,'=',"%3D");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'?')==0) 
{
encoded=replace_all(toEncode,'?',"%3F");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'@')==0) 
{
encoded=replace_all(toEncode,'@',"%40");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'[')==0) 
{
encoded=replace_all(toEncode,'[',"%5B");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'\\')==0) 
{
encoded=replace_all(toEncode,'\\',"%5C");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,']')==0) 
{
encoded=replace_all(toEncode,']',"%5D");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'^')==0) 
{
encoded=replace_all(toEncode,'^',"%5E");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'`')==0) 
{
encoded=replace_all(toEncode,'`',"%60");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'{')==0) 
{
encoded=replace_all(toEncode,'{',"%7B");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'|')==0) 
{
encoded=replace_all(toEncode,'|',"%7C");
toEncode=(char *)encoded.c_str();
}
if(search(toEncode,'}')==0) 
{
encoded=replace_all(toEncode,'}',"%7D");
}
return encoded;
}


string URLEncoder::decode(char *toDecode)
{
//%0A %0D %20 + %21 %23 %24 %25 %26 %27 %28 %29 %2B %2C %2F %3A %3B %3D %3F %40 %5B %5C %5D %5E %60 %7B %7C %7D
string decoded;
decoded="";
decoded+=toDecode;

if(search(toDecode,"%0A")==0) 
{
decoded=replace_all(toDecode,"%0A",'\n');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%0D")==0) 
{
decoded=replace_all(toDecode,"%0D",'\r');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%20")==0) 
{
decoded=replace_all(toDecode,"%20",' ');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,'+')==0) 
{
decoded=replace_all(toDecode,'+',' ');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%21")==0) 
{
decoded=replace_all(toDecode,"%21",'!');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%23")==0) 
{
decoded=replace_all(toDecode,"%23",'#');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%24")==0) 
{
decoded=replace_all(toDecode,"%24",'$');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%25")==0) 
{
decoded=replace_all(toDecode,"%25",'%');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%26")==0) 
{
decoded=replace_all(toDecode,"%26",'&');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%27")==0) 
{
decoded=replace_all(toDecode,"%27",'\'');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%28")==0) 
{
decoded=replace_all(toDecode,"%28",'(');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%29")==0) 
{
decoded=replace_all(toDecode,"%29",')');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%2B")==0) 
{
decoded=replace_all(toDecode,"%2B",'+');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%2C")==0) 
{
decoded=replace_all(toDecode,"%2C",',');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%2F")==0) 
{
decoded=replace_all(toDecode,"%2F",'/');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%3A")==0) 
{
decoded=replace_all(toDecode,"%3A",':');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%3B")==0) 
{
decoded=replace_all(toDecode,"%3B",';');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%3D")==0) 
{
decoded=replace_all(toDecode,"%3D",'=');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%3F")==0) 
{
decoded=replace_all(toDecode,"%3F",'?');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%40")==0) 
{
decoded=replace_all(toDecode,"%40",'@');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%5B")==0) 
{
decoded=replace_all(toDecode,"%5B",'[');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%5C")==0) 
{
decoded=replace_all(toDecode,"%5C",'\\');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%5D")==0) 
{
decoded=replace_all(toDecode,"%5D",']');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%5E")==0) 
{
decoded=replace_all(toDecode,"%5E",'^');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%60")==0) 
{
decoded=replace_all(toDecode,"%60",'`');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%7B")==0) 
{
decoded=replace_all(toDecode,"%7B",'{');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%7C")==0) 
{
decoded=replace_all(toDecode,"%7C",'|');
toDecode=(char *)decoded.c_str();
}
if(search(toDecode,"%7D")==0) 
{
decoded=replace_all(toDecode,"%7D",'}');
}
return decoded;
}
