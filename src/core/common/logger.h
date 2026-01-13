#pragma once

#include "common/config.h"

#include <string>
#include <spdlog/spdlog.h>

class Logger {
public:

  static void InitFromConfig(const Config& config);

  /**
   * @brief 初始化全局 spdlog 环境（异步）
   *
   * @param log_dir     日志目录
   * @param log_name    日志文件名（如 app.log）
   * @param level       全局日志级别
   * @param max_file_mb 单个日志文件最大大小（MB）
   * @param max_files   最大滚动文件数
   * @param to_console  是否同时输出到控制台
   */
  static void Init(const std::string& log_dir,
                   const std::string& log_name,
                   spdlog::level::level_enum level = spdlog::level::info,
                   size_t max_file_mb = 100,
                   size_t max_files = 10,
                   bool to_console = true);

  /**
   * @brief 刷新并关闭日志系统（可选）
   */
  static void Shutdown();

private:
  Logger() = delete;
};
