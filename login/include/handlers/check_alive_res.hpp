//
// Created by TSU on 2023-05-11.
//

#ifndef STREET_HOMELESS_SERVER_CHECK_ALIVE_RES_HPP
#define STREET_HOMELESS_SERVER_CHECK_ALIVE_RES_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "net/login_session.hpp"
#include "net/login_server.hpp"
#include <net/packet_handler.hpp>

namespace hl::login::handlers
{
    struct check_alive_res : public hl::packet_handler<login_session>
    {
    public:
        void handle_packet(login_session& session, in_buffer& in_buf) override;
    };
}

#endif
