# Blockchain

## Overview
This project is a small, educational blockchain implemented in C++.
It demonstrates the core building blocks of blockchain technology:
- Blocks
- Transactions
- Proof-of-Work Mining
- Chain Validation

## Architecture
The project is organized into several submodules:

- **Crypto** — maintains the cryptographic utilities
- **Ledger** — manages the Chain, Blocks, and Transactions  
- **Network** — handles Peers, Messages, and Node coordination

The diagram below shows the high-level relationships between these components.

![Architecture Diagram](docs/architecture-simple.png)

## Build
Requires a C++17 compiler.
This project uses Google Test and CTest for unit testing

### Using CMake
```bash
mkdir build
cd build
cmake ..
make
ctest
```
