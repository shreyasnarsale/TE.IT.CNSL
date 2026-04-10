import socket
import random
from Crypto.Cipher import DES
from Crypto.Util.Padding import pad

HOST = "10.3.0.251"   # change IP
PORT = 5000

# Diffie-Hellman parameters
p = 23
g = 5

a = random.randint(1, 10)  # client private key

client = socket.socket()
client.connect((HOST, PORT))

# Send A
A = pow(g, a, p)
client.send(str(A).encode())

# Receive B
B = int(client.recv(1024).decode())

# Compute shared secret
shared_secret = pow(B, a, p)
print("Shared secret:", shared_secret)

# Convert to DES key
key = str(shared_secret).ljust(8)[:8].encode()

# Message
message = b"Hello from client using DH + DES"

# Encrypt
cipher = DES.new(key, DES.MODE_ECB)
ciphertext = cipher.encrypt(pad(message, 8))

# Send encrypted message
client.send(ciphertext)

client.close()
