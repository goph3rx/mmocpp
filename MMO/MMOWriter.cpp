#include <MMOWriter.h>
#include <algorithm>
#include <stdexcept>

MMOWriter::MMOWriter(asio::mutable_buffer buffer)
    : _buffer(buffer), _position(0)
{
}

void MMOWriter::skip(std::size_t size)
{
    ensure_available(size);
    std::fill(data() + _position, data() + _position + size, 0);
    _position += size;
}

void MMOWriter::write_c(char value)
{
    ensure_available(1);
    data()[_position] = value;
    _position += 1;
}

void MMOWriter::write_h(short value)
{
    ensure_available(2);
    data()[_position] = static_cast<char>(value);
    data()[_position + 1] = static_cast<char>(value >> 8);
    _position += 2;
}

void MMOWriter::write_d(long value)
{
    ensure_available(4);
    data()[_position] = static_cast<char>(value);
    data()[_position + 1] = static_cast<char>(value >> 8);
    data()[_position + 2] = static_cast<char>(value >> 16);
    data()[_position + 3] = static_cast<char>(value >> 24);
    _position += 4;
}

void MMOWriter::write_b(const std::string& value)
{
    ensure_available(value.size());
    std::copy(value.begin(), value.end(), data() + _position);
    _position += value.size();
}

void MMOWriter::ensure_available(std::size_t size)
{
    if (_position + size > _buffer.size())
    {
        throw std::range_error("Write buffer overflow");
    }
}

char* MMOWriter::data()
{
    return static_cast<char*>(_buffer.data());
}
