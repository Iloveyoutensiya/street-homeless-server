//
// Created by TSU on 2023-05-14.
//

#ifndef STREET_HOMELESS_SERVER_USER_RECORD_HPP
#define STREET_HOMELESS_SERVER_USER_RECORD_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include "db_user_job.hpp"
#include "net/master_server.hpp"

namespace hl::master
{
    class map_state;

    class user_record
    {
    private:
        const uint64_t _pid;
        uint32_t _server_idx;
        uint32_t _socket_sn;
        player_data _player_data;
        user_state _state;
        db_job_state _db_job_state;
        std::queue<std::shared_ptr<hl::database::job>> _jobs;
        std::shared_ptr<map_state> _map;
        std::mutex _mutex;

    public:
        user_record(uint32_t server_idx, uint32_t socket_sn, uint64_t pid);

        inline uint64_t get_pid() const { return _pid; }

        inline void set_server_idx(uint32_t server_idx) { _server_idx = server_idx; }
        inline void set_player_socket_sn(uint32_t socket_sn) { _socket_sn = socket_sn; }
        inline uint32_t get_server_idx() const {return _server_idx; }
        inline uint32_t get_player_socket_sn() const { return _socket_sn; }

        const player_data &get_player_data() const;
        void set_player_data(player_data&& player_data);

        user_state get_state() const;
        void set_state(user_state state);

        std::shared_ptr<map_state> get_map() const;
        void set_map(std::shared_ptr<map_state> map);

        template<typename JobTy, typename... Types>
        void enqueue_db_job(std::shared_ptr<user_record> user, Types&&... val)
        {
            if (user.get() != this)
                throw std::runtime_error("passed user_record is different from self object reference.");
            synchronized (_mutex)
            {
                _jobs.push(std::reinterpret_pointer_cast<hl::database::job>(std::make_shared<JobTy>(val...)));
                if (_db_job_state == db_job_state::queued)
                    return;
                _db_job_state = db_job_state::queued;
            }
            hl::singleton<master_server>::get().accessor().post<db_user_job>(std::move(user));
        }

        void process_db_job(sqlpp::mysql::connection& conn);
    };
}

#endif //STREET_HOMELESS_SERVER_USER_RECORD_HPP


//  어떻게 해야 안정적으로 db_user_job을 돌리는가?

/*
          [마스터 세션]            [DB 작업자1]            [DB 작업자2]
       [EnterGameWorldReq]
       [FriendLoadReq]          [u.state=queued ]
                                [               ]
                                [player_load_job]
                                [               ]

                                [               ]
                                [               ]
                                [friend_load_job]
                                [               ]
                                [               ]
       [ParcelLoadReq]          [u.state=waiting]
                                [parcel_load_job]

 해결방안: state체크 및 변경과 job의 enqueue를 같은 mutex로 묶으면 된다.

*/