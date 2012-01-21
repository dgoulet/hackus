import socket

from Crypto.Cipher import Blowfish
from binascii import hexlify
from time import sleep

PATH = "/var/cache/l2/sub/src/blowfish_me.blf"
KEY = "IFoundCharlie!!!"

if __name__ == "__main__":
    sleep(30)
    f = open(PATH, "r")
    my_code = hexlify(Blowfish.new(KEY).encrypt(f.readline()[:64]))
    f.close()

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    sock.settimeout(15)

    sock.connect(("localhost",12345))    

    sock.send(my_code + ":LewisCarroll")
 
    sock.close()
