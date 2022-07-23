#include "components/RTIRemote.h"
#include <IRRemote.h>
#include "pro_micro_pins.h"

IRrecv receiver;
decode_results results;

RTIRemote::RTIRemote(HIDControl* hid) {
  receiver.enableIRIn();
  receiver.begin(RTI_IR_PIN);
  this->hid = hid;
}

void RTIRemote::update(unsigned long currentMillis) {
  if (receiver.decode()) {
    for (int i = 0; i < IR_BUTTON_COUNT; i++) {
      if (receiver.decodedIRData.decodedRawData == irRemoteKeys[i]) {
        // If the new key (compared to previously pressed key) is the NOT same, OR the report timeout has passed, report the new key. 
        if ((lastPressedKey == irKeyboardKeys[i] && (currentMillis - lastKeyReportTime > IR_REMOTE_REPORT_DELAY)) || lastPressedKey != irKeyboardKeys[i]) {
          // Report the key to the host.
          lastPressedKey = irKeyboardKeys[i];
          lastKeyReportTime = currentMillis;

          // Determine if the key is a button or dpad button.
          if (i >= 4) hid->pressGamePadButton(irKeyboardKeys[i]);
          else hid->pressGamePadHatSwitch(irKeyboardKeys[i]);
          break;
        }
      }
    }

    receiver.resume();  // Receive the next value
  }
}