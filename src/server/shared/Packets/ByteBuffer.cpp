/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ByteBuffer.h"
#include "MessageBuffer.h"
#include <sstream>
#include <ctime>

ByteBuffer::ByteBuffer(MessageBuffer&& buffer) : _rpos(0), _wpos(0), _bitpos(InitialBitPos), _curbitval(0), _storage(buffer.Move())
{
}

ByteBufferPositionException::ByteBufferPositionException(size_t pos, size_t size, size_t valueSize)
{
    std::ostringstream ss;

    ss << "Attempted to get value with size: "
       << valueSize << " in ByteBuffer (pos: " << pos << " size: " << size
       << ")";

    message().assign(ss.str());
}

ByteBuffer& ByteBuffer::operator>>(float& value)
{
    value = read<float>();
    if (!std::isfinite(value))
        throw ByteBufferException();
    return *this;
}

ByteBuffer& ByteBuffer::operator>>(double& value)
{
    value = read<double>();
    if (!std::isfinite(value))
        throw ByteBufferException();
    return *this;
}

uint32 ByteBuffer::ReadPackedTime()
{
    uint32 packedDate = read<uint32>();
    tm lt = tm();

    lt.tm_min = packedDate & 0x3F;
    lt.tm_hour = (packedDate >> 6) & 0x1F;
    //lt.tm_wday = (packedDate >> 11) & 7;
    lt.tm_mday = ((packedDate >> 14) & 0x3F) + 1;
    lt.tm_mon = (packedDate >> 20) & 0xF;
    lt.tm_year = ((packedDate >> 24) & 0x1F) + 100;

    return uint32(mktime(&lt));
}

void ByteBuffer::append(const uint8 *src, size_t cnt)
{
    FlushBits();
    _storage.insert(_storage.begin() + _wpos, src, src + cnt);
    _wpos += cnt;
}
void ByteBuffer::put(size_t pos, const uint8 *src, size_t cnt)
{
    std::memcpy(&_storage[pos], src, cnt);
}

void ByteBuffer::PutBits(std::size_t pos, std::size_t value, uint32 bitCount)
{
    for (uint32 i = 0; i < bitCount; ++i)
    {
        std::size_t wp = (pos + i) / 8;
        std::size_t bit = (pos + i) % 8;
        if ((value >> (bitCount - i - 1)) & 1)
            _storage[wp] |= 1 << (7 - bit);
        else
            _storage[wp] &= ~(1 << (7 - bit));
    }
}

void ByteBuffer::print_storage() const
{
    std::ostringstream o;
    o << "STORAGE_SIZE: " << size();
    for (uint32 i = 0; i < size(); ++i)
        o << read<uint8>(i) << " - ";
    o << " ";
}

void ByteBuffer::textlike() const
{
    std::ostringstream o;
    o << "STORAGE_SIZE: " << size();
    for (uint32 i = 0; i < size(); ++i)
    {
        char buf[2];
        snprintf(buf, 2, "%c", read<uint8>(i));
        o << buf;
    }
    o << " ";
}

void ByteBuffer::hexlike() const
{
    uint32 j = 1, k = 1;

    std::ostringstream o;
    o << "STORAGE_SIZE: " << size();

    for (uint32 i = 0; i < size(); ++i)
    {
        char buf[4];
        snprintf(buf, 4, "%2X ", read<uint8>(i));
        if ((i == (j * 8)) && ((i != (k * 16))))
        {
            o << "| ";
            ++j;
        }
        else if (i == (k * 16))
        {
            o << "\n";
            ++k;
            ++j;
        }

        o << buf;
    }
    o << " ";
}
