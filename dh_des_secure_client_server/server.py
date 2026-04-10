import socket
import random
from Crypto.Cipher import DES
from Crypto.Util.Padding import unpad

HOST = "0.0.0.0"
PORT = 5000

# Diffie-Hellman parameters
p = 23
g = 5

b = random.randint(1, 10)  # server private key

server = socket.socket()
server.bind((HOST, PORT))
server.listen(1)

print("Server waiting for connection...")

conn, addr = server.accept()
print("Connected:", addr)

# Receive A from client
A = int(conn.recv(1024).decode())

# Send B to client
B = pow(g, b, p)
conn.send(str(B).encode())

# Compute shared secret
shared_secret = pow(A, b, p)
print("Shared secret:", shared_secret)

# Convert to DES key (8 bytes)
key = str(shared_secret).ljust(8)[:8].encode()

# Receive encrypted message
ciphertext = conn.recv(4096)

# Decrypt
cipher = DES.new(key, DES.MODE_ECB)
plaintext = unpad(cipher.decrypt(ciphertext), 8)

print("Decrypted message:", plaintext.decode())

conn.close()
server.close()
