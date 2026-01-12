#pragma once

#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>              // async_logger, init_thread_pool
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

class Logger {
public:
  // 初始化：创建全局异步 logger，并设为 spdlog 默认 logger
  // - async_queue_size: 异步队列长度（越大越不容易丢日志，但占内存）
  // - async_threads: 后台线程数
  // - flush_on_level: 达到该级别时自动 flush（比如 error）
  //
  // 注意：init_* 只能调用一次（或在 shutdown 后重来）。
  static void init_file(const std::string& logger_name,
                        const std::string& file_path,
                        spdlog::level::level_enum level = spdlog::level::info,
                        size_t async_queue_size = 8192,
                        size_t async_threads = 1,
                        spdlog::level::level_enum flush_on_level = spdlog::level::err);

  // 轮转文件：size 超过 max_file_size 时轮转，最多保留 max_files 个
  static void init_rotating_file(const std::string& logger_name,
                                 const std::string& file_path,
                                 size_t max_file_size,
                                 size_t max_files,
                                 spdlog::level::level_enum level = spdlog::level::info,
                                 size_t async_queue_size = 8192,
                                 size_t async_threads = 1,
                                 spdlog::level::level_enum flush_on_level = spdlog::level::err);

  static std::shared_ptr<spdlog::logger> get();

  static void set_level(spdlog::level::level_enum level);
  static void set_pattern(const std::string& pattern); // 可选：仍由 spdlog 处理
  static void flush();
  static void shutdown();

  // 下面这些接口 = spdlog 的薄封装，不自己做格式化
  template <typename... Args>
  static void trace(spdlog::format_string_t<Args...> fmt, Args&&... args) {
    if (auto lg = get()) lg->trace(fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  static void debug(spdlog::format_string_t<Args...> fmt, Args&&... args) {
    if (auto lg = get()) lg->debug(fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  static void info(spdlog::format_string_t<Args...> fmt, Args&&... args) {
    if (auto lg = get()) lg->info(fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  static void warn(spdlog::format_string_t<Args...> fmt, Args&&... args) {
    if (auto lg = get()) lg->warn(fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  static void error(spdlog::format_string_t<Args...> fmt, Args&&... args) {
    if (auto lg = get()) lg->error(fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  static void critical(spdlog::format_string_t<Args...> fmt, Args&&... args) {
    if (auto lg = get()) lg->critical(fmt, std::forward<Args>(args)...);
  }

  // 也支持直接传字符串（不带 format args）
  static void info(const std::string& msg)  { if (auto lg = get()) lg->info(msg); }
  static void warn(const std::string& msg)  { if (auto lg = get()) lg->warn(msg); }
  static void error(const std::string& msg) { if (auto lg = get()) lg->error(msg); }

private:
  static void ensure_thread_pool(size_t async_queue_size, size_t async_threads);

private:
  static std::shared_ptr<spdlog::logger> logger_;
};
