import socket
import time
from data_source import get_data

HOST = '10.5.35.27'
PORT = 5000

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST, PORT))

while True:
    data = get_data()
    message = ','.join(map(str, data)) + '\n'

    client.sendall(message.encode())
    print("Sent:", data)

    time.sleep(0.05)  # match your joystick rate