The console app for show simple client-server architecture. I used library Win32Api for work with windows sockets. 

The main work stages of this program:
1. Server is listening to all requests for connection.
2. Client identifies device of Server in web, using ip and special number of a port.
3. If Client successfully connects to Server, Server will create a new socket for work with the connected Client. 
   Basic socket keeps listening to any requests to connect to Server from web.
   


Message exchange from Client to Server and from Server to Client:
1. Client sends to Server a message(immitation of a request). Client displays the size of a sent message to Server and his size in bytes.
2. Server successfully receives the message from Client and displays received the message and his size in bytes. 
3. Server also sends a message to Client and shut down socket of Client.
4. Client also successfully receives the message from Server and displays his text and size in bytes, and it shut down socket with Server.
