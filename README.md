# Endian string reversal
Uses sockets to send a string from a client, reverse the character order of the  string on the server and send the processed string back.

## Context
### Original example
This project is based on exercises from
*Hands-On Network Programming with C* by Lewis Van Winkle.

The original example converts small letters (example 'a') to capatilized letters (example 'A') in a string.
The client socket sends a string, the server socker receives the string, processes the string and sends the updated string back to the client.

### Changes
I kept the client socket as is. 
The server socket was modified so that we could practice **endianess**.

Instead of converting letters to uppercase variants, I reversed the endianness of the string.
If you send "Joel" for example, it will reply with "leoJ"
