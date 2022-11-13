#ifndef MMO_MMOEVENTLOOP_H
#define MMO_MMOEVENTLOOP_H

#include <asio/io_context.hpp>
#include <asio/strand.hpp>
#include <functional>
#include <optional>
#include <thread>
#include <vector>

typedef std::function<void()> MMOTask;

/**
 * Event loop for async tasks.
 */
class MMOEventLoop
{
   public:
    /**
     * Create a new event loop.
     * @param loop Context for the event loop.
     * @param total_threads Number of threads.
     */
    explicit MMOEventLoop(asio::io_context& loop, unsigned int total_threads);
    ~MMOEventLoop();

    /**
     * Get the context for this event loop.
     */
    asio::io_context& loop() { return _loop; }

    /**
     * Start the loop.
     */
    void start();

    /**
     * Submit some work to the loop.
     * @param task Task to be done.
     * @param strand Optional object to ensure serial execution.
     */
    void submit(const MMOTask& task,
                asio::io_context::strand* strand = nullptr);

    MMOEventLoop(const MMOEventLoop&) = delete;
    MMOEventLoop& operator=(const MMOEventLoop&) = delete;

   private:
    /**
     * Context for the event loop.
     */
    asio::io_context& _loop;
    /**
     * Number of threads.
     */
    unsigned int _total_threads;
    /**
     * Worker threads.
     */
    std::vector<std::thread> _threads;
};

#endif  // MMO_MMOEVENTLOOP_H
