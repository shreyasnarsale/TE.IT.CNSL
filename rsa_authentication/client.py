# client.py
import socket
import json
import time
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import padding

HOST = '10.3.0.251'   # Server IP
PORT = 5000

# Load private key
with open("client_private.pem", "rb") as f:
    private_key = serialization.load_pem_private_key(
        f.read(),
        password=None
    )

# Create structured message
data = {
    "username": "client1",
    "message": "Hello Secure Server!",
    "timestamp": time.time()
}

message_bytes = json.dumps(data).encode()

# Sign message
signature = private_key.sign(
    message_bytes,
    padding.PSS(
        mgf=padding.MGF1(hashes.SHA256()),
        salt_length=padding.PSS.MAX_LENGTH
    ),
    hashes.SHA256()
)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST, PORT))

# Send data length + data
client.send(str(len(message_bytes)).encode())
client.recv(1024)

client.send(message_bytes)
client.recv(1024)

# Send signature
client.send(str(len(signature)).encode())
client.recv(1024)

client.send(signature)

# Receive server response
response = client.recv(1024).decode()
print("Server response:", response)

client.close()



