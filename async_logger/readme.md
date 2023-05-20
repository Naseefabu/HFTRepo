# async_logger

In the application thread we push the logging message to lockfree queue where thread 2 busy spin on the queue and saves it to disk.
Lock-free data structures are data structures that are thread and interrupt safe without having to use mutual exclusion mechanisms. 
Locking has non-neglegible runtime cost as each lock requires a syscall(context switches are really expensive for low latency applications). Application thread is not blocked while logging the message it will just push the message to the lockfree queue, where writer() thread will run on its own saving to disk.

### Architecture 

![Screenshot from 2023-05-19 19-36-58](https://github.com/Naseefabu/HFTRepo/assets/104965020/9ba0ab4d-c71c-436e-9d62-a8859da118a3)