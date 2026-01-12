#include "logger.h"

std::shared_ptr<spdlog::logger> Logger::logger_;

void Logger::ensure_thread_pool(size_t async_queue_size, size_t async_threads) {
  // init_thread_pool 只能初始化一次；重复会抛异常。
  // 做法：如果默认线程池不存在，就初始化；否则复用。
  if (!spdlog::thread_pool()) {
    spdlog::init_thread_pool(async_queue_size, async_threads);
  }
}

void Logger::init_file(const std::string& logger_name,
                       const std::string& file_path,
                       spdlog::level::level_enum level,
                       size_t async_queue_size,
                       size_t async_threads,
                       spdlog::level::level_enum flush_on_level) {
  ensure_thread_pool(async_queue_size, async_threads);

  auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(file_path, /*truncate=*/false);

  logger_ = std::make_shared<spdlog::async_logger>(
      logger_name,
      spdlog::sinks_init_list{sink},
      spdlog::thread_pool(),
      spdlog::async_overflow_policy::block // 队列满时阻塞，避免丢日志
  );

  logger_->set_level(level);
  logger_->flush_on(flush_on_level);

  // 可选：默认 pattern（你也可以通过 set_pattern 改）
  logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%t] [%^%l%$] %v");

  spdlog::set_default_logger(logger_);
}

void Logger::init_rotating_file(const std::string& logger_name,
                                const std::string& file_path,
                                size_t max_file_size,
                                size_t max_files,
                                spdlog::level::level_enum level,
                                size_t async_queue_size,
                                size_t async_threads,
                                spdlog::level::level_enum flush_on_level) {
  ensure_thread_pool(async_queue_size, async_threads);

  auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      file_path, max_file_size, max_files, /*rotate_on_open=*/false);

  logger_ = std::make_shared<spdlog::async_logger>(
      logger_name,
      spdlog::sinks_init_list{sink},
      spdlog::thread_pool(),
      spdlog::async_overflow_policy::block
  );

  logger_->set_level(level);
  logger_->flush_on(flush_on_level);
  logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%t] [%^%l%$] %v");

  spdlog::set_default_logger(logger_);
}

std::shared_ptr<spdlog::logger> Logger::get() {
  // 允许你在 init 前调用：返回默认 logger（可能为空）
  if (logger_) return logger_;
  return spdlog::default_logger();
}

void Logger::set_level(spdlog::level::level_enum level) {
  if (auto lg = get()) lg->set_level(level);
}

void Logger::set_pattern(const std::string& pattern) {
  if (auto lg = get()) lg->set_pattern(pattern);
}

void Logger::flush() {
  if (auto lg = get()) lg->flush();
}

void Logger::shutdown() {
  logger_.reset();
  spdlog::shutdown();
}
