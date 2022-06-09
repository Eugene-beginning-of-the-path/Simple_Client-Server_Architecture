The console application to show simple client-server architecture. I used library Win32Api to work with windows sockets. 

The main work stages of this program:
1. Server is listening to all requests for connection.
2. Client identifies device of Server in the web, using an ip and a special number of a port.
3. If Client successfully connects to Server, Server will create a new socket to work with the connected Client. 
   Basic socket keeps listening to any requests to connect to Server from the web.
   


Message exchange from Client to Server and from Server to Client:
1. Client sends a message (immitation of a request) to Server. Client displays the size of the sent message to Server and its size in bytes.
2. Server successfully receives the message from Client and displays the received message and its size in bytes. 
3. Server also sends a message to Client and shuts down socket of Client.
4. Client also successfully receives the message from Server and displays its text and size in bytes, and it shuts down socket with Server.
