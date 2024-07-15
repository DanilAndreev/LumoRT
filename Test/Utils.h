#pragma once

#include <vector>

static std::vector<uint8_t> ReadBinary(std::istream& stream) noexcept {
    std::streamsize size = stream.tellg();
    stream.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (stream.read(reinterpret_cast<char*>(buffer.data()), size))
    {
        /* worked! */
    }
    return buffer;
}
