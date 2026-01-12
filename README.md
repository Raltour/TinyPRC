# 🚀 PhotonRPC (v1.0)

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Platform](https://img.shields.io/badge/platform-Linux-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B20-orange)
![License](https://img.shields.io/badge/license-MIT-green)

> [📘 English](README.md) | [📙 中文](README_CN.md)

---

## 📌 Overview

**PhotonRPC** is a high-performance RPC runtime implemented in **C++20**,
designed as a **communication substrate for distributed control planes**.

Unlike general-purpose RPC frameworks that primarily target business logic,
PhotonRPC is built to serve **control paths**, such as:

- Metadata services
- Schedulers and coordinators
- State management and notification mechanisms in distributed systems

**v1.0** focuses on building a
**clean, explainable, and evolvable RPC data plane**,
laying a solid foundation for future control-plane capabilities.

---

## 🔥 Design Goals

The long-term design goals of PhotonRPC include:

- **Control-plane oriented**: optimized for low load, low latency, and strong consistency
- **Low latency & predictability**: explicit event loops and well-defined critical paths
- **Explainability first**: avoiding black-box abstractions, easy to learn and debug
- **Natural evolution**: from synchronous RPC to coroutines, consensus, and state replication

---

## ✨ Core Features

- **High-performance networking**
  - Non-blocking I/O based on `epoll` + Reactor pattern
  - Supports high concurrency connections

- **Protocol support**
  - Custom application-layer protocol (Magic + MsgID + Data)
  - Effectively handles TCP packet fragmentation and coalescing

- **Serialization**
  - Deep integration with **Google Protobuf**
  - Efficient binary serialization and deserialization

- **Service runtime**
  - XML-based configuration loading
  - Application-level **Buffer** abstraction with dynamic growth
  - Automatic service registration and reflection-based invocation

- **Infrastructure**
  - High-performance asynchronous logging via **spdlog**
  - Unit testing based on **Google Test**, with 90%+ coverage on critical components

---

## 🏗️ Architecture

*(To be completed)*

![Architecture Diagram](doc/imgs/architecture.png)

---

## 🛠️ Build & Installation

### Requirements
- OS: Linux (Ubuntu 20.04+ recommended)
- Compiler: g++ >= 10.0 (C++20 support)
- CMake >= 3.10
- Protobuf (`protobuf-compiler`, `libprotobuf-dev`)

### Build Steps
```bash
# 1. Clone the repository
git clone https://github.com/Raltour/PhotonRPC
cd PhotonRPC

# 2. Build with the provided script
chmod +x autobuild.sh
./autobuild.sh

# After build:
# Static library: lib/libphotonrpc.a
# Examples: bin/
````

---

## 💻 Usage Example

### 1. Define Service (Protobuf)

Create `echo_service.proto`:

```protobuf
syntax = "proto3";
package photonrpc;
option cc_generic_services = true;

message EchoRequest { string msg = 1; }
message EchoResponse { string msg = 1; }

service EchoService {
  rpc Echo(EchoRequest) returns (EchoResponse);
}
```

### 2. Server (Provider)

```cpp
#include <photonrpc/rpc/rpc_server.h>
#include "echo_service.pb.h"

class EchoServiceImpl : public photonrpc::EchoService {
public:
    void Echo(google::protobuf::RpcController* controller,
              const photonrpc::EchoRequest* req,
              photonrpc::EchoResponse* res,
              google::protobuf::Closure* done) override {
        res->set_msg("Server Echo: " + req->msg());
        if (done) done->Run();
    }
};

int main() {
    photonrpc::RpcServer server;
    server.RegisterService(new EchoServiceImpl());
    server.Start();
    return 0;
}
```

### 3. Client (Consumer)

```cpp
#include <photonrpc/rpc/rpc_channel.h>
#include "echo_service.pb.h"

int main() {
    photonrpc::RpcChannel channel("127.0.0.1", 12345);
    photonrpc::EchoService_Stub stub(&channel);

    photonrpc::EchoRequest req;
    req.set_msg("Hello PhotonRPC!");
    photonrpc::EchoResponse res;
    photonrpc::RpcController controller;

    stub.Echo(&controller, &req, &res, nullptr);

    if (!controller.Failed()) {
        std::cout << "Success: " << res.msg() << std::endl;
    }
    return 0;
}
```

---

## 📊 Benchmark

Single-threaded Reactor, Echo RPC (100B payload):

* **Environment**: Ubuntu 24.04 (WSL2), AMD Ryzen 7 8845H, 32GB RAM
* **Connections**: 100 concurrent connections
* **QPS**: ~14,000

This benchmark serves as a **baseline measurement**,
used to validate correctness and identify performance bottlenecks,
rather than competing directly with production-grade RPC frameworks.

---

## 📁 Directory Structure

<details>
<summary>Click to expand</summary>

```text
├── CMakeLists.txt
├── README.md
├── auto_build.bash
├── conf
│   └── photonrpc.xml
├── include
│   └── photonrpc
│       ├── common
│       ├── net
│       ├── protocol
│       └── rpc
├── src
├── test
└── third_party
```

</details>

---

## 🗺️ Roadmap

PhotonRPC follows an iterative development model,
aiming to evolve into a modern distributed communication substrate.

* [x] **v1.0 (Current)** – MVP release

    * Reactor-based networking and epoll multiplexing
    * Protobuf-based RPC and service reflection

* [ ] **v1.1** – Performance & robustness

    * Deep profiling (perf / gprof), P99 latency optimization
    * ASan / Valgrind for full memory safety

* [ ] **v1.2** – High-concurrency architecture

    * Multi-Reactor (One Loop Per Thread)
    * Efficient wake-up and task dispatch via `eventfd`

* [ ] **v2.0** – Asynchronous programming

    * C++20 coroutine-based RPC (`co_await`)
    * Eliminating callback-style async programming

* [ ] **v3.0** – Control plane evolution

    * Simplified **Raft** consensus (leader election, log replication)
    * From RPC data plane to a replicated control plane for metadata services

* [ ] **v4.0** – Heterogeneous transport (RDMA)

    * Transport abstraction with **RDMA (RoCE v2)**
    * Kernel-bypass and zero-copy for microsecond-level latency

---

## 📚 Third-party Libraries

| Library        | Purpose                                  | License      |
| -------------- | ---------------------------------------- | ------------ |
| **spdlog**     | High-performance asynchronous logging    | MIT          |
| **tinyxml2**   | Lightweight XML parser for configuration | zlib         |
| **GoogleTest** | Unit and integration testing framework   | BSD-3-Clause |

