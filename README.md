# Ardu
Autonomous multi mode Arduino robot



Equipment Used: <br>
2 servos half rotation converted to full rotation by sticking the potentiometer to 90 degrees <br>
3 Ultrasonic sensors in front. With a few resistors in order to work with the arduino due <br>
1 lcd display with a wire underneath for the buttons to work.<br>
1 pololu IR Beacon<br>
1 Arduino Mega<br>
1 bluetooth tranciever<br>
1 digital compass<br>

##Modes:

###Bluetooth:
App: Arduino Bluetooth Controller (Estado's ltd.)<br>
Set keys to:<br>
Up: U<br>
Left: L<br>
Right: R<br>
Down: D<br>
Bluetooth Mode: B<br>
Follow Mode: F<br>
Explore Mode: E<br>

###Wifi:

Go to knejad.co.uk/ardu-server/ardu.php<br>

Script syntax is:<br>
{Direction}{Amount};{Direction}{Amount};<br>
eg: L200;R100;<br>

Directions:<br>
L: Left (Degrees)<br>
R: Right (Degrees)<br>
F: Forwards (Milliseconds)<br>
B: Backwards (Milliseconds)<br>

###Follow:
Follows a polulu IR sensor which the user holds<br>

###Explore:
Explores on its own whilst avoiding obstacles<br>
