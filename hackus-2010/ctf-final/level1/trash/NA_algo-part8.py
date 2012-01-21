import sys,os

def encrypt(plain):
    cipher = [];
    for i in range(0,8):
        cipher.append(((ord(plain[i])+5) % 65))

    for j in range(0,4):
        cipher[j] = cipher[j] ^ cipher[j+4]

    for k in range(0,4):
        cipher[k+4] = cipher[k] ^ cipher[k+4]

    cipher_str = ""
    for i in range(0,8):
        cipher_str += chr(cipher[i] + 65)

    return cipher_str

def decrypt(cipher):
    plain = []
    for i in range(0,8):
        plain.append(ord(cipher[i]) - 65)

    for j in range(0,4):
        plain[j+4] = plain[j] ^ plain[j+4]

    for k in range(0,4):
        plain[k] = plain[k] ^ plain[k+4]

    plain_str = ""
    for i in range(0,8):
        plain_str += chr(plain[i] + 60)

    return plain_str

print "Cipher : " + encrypt("efxrnpzU")
print "Plain : " + decrypt("\_Cpjk}w")

