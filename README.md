# War and Glory

War and glory is a 3D printed tank controlled using an Arduino UNO, a smart phone
and a BLE module. The tank uses FreeRTOS with two task, one for handling ble
communications and another to control the motors. Motor control directions
are sent via a queue from the ble task to the motor controller.

## Material

1. 3D printer files created by [otvinta3d](https://www.thingiverse.com/thing:2319872)
2. Adafruit Bluefruit LE SPI Friend - Bluetooth Low Energy (BLE)
3. 30 RPM DC 6V 2.5KG Micro Gear Box Speed Reducing Motor
4. L293D DIP 16-pin IC Stepper Motor Drivers Controllers
5. A lot of wires

## Communications

Controlling War and glory is done through the Adafruit Bluefruit LE Connect
in controller mode using the control pad interface. The arrows control the
direction of the tank and the number pad the speed of the motors.
