# <div align='center'> TMWebProjector </div>
A C/C++ HTTP Server which can accepts HTTP requests from browser and returns responses like HTML page, image file, CSS file, JavaScript file etc. whatever the browser requests for. The server only works on windows platform and it currently supports pages having MIME Type.

<div align='center'>
|Page|Type|
|--------|------|
|HTML File|text/html|
|CSS File|text/css|
|JavaScript File|text/javascript |
|PNG File|image/png|
|JPG File|image/jpg|
|JPEG File|image/jpeg|
|BMP File|image/bmp|
|ICO File|image/x-icon|
</div>
  
### How to use the library
```
// eg1.cpp
#include<tmwp>
using namespace std;
void sam(Request &request,Response &response)
{
// another piece of code
}

void tom(Request &request,Response &response)
{
// code to get value from request object 
// int rl=atoi(request.get("rl").c_str());
// string user=request.get("username");

// code to set value in request scope
// request.setKeyValue("status","Available");

// code to forward request to an html file or to a registered function or send an html using overloaded write method of Response class.
// 1 -> request.forward("xyz.html");
//          or
// 2 -> request.forward("callSam");
//          or 
// 3 -> response.write("<!DOCTYPE html>");
// 3 -> response.write("<head>...</head>");
// 3 -> response.write("<body>");
// 3 -> response.write("<h1> Welcome ");
// 3 -> response.write(user);               
// 3 -> response.write("</h1>");
// 3 -> response.write("</body>");
// 3 -> response.write("</html>");
// 3 -> response.close();   // don't forget to call the close method
}

int main()
{
TMWebProjector server(5000);
server.onRequest("/callTom",tom);
server.onRequest("/callSam",sam);
server.start();
return 0;
}
```
for compiling it 
```
g++ -static main-file-name.cpp -o main-file-name.exe -I path-to-include-folder -L path-to-lib-folder -ltmwp -lws2_32
```

### Server-side C Template
This server also has a support for a template feature i.e. SCT(Server-side C Template) file. The code inside this should be written in html and wherever you wants the request object values its key name should be written within '${}'.

For Example :
```
// user.sct
<!DOCTYPE html>
<head> ... </head>
<body>
<h1>Welcome ${username}</h1>
</body>
</html>
```

In the above code the '${username}' will be replaced by the actual value of 'username' in request scope.

### How to use template feature
After creating the SCT file you need to run the templateCreator tool provided in the tool folder, it will create necessary files and the code inside your main CPP file will be the following

```
// eg1.cpp
#include"sct.h"
#include<tmwp>
using namespace std;

void tom(Request &request,Response &response)
{
// there is no change in other codes

// now you will be able to forward the request to the SCT File
// request.forward("sam.sct");
}

int main()
{
TMWebProjector server(5000);
server.onRequest("/callTom",tom);
registerSCTs(&server);
server.start();
return 0;
}
```

for compiling it 
```
g++ -static *.cpp -o main-file-name.exe -I path-to-include-folder -L path-to-lib-folder -ltmwp -lws2_32
```
