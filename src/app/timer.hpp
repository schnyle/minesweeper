#pragma once

#include <chrono>
#include <format>
#include <optional>
#include <source_location>
#include <stdexcept>

class Timer
{
public:
  using clock = std::chrono::steady_clock;

  void start()
  {
    if (started())
    {
      throw std::logic_error(std::format("{}: timer already running", std::source_location::current().function_name()));
    }
    start_ = clock::now();
  }

  void stop()
  {
    if (!running())
    {
      throw std::logic_error(std::format("{}: timer not running", std::source_location::current().function_name()));
    }
    stop_ = clock::now();
  }

  void reset()
  {
    start_.reset();
    stop_.reset();
  }

  [[nodiscard]] bool started() const { return start_.has_value(); }
  [[nodiscard]] bool running() const { return start_ && !stop_; }

  [[nodiscard]] std::optional<clock::duration> elapsed() const
  {
    if (!start_)
    {
      return std::nullopt;
    }
    return (stop_ ? *stop_ : clock::now()) - *start_;
  };

private:
  std::optional<clock::time_point> start_ = std::nullopt;
  std::optional<clock::time_point> stop_ = std::nullopt;
};
