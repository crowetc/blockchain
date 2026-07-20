# Blockchain

## Overview
This project is a small, educational blockchain implemented in C++.
It demonstrates the core building blocks of blockchain technology:
- Blocks
- Transactions
- Proof-of-Work Mining
- Chain Validation

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

## Credits

This project uses a SHA‑256 implementation originally based on Olivier Gay’s
FIPS 180‑2 reference code, updated to C++ by zedwood.com (2012).  
The implementation is released under the Modified BSD License.  
See `sha256.h` for full license details.