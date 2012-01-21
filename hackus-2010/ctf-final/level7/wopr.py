#!/usr/bin/env python

import socket, sys, os
import SocketServer
from time import sleep
from threading import Thread

HOST, PORT = "10.2.0.17", 9999

dialog = {  1: 'Greetings Professor Falken.\n',
            2: 'Shall we play a game?\n',
            3: "Wouldn't you prefer a good game of chess?\n",
            4: 'Fine\nWhich side do you want?\n   1) United States\n   2) Soviet Union\n',
            5: 'Awaiting first strike command\n\nConnection Closed (Flag : ThermoNuclear)\n'}

answers = { 1: 'Hello.',
            2: 'Love to. How about Global Thermonuclear War?',
            3: "Later. Let's play global thermonuclear war",
            4: "2"}

ports = { 1: 333, 2: 8999, 3: 23876, 4: 42224 , 5: 3241}

class WOPR(Thread):
    def __init__(self, addr, state):
        Thread.__init__(self)
        self.addr = addr
        self.state = state

    def run(self):
        self.speak()
        self.stop()

    def speak(self):
        count = 0
        while(self.state < 6):
            print "Cleaning socket"
            self.client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.client_sock.settimeout(30)
            while(1):
                try:
                    if count == 10:
                        return
                    print("Connecting to " + str(ports[self.state]))
                    self.client_sock.connect((self.addr, ports[self.state]))
                    break
                except socket.error:
                    sleep(5)
                    count += 1
                    continue

            count = 0
            self.client_sock.send(dialog[self.state])
            if self.state == 5:
                break
            while(1):
                try:
                    recv_buf = self.client_sock.recv(128)
                except socket.timeout:
                    self.client_sock.send("I don't think you are Pr. Falken.\n\n")
                    return
                if recv_buf.rstrip().upper() == answers[self.state].upper():
                    self.state += 1
                    #self.client_sock.send(str(ports[self.state]) + "\n")
                    break
                else:
                    if count == 5:
                        self.client_sock.send("I'm pissed off\n\n")
                        return
                    elif count >= 3:
                        self.client_sock.send("I'm getting aggravated\n")
                    else:
                        self.client_sock.send("I'm not satisfied\n")
                    count += 1

            self.client_sock.close()

    def stop(self):
        print "Thread " + self.getName() + " terminated"
        self.client_sock.close()

class Level7Handler(SocketServer.BaseRequestHandler):
    def handle(self):
        sleep(3)
        self.request.send("\nI'll knock on David's door\n\n")
        self.request.close()
        wopr = WOPR(self.client_address[0], 1)
        wopr.start()

class Level7TCPServer(SocketServer.TCPServer):
    allow_reuse_address = 1

if __name__ == "__main__":
    print "WOPR initiated"

    server = Level7TCPServer((HOST, PORT), Level7Handler)
    server.serve_forever()

