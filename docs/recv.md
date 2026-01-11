# recv(peer_fd, buffer, sizeof(buffer), flags)

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
    buffer_node --> buffer_detail_node["Writes data into buffer"]
