Robin, the Caps-lock indicating robot controller

NOTE: Compile using keil ARMCC (due to size of code)
mbed library modified to exclude the Stream class from USBKeyboard: this is too large


To do so: modify line 92 of USBKeyboard.h from:

	class USBKeyboard: public USBHID, public Stream {

to

	class USBKeyboard: public USBHID {

recompile your MBED lib, and you are good to go.

Peter.
 