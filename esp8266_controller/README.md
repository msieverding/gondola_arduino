# gondola
A stand-alone version of Gondola for ESP8266 (wemos D1 mini) (system controller and motor controller are both on the Chip)

INSTALLATION
- upload .ino to your Mega
- Gondola initially will open an access point
- Other connection types can be setted up through it's webpage.
- Serial(115200 bps) command "help" to show all comamnds
- Serial(115200 bps) command "move x y z s" to move gondola to coordinate (x, y, z) with speed s (in cm/s)

CONFIGURE
In the Config.cpp, you can change the number of anchors, their position, the starting position of Gondola and the pin connected to the stepper controllers.

INFO ON GONDOLA
http://arxiv.org/abs/1601.07457
