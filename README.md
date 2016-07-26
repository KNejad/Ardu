# Ardu
Autonomous multi mode Arduino robot



Equipment Used:
2 servos half rotation converted to full rotation by sticking the potentiometer to 90 degrees
3 Ultrasonic sensors in front. With a few resistors in order to work with the arduino due
1 lcd display with a wire underneath for the buttons to work.
1 pololu IR Beacon
1 Arduino Mega
1 bluetooth tranciever
1 digital compass



#Bluetooth:
App: Arduino Bluetooth Controller (Estado's ltd.)
Set keys to:
Up: U
Left: L
Right: R
Down: D
Bluetooth Mode: B
Follow Mode: F
Explore Mode: E

#Wifi

go to knejad.co.uk/ardu-server/ardu.php

Script syntax is:
<Direction><Amount>;<Direction><Amount>;
eg: L200;R100;

Directions:
L: Left (Degrees)
R: Right (Degrees)
F: Forwards (Milliseconds)
B: Backwards (Milliseconds)


