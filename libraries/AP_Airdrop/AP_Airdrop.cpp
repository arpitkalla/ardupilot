/*
 * AP_Aidrop is for controlling the baydoors and the drop actuators
 */
#include "AP_Airdrop.h"
#include "SRV_Channel/SRV_Channel.h"

#include <GCS_MAVLink/GCS.h>

#define AIRDROP_LOCATION_RADIUS 10 // Drop Location Radius

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

    Location current_loc;
    if (!AP::ahrs().get_position(current_loc)) {
        return;
    }

    SRV_Channels::set_output_limit(SRV_Channel::k_drop_actuator, SRV_Channel::Limit::MIN);
    _dropped = true;

    actual_drop_location.lat = current_loc.lat;
    actual_drop_location.lng = current_loc.lng;

    gcs().send_text(MAV_SEVERITY_INFO, "Airdrop: Dropping at %d, %d", current_loc.lat, current_loc.lng);
    gcs().send_text(MAV_SEVERITY_INFO, "Airdrop: Dropped");
}

void AP_Airdrop::set_drop_location(Location &loc) {
    drop_location.lat = loc.lat;
    drop_location.lng = loc.lng;
    gcs().send_text(MAV_SEVERITY_INFO, "Airdrop: Location set to %d, %d", drop_location.lat, drop_location.lng);
}

void AP_Airdrop::update() {
    if(!_armed) {
        return;
    }

    if(_dropped) {
        return;
    }

    if(!drop_location.initialised()) {
        return;
    }
    
    Location current_loc;
    if (!AP::ahrs().get_position(current_loc)) {
        return;
    }

    // Find the distance between current location and drop location in meters
    float dist = drop_location.get_distance(current_loc);
    
    if (dist <= AIRDROP_LOCATION_RADIUS) {
        drop();
    }
}

void AP_Airdrop::send_status(const GCS_MAVLINK &channel)
{
    mavlink_msg_airdrop_status_send(
        channel.get_chan(),
        AP_HAL::micros64(),
        _armed,
        _dropped,
        drop_location.lat,
        drop_location.lng,
        actual_drop_location.lat,
        actual_drop_location.lng
    );
}

namespace AP {

AP_Airdrop *airdrop()
{
    return AP_Airdrop::get_singleton();
}

};