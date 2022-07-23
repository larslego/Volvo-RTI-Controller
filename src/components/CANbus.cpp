#include "components/CANbus.h"
#include <mcp2515.h>
#include "pro_micro_pins.h"
#include "canbus_ids.h"
#include "canbus_msgs.h"

struct can_frame canMsg;
MCP2515 mcp2515(CANBUS_CS_PIN);

CANbus::CANbus() {
    mcp2515.reset();
    mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
    mcp2515.setConfigMode();
    mcp2515.setFilterMask(MCP2515::MASK0, true, 0x00000000);
    mcp2515.setFilter(MCP2515::RXF0, true, 0x00000000);
    mcp2515.setFilter(MCP2515::RXF1, true, 0x00000000);
    mcp2515.setFilterMask(MCP2515::MASK1, true, 0x00000000);
    mcp2515.setFilter(MCP2515::RXF2, true, 0x00000000);
    mcp2515.setFilter(MCP2515::RXF3, true, 0x00000000);
    mcp2515.setFilter(MCP2515::RXF4, true, 0x00000000);
    mcp2515.setFilter(MCP2515::RXF5, true, 0x00000000);
    mcp2515.setListenOnlyMode();
}

void CANbus::update() {
    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
        if ((canMsg.can_id & 0x1FFFFFFF) == CEM) {
            if (canMsg.data[4] == KEY_0) data.keyPosition = 0;
            else if (canMsg.data[4] == KEY_1) data.keyPosition = 1;
            else if (canMsg.data[4] == KEY_2) data.keyPosition = 2;
            else if (canMsg.data[4] == NO_KEY) data.keyPosition = -1;
        }
        if ((canMsg.can_id & 0x1FFFFFFF) == GEARBOX) {
            if (canMsg.data[2] == 1) data.currentGear = -1;
            else data.currentGear = 0;
        }
        if ((canMsg.can_id & 0x1FFFFFFF) == STEERING_WHEEL_MODULE) {
            if (canMsg.data[7] != 0x3F) data.currentVersionByte = canMsg.data[1];

            if (canMsg.data[6] == RTI_BUTTON_LEFT) data.currentButton = DPAD_LEFT;
            else if (canMsg.data[6] == RTI_BUTTON_RIGHT) data.currentButton = DPAD_RIGHT;
            else if (canMsg.data[6] == RTI_BUTTON_UP) data.currentButton = DPAD_UP;
            else if (canMsg.data[6] == RTI_BUTTON_DOWN) data.currentButton = DPAD_DOWN;
            else if (canMsg.data[6] == RTI_BUTTON_ENTER) data.currentButton = DPAD_ENTER;
            else if (canMsg.data[6] == RTI_BUTTON_BACK) data.currentButton = DPAD_BACK;
            else if (canMsg.data[7] == VOLUME_UP) data.currentButton = SW_VOLUME_UP;
            else if (canMsg.data[7] == VOLUME_DOWN) data.currentButton = SW_VOLUME_DOWN;
            else if (canMsg.data[7] == PREVIOUS_TRACK) data.currentButton = SW_REWIND;
            else if (canMsg.data[7] == NEXT_TRACK) data.currentButton = SW_FAST_FORWARD;
        }
    } else {
        // If no data is received, assume the car is turned off.
        data.keyPosition = -1;
        data.currentGear = 0;
    }
}