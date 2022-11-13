#ifndef MMO_MMOREADER_H
#define MMO_MMOREADER_H

#include <asio/buffer.hpp>
#include <string>

/**
 * Reading MMO values from buffers.
 */
class MMOReader
{
   public:
    /**
     * Create a new reader.
     * @param buffer Buffer for reader.
     */
    explicit MMOReader(asio::const_buffer buffer);

    MMOReader(const MMOReader&) = delete;
    MMOReader& operator=(const MMOReader&) = delete;

    /**
     * Position in the read buffer.
     */
    [[nodiscard]] std::size_t position() const { return _position; }

    /**
     * Skip.
     * @param size Number of bytes.
     */
    void skip(std::size_t size);

    /**
     * Read B value (size bytes).
     * @param size Size of the value.
     */
    std::string read_b(std::size_t size);

    /**
     * Read C value (1 byte).
     */
    char read_c();

    /**
     * Read H value (2 bytes).
     */
    short read_h();

    /**
     * Read D value (4 bytes).
     */
    long read_d();

   private:
    /**
     * Ensure that writer has enough space available.
     * @param size Number of bytes.
     */
    void ensure_available(std::size_t size);

    /**
     * Get the buffer as a readable sequence of bytes.
     */
    const char* data();

    /**
     * Buffer for writing.
     */
    asio::const_buffer _buffer;
    /**
     * Position in the buffer.
     */
    std::size_t _position;
};

#endif  // MMO_MMOREADER_H
