Several features of the current schematic were removed / disabled in order to improve the power consumption
Changes implemented in the arduino:

1. LDO removed, thus external supply voltage other than the USB is not possible
2. Apart from the built-in LED, all other LEDs were removed.
3. Supply voltage of the CH340 was removed


Changes implemented in the peripheral circuitry:

1. ESP8266 WIfi module removed
2. Water level meter disabled