# Fanbot Registration System

Bits and pieces needed to register FanBots and print their "passports":

* **images/** contains various images used by the master or the slaves
* **master-node/** is the Node.js code running on the print & display master
* **slave-lcd/** is a utility to manage the LCD screen and touch sensor
* **slave-usbot/** is a modified fanbot.cpp version to read out FanBot info

The master code runs on a Mac OSX laptop ('cause Node.js drove the CPU to 100%
utilisation on the Acer Inspire One Linux netbook I originally wanted to use).
It is started by running "./master-go.sh", and uses AppleScript for printing.

The slave code runs on Raspberry Pi's and requires a GPIO-connected LCD display
plus USB webcam. Started via "./slave-go.sh" on power-up (using cron).

For a bit of technical info, see the JeeLabs weblog:

* http://jeelabs.org/2013/06/12/lcd-screen-for-the-rpi/
* http://jeelabs.org/2013/06/13/getting-an-image-on-the-lcd/
* http://jeelabs.org/2013/06/14/what-it-took-to-generate-that-lcd-image/
* http://jeelabs.org/2013/06/21/cheap-webcam/
* http://jeelabs.org/2013/06/25/fanbots-at-robocup-2013/
* http://jeelabs.org/2013/06/27/meet-the-fanbot/
* http://jeelabs.org/2013/06/29/fanbot-registration-system/

For project details and results, check out http://www.kekbot.org/ - enjoy!
