import socket
from time import sleep
from time import time

MY_CODE = "charlie:742330d6617e449e7bb460e802d50701"

if __name__ == "__main__":
    # Synchro workaround
    sleep(45)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    sock.settimeout(15) 

    sock.connect(("localhost", 12345))

    sock.send(MY_CODE)

    data = sock.recv(256)

    secret_path, key = data.split(":")

    r_file = open(secret_path , "w")
    r_file.writelines(key)
    r_file.close()

    sock.close()
