#include "photonrpc/common/logger.h"
#include "photonrpc/rpc/rpc_channel.h"
#include "protocol/calculate_service.pb.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#define NUM_OF_THREADS 8
#define NUM_OF_REQUESTS_PER_THREAD 1000

std::atomic<int> request_count(0);

void thread_func() {
  RpcChannel channel;
  rpc::CalculateService_Stub calculate_service_stub(&channel);

  int arg1 = 5;
  int arg2 = 6;
  rpc::AddRequest add_request;
  rpc::AddResponse add_response;
  add_request.set_a(arg1);
  add_request.set_b(arg2);

  for (int i = 0; i < NUM_OF_REQUESTS_PER_THREAD; i++) {
    calculate_service_stub.Add(nullptr, &add_request, &add_response, nullptr);
    if (add_response.result() != 11) {
      LOG_ERROR("RPC failed");
    }
    request_count++;
  }
}

int main() {
  auto start_time = std::chrono::high_resolution_clock::now();

  std::thread threads[NUM_OF_THREADS];
  for (int i = 0; i < NUM_OF_THREADS; ++i) {
    threads[i] = std::thread(thread_func);
  }

  for (int i = 0; i < NUM_OF_THREADS; ++i) {
    threads[i].join();
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);

  double qps = (request_count * 1000.0) / duration.count();

  std::cout << "Total requests: " << request_count << std::endl;
  std::cout << "Total time: " << duration.count() << " ms" << std::endl;
  std::cout << "QPS: " << qps << std::endl;

  return 0;
}