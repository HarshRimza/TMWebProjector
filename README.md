# TMWebProjector
A C++ HTTP Server which can accepts HTTP requests from browser and returns responses like HTML page, image file, CSS file, JavaScript file etc. whatever the browser requests for.
This server currently supports pages having MIME Type

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

### Server-side C Template
This server also has a support for a template feature i.e. SCT(Server-side C Template) file. The code inside this should be written in html and wherever you wants the request object values its key name should be written within '${}'. A tool 

For Example :
```
// user.html
<!DOCTYPE html>
<head> ... </head>
<body>
<h1>Welcome ${username}</h1>
</body>
</html>

// 
```

In the above code the '${username}' will be replaced by the actual value of 'username' in request scope.

