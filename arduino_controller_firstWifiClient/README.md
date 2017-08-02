# gondola
A stand-alone version of Gondola for arduino mega (system controller and motor controller are both on the arduino)

INSTALLATION
- upload .ino to your Mega 
- connect via serial (115200 bps) and send coordinates in the following format "x y z s ". Note that each values is a float and s is the speed in cm/s. Note also that the string must end with a space (this is due to un unsolved bug on string tokenizer)

CONFIGURE
In the .ino code, you can change the number of anchors, their position, the starting position of Gondola and the pin connected to the stepper controllers.

INFO ON GONDOLA
http://arxiv.org/abs/1601.07457
