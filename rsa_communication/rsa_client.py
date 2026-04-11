import socket
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives import hashes

# Change this to your server IP if on a different machine
HOST = '10.3.0.128'
PORT = 9734

# Create client socket
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST, PORT))

# Receive server's public key
public_pem = client.recv(4096)  # increase buffer just in case
public_key = serialization.load_pem_public_key(public_pem)

print("Connected to server...")

while True:
    # Get message from user
    message = input("Client write: ").encode()

    # Encrypt message with server's public key
    encrypted_msg = public_key.encrypt(
        message,
        padding.OAEP(
            mgf=padding.MGF1(algorithm=hashes.SHA256()),
            algorithm=hashes.SHA256(),
            label=None
        )
    )

    # Send encrypted message to server
    client.send(encrypted_msg)

    # Exit if message is "bye"
    if message.decode().lower() == "bye":
        break

    # Receive server's reply (plain text)
    reply = client.recv(4096)
    if not reply:
        print("Server disconnected.")
        break

    print("Client read:", reply.decode())

    # Exit if server says "bye"
    if reply.decode().lower() == "bye":
        break

client.close()
print("Connection closed.")


