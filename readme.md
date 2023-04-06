# Bluetooth Remote Control for [Lego Technic Hub](https://www.lego.com/de-at/product/technic-hub-88012?cmp=KAC-INI-GOOGEU-GO-AT-DE-SP-BUY-SHOP-PLA-BP-SP-RN-Shopping&ef_id=EAIaIQobChMIm9fm7N_D6wIVvSB7Ch3OtwRWEAQYAiABEgI_DvD_BwE:G:s&s_kwcid=AL!933!3!288221002759!!!g!296439327739!)

This is an arduino sketch to control the assembled LEGO Technic vehicles via the Bluetooth LE interface and RC controller.

## Requirements
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [Arduino NANO 33 BLE Sense](https://store.arduino.cc/arduino-nano-33-ble-sense) ([Getting Started Guide](https://www.arduino.cc/en/Guide/NANO33BLESense))
- [RC controller](https://www.rcprofi.sk/engine-diel-c-2-ovladac)

## Installation
Just upload the sketch with your preferred IDE to the board. If you want to have debug information you might uncomment the `#define DEBUG` line in debug.h. If you do so and upload it to the remote it will wait after powered on until a serial terminal is connected.

## Current supported vehicles
- Lego Technic Mercedes-Benz Zetros 4 x 4 42129

## Initialization Sequence
ATTENTION: After you powered on the remote and pushed the green button on the Technic HUB the vehicle will automatically steer to the left and right to measure the steering range and center position.

Power on your remote (Arduino Board). The easiest way might be to supply it via USB. Now it will scan for BLE Devices with the name **Zetros Hub**. If the central found one it will connect to it immediately.  
The remote will send some setup commands to the hub so the two motors on port A and B get mapped to one virtual port. For the last step the steering range and the central position for the steering will be initialized. 

## Useful Links
- [LEGO BLE Protocol](https://lego.github.io/lego-ble-wireless-protocol-docs/index.html#document-index)  
- If you use Microsoft Edge as your preferred browser use `edge://bluetooth-internals/#adapter` in case of Google Chrome `chrome://bluetooth-internals/#adapter` to investigate your bluetooth peripheral
