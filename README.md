# Network and System Monitoring Tool

This project includes multiple tools and systems that focus on interacting with operating system APIs, managing system resources, multi-threading, and client-server communication. The project is divided into four key tasks:

1. **System Resource Monitoring** (CPU, Memory, Network, Disk usage)
2. **Multi-Threading Producer-Consumer Model**
3. **Socket-Based Chat System with Authentication**
4. **IP Capture and Logging Tool**

Each task is implemented in C, with appropriate system calls, threads, and networking features, followed by Python scripts for graphing and visualization.

## Table of Contents

1. [System Resource Monitoring](#system-resource-monitoring-5)
2. [Multi-Threading (Producer-Consumer)](#multi-threading-6)
3. [Socket-Based Chat System](#socket-6)
4. [IP Capture and Logging](#ip-capture-5)
5. [Setup & Usage](#setup-usage)

## System Resource Monitoring (5)

### Overview
This tool tracks the following system metrics:
- **CPU Usage (%)**
- **Memory Usage (%)**
- **Network Usage (KB) - Received and Transmitted Bandwidth**

The tool logs these metrics every 2 seconds in a `usage.txt` file and lists all active processes in a `processes.txt` file.

### Key Features:
- **CPU Usage**: Tracks and records the percentage of CPU usage.
- **Memory Usage**: Monitors the memory usage in percentage.
- **Network Usage**: Logs received and transmitted bandwidth in KB.
- **Active Processes**: Records all active processes and their details.

### Files:
- `monitoring.c`: Main program to monitor system resources.
- `usage.txt`: Contains the system metrics.
- `processes.txt`: Contains details of active processes.
- `plot_metrics.py`: Python script to read and plot the recorded metrics.

---

## Multi-Threading (Producer-Consumer) (6)

### Overview
This task simulates a **producer-consumer** model using two threads:
1. **Producer Thread**: Produces bottles every 2 seconds and places them on the queue.
2. **Consumer Thread**: Consumes bottles every 3 seconds, only if the queue has bottles.

The queue can hold a maximum of 10 bottles. If the queue is full, the producer stops producing, and the consumer consumes bottles. Once the queue goes below 10, the producer resumes production.

### Key Features:
- **Producer**: Adds bottles to the queue with a delay of 2 seconds.
- **Consumer**: Removes bottles from the queue with a delay of 3 seconds.
- **Queue Management**: The queue can hold up to 10 bottles.

### Files:
- `producer_consumer.c`: The program simulating the producer-consumer model using threads.

---

## Socket-Based Chat System (6)

### Overview
This task implements a **chat system** where a central server manages communication between multiple clients. The system ensures:
- Clients authenticate with a username before chatting.
- Clients can view all online clients and choose who to chat with.
- Chats are bidirectional (either client can send/receive messages).
- Only authenticated clients can communicate.

### Key Features:
- **Server**: Manages connections and facilitates message exchange.
- **Clients**: Authenticate, choose a client to chat with, and send/receive messages.
- **Authentication**: Clients must authenticate with a username before starting to chat.
  
### Files:
- `server.c`: Server application managing client connections and messages.
- `client.c`: Client application that allows users to authenticate and chat.
  
---

## IP Capture and Logging Tool (5)

### Overview
This task captures the **IP addresses** and **hostnames** of all devices on the same network, sends the information to a server, and logs it in a `log.txt` file. The program helps network administrators track devices on the network and identify potential external devices.

### Key Features:
- **Client**: Collects IP addresses and hostnames of all devices on the same network.
- **Server**: Logs the received IP and hostname information in a text file.

### Files:
- `client.c`: Captures IP addresses and hostnames and sends them to the server.
- `server.c`: Logs the received information into a `log.txt` file.

---

## Setup & Usage

### System Resource Monitoring
1. **Compile the program**:
   ```bash
   gcc -o monitoring monitoring.c
