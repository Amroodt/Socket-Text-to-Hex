# String reversal (TCP Sockets)
Uses sockets to send a string from a client, reverse the character order of the  string on the server and send the processed string back.
Although production code should use htonl() / ntohl() for clarity and portability, this example demonstrates manual serialization to make byte ordering explicit.

## 32-bit reversal
If we take a 4 byte example like my name "Joel", each letter takes up 8 bits of storage equivalent to up to 0x00 - 0xFF

This would be perfect to serialize "Joel" so that we can send bytes on the wire:
```cpp
    char name[] = "Joel";

    uint32_t wire_value = (static_cast<uint_32>(name[3]) << 24) | (name[2] << 16) | (name[1] << 8) | name[0]; 
```


```mermaid
graph BT

  subgraph 7-0
    l --> first["J"]
  end

  subgraph 15-8
    e --> second["o"]
  end

  subgraph 23-16
    o -->third["e"]
  end

  subgraph 31-24
    J --> fourth["l"]

end
```




   

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


## recv(peer_fd, buffer, sizeof(buffer), flags)

The function call *recv()* returns the number of bytes received;
as a side effect the user supplied buffer is modified and the data is pushed into the buffer variable

in this example, we set flags to '0' because we don't want to do anything complex yet

### A return value of:
- `> 0` indicates the number of bytes written to the buffer
- `0` indicates the peer has closed the connection
- `< 0` indicates an error and sets `errno`


```mermaid
graph TD
    recv_node["recv()"]
    recv_node --> return_node["Function Return"]

    return_node --> return_detail_node["int bytes_received"]

    recv_node --> buffer_node["Side Effect"]
    buffer_node --> buffer_detail_node["Writes data into buffer"];

```
## send(peer_fd, &data, sizeof(data), flags)

The function call *send* returns the number of bytes sent;


In this example, we set flags to '0' because we don't want to do anything complex yet

### A return value of:
- `> 0` indicates the number of bytes written to the buffer
- `0` indicates that nothing has been sent
- `< 0` indicates an error and sets `errno`


```mermaid
graph TD
    send_node["send()"]
    send_node --> return_node["Function Return"]

    return_node --> return_detail_node["int bytes_sent"]
```


## select()

I used select() because the functions accept(), send() and recv(), does some form of blocking.
Blocking occurs when a program asks the OS to put the calling CPU thread asleep until a defined event occurs. 

By default a program runs on one thread. An alternative to using select is fork(). This makes another process available and works pass the block.

select() uses one thread, but observes when fd's are ready and are less likely to stall an entire program.
This makes it possible for one server socket to talk to many clients at once

select() only allows fd descriptors with  value between 0 and 1024. If the fd's value is 1025, it will not with the function.

### Notes
I noticed that Lewis van Winkle had a select() function with his client socket. This was most likely for demostration purposes;often than not client sockets utilise the blocking.

```mermaid
graph LR
  node_1["getaddrinfo()"] --> node_2["socket()"]
  node_2 --> node_3["connect()"]
  subgraph client
    node_3
    end  
  

  subgraph event_loop["while(1)"]
        direction LR
        select_node["select()"]
        accept_node["accept()"]
        recv_node["recv()"]
        send_node["send()"]
        close_node["close()"]

        select_node --> accept_node
        accept_node --> recv_node
        recv_node --> send_node
        send_node --> close_node
        
        close_node --> select_node
        
    end
subgraph server_loop["server"]
        direction LR
        bind_node["bind()"] -->
        listen_node["listen()"]      
    end
  node_2-->server_loop
  server_loop --> select_node


    node_3 --> event_loop
```

