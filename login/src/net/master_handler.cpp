//
// Created by TSU on 2023-05-11.
//
#include "std.hpp"
#include "net/master.hpp"
#include "net/master_handler.hpp"

hl::login::master_handler::master_handler()
    : _handlers()
{
}

void hl::login::master_handler::process(hl::login::master &session, in_buffer &in_buf)
{
    auto packet = static_cast<hl::InternalServerMessage>(in_buf.read<uint16_t>());

    if (packet >= InternalServerMessage_Max)
    {
        throw std::runtime_error("Unknown packet type " + std::to_string(packet));
    }

    if (_handlers[packet])
        _handlers[packet]->handle_packet(session, in_buf);
}
