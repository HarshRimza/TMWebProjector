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

### Server-side C Template
This server also has a support for a template feature i.e. SCT(Server-side C Template) file. The code inside this should be written in html and wherever you wants the request object values its key name should be written within '${}'.

For Example :
```
<!DOCTYPE html>
<head> ... </head>
<body>
<h1>Welcome ${username}</h1>
</body>
</html>
```

In the above code the '${username}' will be replaced by the actual value of 'username' in request scope.
