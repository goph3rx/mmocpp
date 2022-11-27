#include <MMOReader.h>
#include <algorithm>
#include <iterator>

MMOReader::MMOReader(asio::const_buffer buffer) : _buffer(buffer), _position(0)
{
}

void MMOReader::skip(std::size_t size)
{
    ensure_available(size);
    _position += size;
}

std::string MMOReader::read_b(std::size_t size)
{
    ensure_available(size);
    std::string value;
    std::copy(data() + _position, data() + _position + size,
              std::back_inserter(value));
    _position += size;
    return value;
}

char MMOReader::read_c()
{
    ensure_available(1);
    auto result = data()[_position];
    _position++;
    return result;
}

short MMOReader::read_h()
{
    ensure_available(2);
    long result = data()[_position + 1] & 0xFF;
    result <<= 8;
    result |= data()[_position] & 0xFF;
    _position += 2;
    return static_cast<short>(result);
}

long MMOReader::read_d()
{
    ensure_available(4);
    long result = data()[_position + 3] & 0xFF;
    result <<= 8;
    result |= data()[_position + 2] & 0xFF;
    result <<= 8;
    result |= data()[_position + 1] & 0xFF;
    result <<= 8;
    result |= data()[_position] & 0xFF;
    _position += 4;
    return result;
}

long long MMOReader::read_q()
{
    ensure_available(8);
    long long result = data()[_position + 7] & 0xFF;
    result <<= 8;
    result |= data()[_position + 6] & 0xFF;
    result <<= 8;
    result |= data()[_position + 5] & 0xFF;
    result <<= 8;
    result |= data()[_position + 4] & 0xFF;
    result <<= 8;
    result |= data()[_position + 3] & 0xFF;
    result <<= 8;
    result |= data()[_position + 2] & 0xFF;
    result <<= 8;
    result |= data()[_position + 1] & 0xFF;
    result <<= 8;
    result |= data()[_position] & 0xFF;
    _position += 8;
    return result;
}

void MMOReader::ensure_available(std::size_t size)
{
    if (_position + size > _buffer.size())
    {
        throw std::range_error("Read buffer overflow");
    }
}

const char* MMOReader::data()
{
    return static_cast<const char*>(_buffer.data());
}