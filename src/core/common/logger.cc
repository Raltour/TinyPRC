#include "logger.h"

#include <memory>
#include <vector>
#include <filesystem>

#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace fs = std::filesystem;

namespace {

spdlog::level::level_enum ToSpdlogLevel(int level) {
  // 约定
  // 0=trace, 1=debug, 2=info, 3=warn, 4=error, 5=critical, 6=off
  switch (level) {
    case 0: return spdlog::level::trace;
    case 1: return spdlog::level::debug;
    case 2: return spdlog::level::info;
    case 3: return spdlog::level::warn;
    case 4: return spdlog::level::err;
    case 5: return spdlog::level::critical;
    default: return spdlog::level::info;
  }
}

} // anonymous namespace

void Logger::InitFromConfig(const Config& config) {
  static bool initialized = false;
  if (initialized) return;
  initialized = true;

  // ---------- 从 Config 读取 ----------
  const int log_level = config.log_level();
  const int queue_size =
      config.log_queue_size() > 0 ? config.log_queue_size() : 8192;

  // ---------- 当前默认策略（无 config 项） ----------
  const std::string log_dir = "./logs";
  const std::string log_file = "photonrpc.log";
  const size_t max_file_mb = 100;
  const size_t max_files = 5;
  const bool to_console = true;
  const size_t thread_count = 1;

  fs::create_directories(log_dir);
  const std::string log_path = log_dir + "/" + log_file;

  // ---------- 异步线程池 ----------
  spdlog::init_thread_pool(queue_size, thread_count);

  std::vector<spdlog::sink_ptr> sinks;

  sinks.emplace_back(
      std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
          log_path,
          max_file_mb * 1024 * 1024,
          max_files));

  if (to_console) {
    sinks.emplace_back(
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  }

  auto logger = std::make_shared<spdlog::async_logger>(
      "default",
      sinks.begin(),
      sinks.end(),
      spdlog::thread_pool(),
      spdlog::async_overflow_policy::block);

  spdlog::set_default_logger(logger);

  spdlog::set_level(ToSpdlogLevel(log_level));
  spdlog::flush_on(spdlog::level::warn);

  spdlog::set_pattern(
      "[%Y-%m-%d %H:%M:%S.%e] [%P:%t] [%l] %v");

  spdlog::info("logger initialized from config");
}

void Logger::Init(const std::string& log_dir,
                  const std::string& log_name,
                  spdlog::level::level_enum level,
                  size_t max_file_mb,
                  size_t max_files,
                  bool to_console)
{
    static bool initialized = false;
    if (initialized) {
        return;
    }
    initialized = true;

    // 创建日志目录
    if (!log_dir.empty()) {
        fs::create_directories(log_dir);
    }

    const std::string log_path =
        log_dir.empty() ? log_name : (log_dir + "/" + log_name);

    // --------- 异步线程池 ---------
    // 队列大小 & 后台线程数
    constexpr size_t kQueueSize = 8192;
    constexpr size_t kThreadCount = 1;

    spdlog::init_thread_pool(kQueueSize, kThreadCount);

    // --------- sinks ---------
    std::vector<spdlog::sink_ptr> sinks;

    // rolling file sink
    sinks.emplace_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        log_path,
        max_file_mb * 1024 * 1024,
        max_files));

    // console sink（可选）
    if (to_console) {
        sinks.emplace_back(
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    }

    // --------- async logger ---------
    auto logger = std::make_shared<spdlog::async_logger>(
        "default",
        sinks.begin(),
        sinks.end(),
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block);

    // 设置为全局默认 logger
    spdlog::set_default_logger(logger);

    // --------- 全局配置 ---------
    spdlog::set_level(level);
    spdlog::flush_on(spdlog::level::warn);

    spdlog::set_pattern(
        "[%Y-%m-%d %H:%M:%S.%e] "
        "[%P:%t] "
        "[%l] "
        "%v");

    spdlog::info("spdlog async logger initialized, log_path={}", log_path);
}

void Logger::Shutdown()
{
    spdlog::shutdown();
}
