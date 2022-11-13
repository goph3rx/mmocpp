#ifndef MMO_MMOWRITER_H
#define MMO_MMOWRITER_H

#include <asio/buffer.hpp>
#include <string>

/**
 * Writing MMO values into buffers.
 */
class MMOWriter
{
   public:
    /**
     * Create a new writer.
     * @param buffer Buffer for writing.
     */
    explicit MMOWriter(asio::mutable_buffer buffer);

    MMOWriter(const MMOWriter&) = delete;
    MMOWriter& operator=(const MMOWriter&) = delete;

    /**
     * Size of the data written so far.
     */
    [[nodiscard]] std::size_t size() const { return _position; }

    /**
     * Skip.
     * @param size Number of bytes.
     */
    void skip(std::size_t size);

    /**
     * Write C value (1 byte).
     * @param value Value.
     */
    void write_c(char value);

    /**
     * Write H value (2 bytes).
     * @param value Value.
     */
    void write_h(short value);

    /**
     * Write D value (4 bytes).
     * @param value Value.
     */
    void write_d(long value);

    /**
     * Write B value.
     * @param value Value.
     */
    void write_b(const std::string& value);

   private:
    /**
     * Ensure that writer has enough space available.
     * @param size Number of bytes.
     */
    void ensure_available(std::size_t size);

    /**
     * Get the buffer as a writeable sequence of bytes.
     */
    char* data();

    /**
     * Buffer for writing.
     */
    asio::mutable_buffer _buffer;
    /**
     * Position in the buffer.
     */
    std::size_t _position;
};

#endif  // MMO_MMOWRITER_H
