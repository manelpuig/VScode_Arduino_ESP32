import socket
import json

UDP_IP = "0.0.0.0"  # Listen on your computer's IP
UDP_PORT = 12345
BUFFER_SIZE = 1024

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
print(f"Listening on {UDP_IP}:{UDP_PORT}")

try:
    while True:
        data, addr = sock.recvfrom(BUFFER_SIZE)
        message = data.decode()  # Decode the received message
        print(f"Received message from {addr}: {message}")
        try:
            orientation = json.loads(data.decode())
            device_id = orientation.get("device")
            roll = orientation.get("roll")
            pitch = orientation.get("pitch")
            yaw = orientation.get("yaw")
            if device_id==1: 
                roll1 = roll
                pitch1 = pitch
                yaw1 = yaw
                print(f"Data from {device_id}: Roll_1: {roll1:.2f}, Pitch_1: {pitch1:.2f}, Yaw_1: {yaw1:.2f}")
            elif device_id==2: 
                roll2 = roll
                pitch2 = pitch
                yaw2 = yaw
            else:
                print("No device ID found in the message.")
        except json.JSONDecodeError:
                print("Invalid JSON data received")
except KeyboardInterrupt:
    print('You pressed Ctrl+C!')
    sock.close()
    print("Socket closed.")

print("Program terminated.")