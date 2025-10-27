# BALANCING SCALE
A pendulum scale that is part of the pirate shiproom prop.
It is used to measure the weight of the different items in the puzzle.

The scale is using RFID technology to determine the pre-designated items
that are placed on the scale(plates), rather than measuring the actual weight. 

## RFID Readers
 The RFID readers used in this setup are two [ID-12LA](https://www.digikey.com/en/products/detail/sparkfun-electronics/11827/7364971); one for each plate. They are 125kHz RFID reader modules by ID-innovations and can be sourced most notibly from [DigiKey](https://www.Digikey.com) and [Sparkfun](https://www.sparkfun.com/).
The corresponding cards, stickers, fobs, etc. to be used with this reader must be 125kHz compatible to get a proper response from the reading module.

## Controller
The controller for this setup is the Arduino Nano. Its small compact size allow for it to conveniently be placed inside the scale's drawer. It is responsible for gathering and processing the information from the RFID readers.

## Breakout Board
A custom printed circuit board that was designed to receive the 2mm pitch pins from the RFID reader and provide a few additional pins to connect with the reader.

## 3D Models
The RFID reader is uncapable of properly broadcasting a signal through metal plates; therefore a 3D model was designed to be used. Within the plate design, a guide for the RFID cover housing was attached underneath the plate. The RFID readers while being placed under the plates, should not be exposed, therefore a cover housing was designed. They are connected to the plate using screws.  

## Algorithm
1. Plate 1 Listen for any rfid transmitter near by.
2. If there is no rfid being scanned, leave the sub routine. 
3. If there is a rfid in range, take the value and store 
it in a temporary variable. 
4. Check if the value is a valid pre-designated ID. 
5. If it is not a valid ID, leave the sub routine.
6. If it is a valid ID, check if it was already stored for 
the corresponding plate. 
7. If it was already stored, leave the sub routine. 
8. If it was not already stored, store the new ID for the 
corresponding plate.
9. Follow the same steps from 1-8 for plate 2. 
10. Check for any succession. 
11. If there was a succession, illuminate the green LED,
otherwise maintain the red LED.
12. Go back to step 1.

## Connections
There are a total of 3 modules in the Balancing Scale. Two rfid reader modules and an Arduino Nano. The ID-12LA rfid reader modules, is configured to output ASCII format via the D0 pin, when the FORMAT pin is tied to ground. The reset pin on the reader modules is tied to the supply voltage. The power connections (Vsupply and GND) to the rfid reader modules comes from the Arduino Nano. The D0 pins from the reader modules both get their own pin on the Arduino Nano. An RGB LED is added as an indicator. The red and green leads of the LED is tied via 1k resistors to the Arduino Nano. And, the final connection is the power to Arduino Nano comes in via the USB port. 
