//------------------------------------------------------------------------------
/*
    This file is part of rippled: https://github.com/ripple/rippled
    Copyright (c) 2012, 2013 Ripple Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#include <ripple/basics/safe_cast.h>
#include <ripple/overlay/Message.h>
#include <ripple/overlay/impl/TrafficCount.h>
#include <cstdint>

namespace ripple {

Message::Message (::google::protobuf::Message const& message, int type)
    : mCategory(TrafficCount::categorize(message, type, false))
{
    auto const messageBytes = message.ByteSizeLong();

    assert (messageBytes != 0);

    /** Number of bytes in a message header. */
    std::size_t constexpr headerBytes = 6;

    mBuffer.resize (headerBytes + messageBytes);

    auto ptr = mBuffer.data();

    *ptr++ = static_cast<std::uint8_t>((messageBytes >> 24) & 0xFF);
    *ptr++ = static_cast<std::uint8_t>((messageBytes >> 16) & 0xFF);
    *ptr++ = static_cast<std::uint8_t>((messageBytes >> 8) & 0xFF);
    *ptr++ = static_cast<std::uint8_t>(messageBytes & 0xFF);

    *ptr++ = static_cast<std::uint8_t>((type >> 8) & 0xFF);
    *ptr++ = static_cast<std::uint8_t> (type & 0xFF);

    if (messageBytes != 0)
        message.SerializeToArray(ptr, messageBytes);
}

}
