# TCP Server-Client File Transfer Project

## Introduction
This project implements a simple TCP server-client architecture for file transfer. The client requests a file from the server by providing the server's hostname, port, and the desired file path as command-line arguments. The server, upon receiving the request, sends the requested file to the client.

## Client

### Usage
```bash
./client <hostname> <port> <file path>
