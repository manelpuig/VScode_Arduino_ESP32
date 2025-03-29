import socket
import json  # To parse JSON data

udp_ip = "0.0.0.0"  # Listen on all interfaces
udp_port = 12345    # Port to listen on

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((udp_ip, udp_port))

print(f"Listening for UDP data on {udp_ip}:{udp_port}...")

try:
    while True:
        data, addr = sock.recvfrom(1024)  # Receive up to 1024 bytes
        message = data.decode()  # Decode the received message
        print(f"Received message from {addr}: {message}")
        try:
            # Parse the JSON data
            orientation = json.loads(message)
            
            # Extract roll, pitch, and yaw
            roll = orientation.get("roll", None)
            pitch = orientation.get("pitch", None)
            yaw = orientation.get("yaw", None)
            
            # Print the extracted values
            print(f"Roll: {roll}, Pitch: {pitch}, Yaw: {yaw}")
        except json.JSONDecodeError:
            print("Invalid JSON data received")
except KeyboardInterrupt:
    print('You pressed Ctrl+C!')
    sock.close()
    print("Socket closed.")

print("Program terminated.")