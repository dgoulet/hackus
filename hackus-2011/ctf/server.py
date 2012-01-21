#!/usr/bin/env python

import subprocess
import SocketServer

class MyTCPHandler(SocketServer.BaseRequestHandler):
   def handle(self):
    exploit = "./exploit "
    # self.request is the TCP socket connected to the client
    self.data = self.request.recv(4096)
    exploit += self.data
    # just send back the same data, but upper-cased
    p1 = subprocess.Popen(exploit, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    self.request.send(p1.communicate()[0] + "\n")

if __name__ == "__main__":
    HOST, PORT = "10.1.8.10", 13310

    # Create the server, binding to localhost on port 9999
    server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)

    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()
