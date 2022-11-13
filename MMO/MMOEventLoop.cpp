#include <MMOEventLoop.h>
#include <spdlog/fmt/chrono.h>
#include <spdlog/spdlog.h>
#include <asio/bind_executor.hpp>
#include <asio/post.hpp>
#include <chrono>
#include <exception>

MMOEventLoop::MMOEventLoop(asio::io_context& loop, unsigned int total_threads)
    : _loop(loop), _total_threads(total_threads)
{
}

void MMOEventLoop::start()
{
    SPDLOG_INFO("Starting event loop with {} threads", _total_threads);

    for (auto i = 0; i < _total_threads; i++)
    {
        _threads.emplace_back([this]() { _loop.run(); });
    }
}

void MMOEventLoop::submit(const MMOTask& task, asio::io_context::strand* strand)
{
    // Create the task to run
    auto created = std::chrono::steady_clock::now();
    auto wrapped_task = [=]()
    {
        auto started = std::chrono::steady_clock::now();
        try
        {
            task();
        }
        catch (const std::exception& e)
        {
            SPDLOG_CRITICAL("Failed to run task: {}", e.what());
        }
        auto finished = std::chrono::steady_clock::now();
        auto waiting = std::chrono::duration_cast<std::chrono::milliseconds>(
            started - created);
        auto working = std::chrono::duration_cast<std::chrono::milliseconds>(
            finished - started);
        SPDLOG_DEBUG("Task done, spent {} waiting and {} working", waiting,
                     working);
    };

    // Add it to the event loop
    if (strand)
    {
        asio::post(_loop, asio::bind_executor(*strand, wrapped_task));
    }
    else
    {
        asio::post(_loop, wrapped_task);
    }
}

MMOEventLoop::~MMOEventLoop()
{
    for (auto& thread : _threads)
    {
        thread.join();
    }
}