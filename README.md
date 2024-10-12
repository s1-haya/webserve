# 42 Webserver

<img src="https://github.com/user-attachments/assets/34ed6b59-d0ed-4686-859e-b0e7a64ad300" alt="42Webserv" height="325">

## OVERVIEW

A Simple Webserver Implementation in C++

## USAGE

```
make run
```
to use default configuration file or
```
make && ./webserv [configuration file]
```
to use optional configuration file

## IMPLEMENTATION

You can use HTTP methods

- GET, POST, DELETE to ordinary files
- GET, POST to cgi files

You use GET to retrieve files from the server, POST to upload files or echo the messages you sent, and DELETE to remove files you have uploaded.  
Additionally, you can execute CGI scripts using GET, or use POST to execute them with the messages you sent.

You can configure the following:
- server names
- listen host:port
- uri routing
- default error pages
- client max body size
- accepted HTTP methods
- HTTP redirection
- alias
- directory listing
- default page
- cgi extension
- file upload directory

This server is compatible with `FireFox`

## TESTS

```
make test
```

There are unit tests and integration tests in `test` directory

## REFERENCE
1. https://datatracker.ietf.org/doc/html/rfc9112
2. https://datatracker.ietf.org/doc/html/rfc9110
3. https://datatracker.ietf.org/doc/html/rfc3875
4. https://datatracker.ietf.org/doc/html/rfc4918
5. https://datatracker.ietf.org/doc/html/rfc7578
