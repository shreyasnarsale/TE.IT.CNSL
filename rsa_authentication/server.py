# server.py
import socket
import json
import time
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import padding

HOST = '0.0.0.0'
PORT = 5000

# Load client public key
with open("client_public.pem", "rb") as f:
    public_key = serialization.load_pem_public_key(f.read())

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen(5)

print("Server listening...")

while True:
    conn, addr = server.accept()
    print(f"\nConnected by {addr}")

    try:
        # Receive message
        msg_len = int(conn.recv(1024).decode())
        conn.send(b'OK')

        message = conn.recv(msg_len)
        conn.send(b'OK')

        # Receive signature
        sig_len = int(conn.recv(1024).decode())
        conn.send(b'OK')

        signature = conn.recv(sig_len)

        # Verify signature
        public_key.verify(
            signature,
            message,
            padding.PSS(
                mgf=padding.MGF1(hashes.SHA256()),
                salt_length=padding.PSS.MAX_LENGTH
            ),
            hashes.SHA256()
        )

        # Parse JSON
        data = json.loads(message.decode())

        # Check timestamp (within 30 sec)
        current_time = time.time()
        if abs(current_time - data["timestamp"]) > 30:
            raise Exception("Replay attack detected!")

        print("Authenticated user:", data["username"])
        print("Message:", data["message"])

        conn.send(b"AUTH SUCCESS")

    except Exception as e:
        print("Error:", str(e))
        conn.send(b"AUTH FAILED")

    conn.close()
    
    
    
    
