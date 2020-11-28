#pragma once

// #include <AP_Param/AP_Param.h>
#include "AP_Common/Location.h"

class AP_Airdrop {
    public:
        AP_Airdrop();

        AP_Airdrop(const AP_Airdrop &other) = delete;
        AP_Airdrop &operator=(const AP_Airdrop&) = delete;

        static AP_Airdrop *get_singleton();
        static AP_Airdrop *_singleton;

        void update();
        void arm();
        void disarm();
        void drop();
        void set_drop_location(Location &loc);

        // static const struct AP_Param::GroupInfo        var_info[];

    private:
        // AP_Int8 _auto_enabled;
        bool _armed;
        bool _dropped;
        Location drop_location;
};

namespace AP {
    AP_Airdrop *airdrop();
};

