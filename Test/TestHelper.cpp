#include "TestHelper.h"

#include <fmt/core.h>
#include <stdexcept>

std::string test_from_hex(const std::string& hex)
{
    if (hex.length() % 2 != 0)
    {
        throw std::invalid_argument("hex");
    }

    std::string result(hex.size() / 2, 0);
    for (auto i = 0; i < hex.length(); i += 2)
    {
        result[i / 2] =
            static_cast<char>(std::stoi(hex.substr(i, 2), nullptr, 16));
    }
    return result;
}

std::string test_to_hex(asio::const_buffer s)
{
    auto data = static_cast<const char*>(s.data());
    std::string result;
    for (auto i = 0; i < s.size(); i++)
    {
        result.append(
            fmt::format("{0:02X}", static_cast<unsigned char>(data[i])));
    }
    return result;
}
