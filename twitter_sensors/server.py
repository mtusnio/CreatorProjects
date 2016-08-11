#!/usr/bin/python3
import socket
import datetime
import time
from twython import Twython

twitter = Twython(<YOUR_KEYS_HERE>)  
# No IP to connect to needed for a server
IP = "::"
PORT = 3000

print("Init");
# Creates a socket using IPV6 and accepting datagrams
sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
sock.bind((IP, PORT))

def calculate_temperature(id, val):
    return id + val/100;

def process_data(id, val):
    t = time.strftime("%H %M %B %d, %Y")
    if(id == 0):
        if(val == 1):
            twitter.update_status(status="Window detected at " + t)
        if(val == 2):
            twitter.update_status(status="Window empty at " + t);
    elif(id == 1):
        if(val == 1):
            twitter.update_status(status="Corridor detected at " + t)
        if(val == 2):
            twitter.update_status(status="Corridor empty at " + t);   
    elif(id > 1):
        temperature = calculate_temperature(id, val)
        temp_status = "Temperature is now {0:.2f} at {1}".format(temperature, t)
        twitter.update_status(status=temp_status)

while True:
    data, address = sock.recvfrom(2)
    for i in range(0, 3):
        sock.recvfrom(2)
    print("Received", data)
    # Decode the client data
    try:
        process_data(data[0], data[1]);
    except Exception as e:
        print("Exception", e)
        
