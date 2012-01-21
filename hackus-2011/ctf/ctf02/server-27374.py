#!/usr/bin/env python

import subprocess
import SocketServer

key = "bipbipbiiiipAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAbipbapbipAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAbongbapbipAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAPMBgotwormsAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

class MyTCPHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        data = self.request.recv(1024).strip()
        if key == data:
            self.request.send("\nCode : ctf02\nFlag : SuerAvecSiPeu\n")
        else:
            self.request.send("Access Denied\n")

if __name__ == "__main__":
    HOST, PORT = "10.1.8.12", 27374

    # Create the server, binding to localhost on port 9999
    server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)

    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()
