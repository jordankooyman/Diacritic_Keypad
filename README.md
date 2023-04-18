# Diacritic_Keypad
An Arduino-based custom keyboard for more easily typing diacritic letters, particularly on Windows, but with support for many Linux distros as well.

Written for Arduino Pro Micro/Arduino Leonardo boards
Intended for use with 17 buttons and a switch, all connected to the arduino via chained shift registers
![diacritc keypad finished perspective view](https://user-images.githubusercontent.com/62574509/198838440-baff0467-b2a4-4df5-b820-2f10c46f2254.jpg)

Bill of Materials (BOM):

    1x  RGB LED (common negative) for status indication at runtime
    17x Cherry MX Red keyswitches were used in my build, but any Cherry MX keyswitch should be compatible
    1x  Power switch style toggle switch (acts as Linux/Windows mode switch)
    3x  74HC165 Parallel to serial Shift Registers were used
    3x  0.1uF ceramic capacitors to stabalize shift registers
    1x  1uF electrolytic capacitor to handle larger current spikes - exact value not too important, can be larger
    1x  Arduino Pro Micro
    1x  6ft Micro USB to USB cable (glued in)
    19x 10k Resistors
        26awg Enamled copper wire for keyswitch connections
        Copper Foil tape for positive and negative bus bars (optional, but recommended)
        24awg Insulated copper wire for connections to arduino
        Pin headers to connect to arduino (optional)
        Hot Glue to hold in keyswitches and insulate shift registers
    4x  M3 brass heat-set inserts
    4x  M3 by 8mm bolts to hold everything closed
    1x  Case Set (case and insert) - must be 3D Printed (material choice not important)
    4x  Rubber Feet (optional)
    17x Keycaps (can be 3D Printed (material not important), sample files included)
![diacritc keypad finished top view](https://user-images.githubusercontent.com/62574509/198838486-0e204455-44b1-4137-bf65-7c68cd92f01a.jpg)


All CAD files and example wiring pictures are included.
A sample set of keycaps is also included.
The two component reference pictures were sourced online, all other materials were made by Jordan Kooyman.

Basic wiring:  (Look at Example Wiring Photos folder included in this repo for an example)

    Every keyswitch (and toggle switch) is connected to GND (0V) directly.
    The other terminal on every keyswitch (and toggle switch) is connected to 1 data input pin on a shift register, with a 10k resistor also connecting that to Vcc (5V)
    The clock input pin on all 3 shift registers is connected together
    The parallel load input pin on all 3 shift registers is connected together
    The serial output pin of the shift register with only 2 connections is connected to the serial input pin of one of the other 2 shift registers
    Then, the serial output pin of that shift register is connected to the serial input pin of the final shift register
    The serial output, clock input, and parallel load input pins are all connected to I/O pins of the arduino
    Each shift register also needs the Vcc pin connected to Vcc (5V), GND to GND (0V), and Clock Enable connected to GND (0V)
    The common negative pin of the RGB LED is connected to a 10k resistor that is then connected to GND (0V)
    The other 3 pins of the RGB LED are connected to the I/O pins of the arduino
    The GND (0V) bus is conencted to a GND pin on the arduino
    The Vcc (5V) bus is conencted to the Vcc pin on the arduino
        Note: I recommend leaving some extra length on the wires that need to go from the top to the bottom (or vice versa) to
        make working on this easier (but make sure the case can still close)
    The electrolytic capacitor is connected between VCC (5v) and GND (0V) busses somewhere (ensure correct polarity, strip is negative pin)
    1 ceramic capacitor is connected between GND (0V) and the Vcc input pin of each shift register

Here is a video of me wiring my keypad together, uncut but running at 2x speed: https://youtu.be/HU2RTQba6jM

Due to the variability of wiring and desired character outputs, the code included is incomplete and all character combinations must be written in manually.
If you have any questions regarding how to modify the code or wire things together, feel free to reach out to me at jordankooyman@gmail.com

# Prevent Code Uploads
This keypad, as currently designed, will always still appear as an arduino device when connected to a computer. To change how it appears, but not restrict reprogrammability at all, the VID and PID values can be modified for the profile being uploaded to the board. To restrict reprogramming, some COM ports related to the attatched can be disabled in Windows, but it is local to each PC. To better restrict programming, an external ISP such as a usbTinyISP can be used to upload a the code and overwrite the bootloader of the Arduino Pro Micro. Note that overwriting the bootloader means that the ISP must be used to upload code to the board again in the future, until a bootloader is reloaded. Traditional methods of disabling sketch uploading to arduino boards do not work for the Atmega32u4 since it has built-in usb support rather than using an external chip, however this may be different if you are using a different microprocessor. For more information, please do your own research for the specific board you are using.

# Design and Wiring Advice
When designing your own case, it is important to keep in mind both how to assemble it and where all the wiring will go inside. Make sure to leave enough room on the inside to fit all the wires and the microcontroller such that no wires inadvertently short out once assembled, which may cause permanent damage to the microcontroller or worse. To make this easier, it is recommended to make the whole case from plastic rather than metal. If making the case from metal, make sure to insulate all connections to prevent short circuits (and be careful using the copper foil tape bus bar design demonstrated above since those tapes often feature a conductive adhesive). Before doing the final assembly of the project, make sure to carefully assembly throughout the build process to ensure everything will still fit together and nothing will short circuit. It is also recommended to test the wiring throughout, which can be accomplished by uncommenting the TestMode line in the code provided (it is uncommented as provided here) before testing all the parts believed to be fully functional while the device is plugged into a computer with Arduino IDE and the serial monitor open on the correct port.
While wiring, try to keep the wires organized. It is recommended to mark them if they are not unique, and to write down where each one goes until it can be fully connected. Also ensure the wires are long enough to reach to their destination and the device can still be opened, but not so long that there is a lot of extra wire when attempting to assemble the device.

# Testing Guide
To test operations of the device as well as to determine the button layout, the built-in TestMode allows you to see what the microcontroller reports when a button is pressed. This can be seen with the serial monitor running (included in Arduino IDE) with the device connected to that computer via USB. If any anomalies appear, such as a button not showing it is pressed, or always being pressed, or 1 button being pressed shows multiple buttons being pressed, the first thing that should be done is to verify the wiring is correct. Do this by using a multimeter with a continuity test and check both connections that should exist and connections that should not. If nothing can be found, check again with the serial monitor while the device is open for testing, as short circuits may occur only while assembled. If no faults can be found in the wiring, check the code to ensure it is reading the buttons correctly, particularly any changes you have made. Another common issue is not updating the pins that are used, which can lead to erratic behavior.

# Disclaimers
This project is licensed under GPLv3. As such, you are free to use this project and modify it as you see fit, both privately and commercially. However, if used commercially, the product must disclose this project as its source as well as all changes made. For more information, please check out the license directly.
You are fully responsible and liable for any faults, issues, damage, or other problems for any devices you make. Make at your own risk.
