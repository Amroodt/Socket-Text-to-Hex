
# send(peer_fd, &data, sizeof(data), flags)

The function call *send* returns the number of bytes sent;
as a side effect the user supplied buffer is modified and the data is pushed into the buffer variable

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
