import socket
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.primitives import serialization, hashes

HOST = '0.0.0.0'
PORT = 9734

# Generate RSA keys
private_key = rsa.generate_private_key(
    public_exponent=65537,
    key_size=2048
)
public_key = private_key.public_key()

# Convert public key to bytes to send to client
public_pem = public_key.public_bytes(
    encoding=serialization.Encoding.PEM,
    format=serialization.PublicFormat.SubjectPublicKeyInfo
)

# Create server socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen(5)

print("Server waiting for client...")

conn, addr = server.accept()
print("Client connected:", addr)

# Send public key to client
conn.send(public_pem)

while True:
    # Receive encrypted message from client
    encrypted_msg = conn.recv(4096)
    if not encrypted_msg:
        print("Client disconnected.")
        break

    # Decrypt message using private key
    decrypted_msg = private_key.decrypt(
        encrypted_msg,
        padding.OAEP(
            mgf=padding.MGF1(algorithm=hashes.SHA256()),
            algorithm=hashes.SHA256(),
            label=None
        )
    )

    message = decrypted_msg.decode()
    print("Server read:", message)

    if message.lower() == "bye":
        break

    # Get reply from server (plain text)
    reply = input("Server write: ").encode()

    # Send plain text reply (do NOT encrypt)
    conn.send(reply)

    if reply.decode().lower() == "bye":
        break

conn.close()
server.close()
print("Connection closed.")





