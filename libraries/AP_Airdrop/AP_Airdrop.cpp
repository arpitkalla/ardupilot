/*
 * AP_Aidrop is for controlling the baydoors and the drop actuators
 */
#include "AP_Airdrop.h"
#include "SRV_Channel/SRV_Channel.h"

#include <GCS_MAVLink/GCS.h>

extern const AP_HAL::HAL& hal;

// const AP_Param::GroupInfo AP_Airdrop::var_info[] = {
//     // @Param: AUTO_ENABLE
//     // @DisplayName: Automatic Airdrop Enable/Disable
//     // @Description: Automatic Airdrop enable/disable
//     // @User: Standard
//     // @Values: 0:Disabled, 1:Enabled
//     AP_GROUPINFO_FLAGS("AUTO_ENABLE", 0, AP_Airdrop, _auto_enabled, 0, AP_PARAM_FLAG_ENABLE),
//     AP_GROUPEND
// };

AP_Airdrop::AP_Airdrop()
{
    if (_singleton) {
        return;
    }
    _singleton = this;

    // AP_Param::setup_object_defaults(this, var_info);
}

AP_Airdrop *AP_Airdrop::_singleton = nullptr;
AP_Airdrop *AP_Airdrop::get_singleton()
{
    return _singleton;
}


void AP_Airdrop::arm() {
    SRV_Channels::set_output_limit(SRV_Channel::k_baydoors, SRV_Channel::Limit::MAX);
    _armed = true;
    gcs().send_text(MAV_SEVERITY_INFO, "Airdrop: Armed");
}

void AP_Airdrop::disarm() {
    SRV_Channels::set_output_limit(SRV_Channel::k_baydoors, SRV_Channel::Limit::MIN);
    _armed = false;
    gcs().send_text(MAV_SEVERITY_INFO, "Airdrop: Disarmed");
}

void AP_Airdrop::drop() {
    if (!_armed) {
        gcs().send_text(MAV_SEVERITY_INFO, "Airdrop: Failed to drop - not armed");
        return;
    }
    
    if (_dropped) {
        gcs().send_text(MAV_SEVERITY_INFO, "Airdrop: Already dropped");
        return;
    }


    SRV_Channels::set_output_limit(SRV_Channel::k_drop_actuator, SRV_Channel::Limit::MIN);
    _dropped = true;
    gcs().send_text(MAV_SEVERITY_INFO, "Airdrop: Dropped");
}

void AP_Airdrop::set_drop_location(Location &loc) {
    drop_location = loc; 
}

namespace AP {

AP_Airdrop *airdrop()
{
    return AP_Airdrop::get_singleton();
}

};