# Airdrop

## Implementation

We create new MAVLink messages to set location, arm, disarm and drop the payload while flying. Additionally we introduce a new servo function for ease of setting the max and min PWMs.

There are two ways to call the drop: a) One is manually by sending the drop message and b) automatically as the plane calculates the location of when to drop automatically.

## Todo
- [x] Create MAVLink Messages

    Added the following Mavlink Messages:
    ```
    MAV_CMD_DO_AIRDROP
    MAV_CMD_DO_SET_AIRDROP_LOCATION
    ```  
    with `AIRDROP_ACTIONS` enum which stores the the different actions like `AIRDROP_ACTION_DISARM` , `AIRDROP_ACTION_ARM` , `AIRDROP_ACTION_DROP`. 
    Try running these commands in the MAVProxy console:
    ```
    long 225 0 1 0 0 0 0 0
    long 226 0 0 0 0 10 11 0 
    ```

- [x] Create the servo functionality

    Can set the `SERVOn_FUNCTION` to `Baydoors` (134) and `Drop Actuator` (135).
    Baydoors MAX trim is to open and and the MIN is to close.
    Drop Actuators MAX is to keep the payload locked, MIN will retract the actuator for a drop.
    Try setting the param using the frontend or type this in the MAVProxy console:
    ```
    param set SERVO10_FUNCTION 134
    param set SERVO11_FUNCTION 135
    ```

- [x] Create basic dropping functionality

    Added a constant radius, if the plane is within the circle, drops the payload automatically. Runs on a 50Hz update loop for Planes.
    For very bare-bones testing put the folloing in the MAVporxy console when running with SITL:
    ```
    long 225 0 1 0 0 0 0 0
    long 226 0 0 0 0 -35.3632621 149.1652374 0 
    ```
    The first line arms the airdrop system, and the second sets the drop location to the planes default location, so it should instant drop and display a message.

- [x] Add status message

    Need to send a continuous message to the GCS to share the state and the set location of the Airdrop system.
    Added the message `AIRDROP_STATUS` which contains the information about arming, has dropped and the set location and the location it was actually dropped.

- [ ] Add logic for dropping it smartly

- [ ] Create a parameters for Airdrop

- [ ] Write tests for different scenarios
