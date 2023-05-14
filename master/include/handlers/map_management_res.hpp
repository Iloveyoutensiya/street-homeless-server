//
// Created by T2SU on 2023/05/15.
//

#ifndef STREET_HOMELESS_SERVER_F6E287DA8DCF4D00960C6A70ABA34405
#define STREET_HOMELESS_SERVER_F6E287DA8DCF4D00960C6A70ABA34405

#if defined(_MSC_VER) || \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "net/master_session.hpp"
#include "net/master_server.hpp"
#include <net/packet_handler.hpp>

namespace hl::master::handlers
{
    struct map_management_res : public hl::packet_handler<master_session>
    {
    public:
        void handle_packet(master_session &session, in_buffer &in_buf) override;
    };
}

#endif //STREET_HOMELESS_SERVER_F6E287DA8DCF4D00960C6A70ABA34405
