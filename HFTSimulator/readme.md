# High-Frequency Trading (HFT) Simulator
The High-Frequency Trading (HFT) Simulator is an open-source project designed to simulate crypto high-frequency trading strategies 

## Architecture

The HFT Simulator is built using a modular and scalable multithreaded architecture, taking inspiration from the Disruptor pattern, which enables high-performance, low-latency event processing. This architecture ensures efficient handling of market data updates, order matching, and trading execution, leveraging parallelism and core affinity to maximize performance.

By assigning hot threads, such as the matching engine and strategy threads, to specific cores, the simulator aims to keep the CPU caches "hot" with the relevant data, improving cache utilization and reducing memory latency. This approach can significantly improve overall throughput and reduce latency, especially in high-frequency trading scenarios where microseconds matter.

![Screenshot from 2023-05-22 23-18-55](https://github.com/Naseefabu/HFTRepo/assets/104965020/5cfa959c-818e-45e6-8642-bf077cebf26d)


## Future Performance Improvements
The HFT Simulator is designed with future performance enhancements in mind. Here are some areas where further optimizations can be explored:

### Kernel Bypass Technology
To achieve even lower latency and higher throughput, the simulator can leverage kernel bypass technology. This technology allows direct data transfer from the Network Interface Card (NIC) to userspace 

### SIMD for JSON Parsing
To enhance parsing efficiency, the simulator can leverage Single Instruction, Multiple Data (SIMD) instructions available in modern processors. 

### ISOLCPUS and Thread Isolation
ISOLCPUS allows for isolating specific CPU cores exclusively for specific tasks, ensuring that no other user-level threads can run on those cores. By dedicating CPU cores solely to the HFT Simulator's critical threads, it reduces contention and interference from other processes, enhancing overall performance and predictability. 