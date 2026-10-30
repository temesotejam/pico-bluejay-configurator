#include "esc_passthrough.h"

// Same motor wiring and channel order as temesotejam/pico-copter-fc.
// ESC 1 = FR GPIO15, ESC 2 = FL GPIO14,
// ESC 3 = RR GPIO3, ESC 4 = RL GPIO2.
static uint8_t kEscPins[] = {15, 14, 3, 2};

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  beginPassthrough(kEscPins, sizeof(kEscPins));
}

void loop() {
  // Keep the USB <-> BLHeli 4-way bridge alive after ESC Configurator
  // disconnects so users can reconnect without rebooting the Pico.
  // Only ESC configuration traffic; never send DShot motor throttle.
  processPassthrough();
}
