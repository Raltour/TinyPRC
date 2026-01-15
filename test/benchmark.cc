#include "../include/photonrpc/rpc.h"
#include "protocol/calculate_service.pb.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#define NUM_OF_THREADS 4
#define NUM_OF_CLIENT_PER_THREADS 10
#define NUM_OF_REQUESTS_PER_CLIENT 100

std::atomic<int> request_count(0);

void thread_func() {
  std::vector<std::unique_ptr<RpcChannel>> channels;
  std::vector<std::unique_ptr<rpc::CalculateService_Stub>> calculate_service_stubs;
  for (int i = 0; i < NUM_OF_CLIENT_PER_THREADS; ++i) {
    channels.emplace_back(std::make_unique<RpcChannel>());
    calculate_service_stubs.emplace_back(
        std::make_unique<rpc::CalculateService_Stub>(channels.back().get()));
  }

  // RpcChannel channel;
  // rpc::CalculateService_Stub calculate_service_stub(&channel);

  int arg1 = 5;
  int arg2 = 6;
  rpc::AddRequest add_request;
  rpc::AddResponse add_response;
  add_request.set_a(arg1);
  add_request.set_b(arg2);

  for (int i = 0; i < NUM_OF_REQUESTS_PER_CLIENT; i++) {
    for (auto& stub : calculate_service_stubs) {
      stub->Add(nullptr, &add_request, &add_response, nullptr);
      ++request_count;
    }
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