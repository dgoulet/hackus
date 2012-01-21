"""
Author : Alice in Wonderland
Date : 789th of April 2054

Alice cron script
"""

import os, sys
import MySQLdb
import socket
from time import time, sleep
from optparse import OptionParser
from Crypto.Cipher import Blowfish
from binascii import hexlify

CHARLIE_PATH = "/lib/tls/i686/cmov/FlAg_l3v3l_Two.txt"

class Alice:
    """ Class for opts parser """
    @staticmethod
    def getParser():
        """ Returns dictionnary with ALL program options """
        p = OptionParser(usage="%prog --user=USER --pass=PASS --host=HOST")
        p.enable_interspersed_args()

        p.add_option("--user", action="store", dest="user_db")
        p.add_option("--pass", action="store", dest="pass_db")
        p.add_option("--host", action="store", dest="host_db")

        return p

    @staticmethod
    def usage():
        """ Output the usage string and exit """
        AskGod.getParser().print_usage() 

if __name__ == "__main__":

    # Wipping proof
    tmp_f = open(CHARLIE_PATH,"w")
    tmp_f.writelines("Not The Flag. It's coming...\n")
    tmp_f.close()

    HOST, PORT = "localhost", 12345

    opts, args = Alice.getParser().parse_args()

    # === MySQL Fecth ===

    mysql_conn = MySQLdb.connect(host=opts.host_db, \
                                user=opts.user_db, \
                                passwd=opts.pass_db, \
                                db="level2")

    cursor = mysql_conn.cursor()
    cursor.execute("SELECT path,code FROM code WHERE id=42")
    bob_path = cursor.fetchall()[0]
    #print "bob_path : " + str(bob_path)
    mysql_conn.close()

    bob_file = open(os.path.join(bob_path[0], "blowfish_me.blf"), "r")
    bob_line = bob_file.readline()
    #print bob_line[:64]
    bob_code = hexlify(Blowfish.new(bob_path[1]).encrypt(bob_line[:64]))
    #print "bob_code : " + bob_code
    bob_file.close()

    # === Network Listen ===
    key = "-1"
    blacklist = []

    print "Listening on localhost:12345"

    # Create the server, binding to localhost on port 12345
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    sock.bind((HOST,PORT))
    sock.listen(1)

    # Timeout after 30 seconds
    sock.settimeout(30)
    
    total_time = time()
    while (time() - total_time) < 60:
        try:
            conn, addr = sock.accept()
            if addr[0] in blacklist:
                conn.send("Banned\n")
                conn.close()
            conn.settimeout(10)
            data = conn.recv(256).strip()
        except socket.error:
            continue
        if data.startswith(bob_code):
            key = data.rpartition(":")[2]
            conn.close()
        elif data == "charlie:742330d6617e449e7bb460e802d50701" and key != "-1":
            conn.send(CHARLIE_PATH + ":" + key + "\n")
            break
        else:
            conn.send("You are not Bob! Banned!\n")
            blacklist.append(addr[0])
            conn.close()

    tmp_f = open(CHARLIE_PATH,"w")
    tmp_f.writelines("Not The Flag. It's coming...\n")
    tmp_f.close()

    conn.close()
