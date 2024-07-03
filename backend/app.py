from flask import Flask
import socket
import time
import random
import sys
import select

class Client:
    """
    Connects to a server using TCP/IP socket.
    Attributes:
        host (str): The IP address or hostname of the server.
        port (int): The port number to connect to (any number, but must be high).
        sock (socket.socket): The socket object used for communication.

    Methods:
        __init__(): Initializes a new instance of the Client class.
        send(data: str): Sends data to the server.
        receive() -> str: Receives data from the server.
    """
    def __init__(self):
        self.host = "192.168.42.1"  # Replace by the IP of the access point
        self.port = 33000           # Replace by the port number of the server

        # Open socket connection
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host, self.port))
        print(f"Connected to {self.host}, {self.port}")

    def send(self, data):
        """
        Sends data to the server.

        Args:
            data (str): The data to send.
        """
        try:
            self.sock.sendall(data.encode("utf-8"))
        except BrokenPipeError:
            print("Connection to server lost.")
            self.sock.close()  # Close the socket when done
    
    def receive(self) -> str:
        """
        Receives data from the server.

        Returns:
            str: The received data.
        """
        try:
            return self.sock.recv(1024).decode("utf-8")
        except OSError as e:
            print("Error receiving data:", e)
            self.sock.close()  # Close the socket when done


app = Flask(__name__)

def setup():
    print("Setting up")
    # Establish connection to robot


@app.route('/mapping/<algorithm>', methods=['POST'])
def maze_mapping(algorithm):
    # TODO: map the maze using the specified algorithm (DFS, BFS)
    print("Mapping -> ", algorithm)
    return f'Algorithm: {algorithm}'

@app.route('/solve/<algorithm>', methods=['POST'])
def maze_solve(algorithm):
    # TODO: solve the maze using the specified algorithm (A*, Dijsktra)
    print("Solving -> ", algorithm)
    return f'Algorithm: {algorithm}'


if __name__ == '__main__':


    # For testing purposes
    # client = Client()
    # directions = ["F", "R", "L", "FR", "FL", "RL", "FRL"]  # F-front; R-Right; L-Left; LF-Front&Right

    # # The loop Represent an example of a functionality, participants can and should modify it
    # for _ in range(5):
    #     distance = random.uniform(1, 1000)
    #     intersection = random.choice(directions)
    #     decision = random.choice(intersection)
    #     request = f"dist:{.2%distance} | inter:{intersection} | decision:{decision}\0"
    #     client.send(request)
    #     print("[PC]" + request)
    #     resp = client.receive()
    #     if resp:
    #         print("[RASP]" + resp)

    #     time.sleep(1)

    # print("Client says bye! :)")
    # End testing

    setup()
    app.run()
