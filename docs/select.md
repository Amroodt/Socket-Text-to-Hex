# select()

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
