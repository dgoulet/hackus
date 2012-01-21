#!/usr/bin/python

import socket
import os, sys
import subprocess
import SocketServer

def validate_func(msg):
    if msg.find("secret") > -1:
        return "Fail"
    if msg.find("eval") > -1:
        return "Fail"
    if msg.find("validate_func") > -1:
        return "Fail"

    return "Good"

def secret():
    FLAG = "COME_GET_ME"
    return FLAG

def fail(sock):
    sock.send("Fail\n")

d = {'f': fail, 'k': 1, 's': validate_func}

class MyTCPHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        # self.request is the TCP socket connected to the client
        func = self.request.recv(75).strip()
        try:
            if len(func) > 5:
                ret = validate_func(func)
                if ret == "Fail":
                    fail(self.request)
                    return
                try:
                    exec func
                except:
                    return
                if d['s'] != fail:
                    fail(self.request)
                    return
                if d['k']:
                    fail(self.request)
                    return

            if d['s'] == fail and d['a'] == secret:
                self.request.send(secret())
                return
        except KeyError, ke:
            pass

if __name__ == "__main__":
    # Create the server, binding to localhost on port 9999
    server = SocketServer.TCPServer(("localhost", 9999), MyTCPHandler)

    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()
