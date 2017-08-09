# Electra-AC-Remote
This little library generates raw IR commands for your Electra air conditioner (Raspberry Pi)
![electra remote](https://user-images.githubusercontent.com/29211431/29035330-d7b80a4e-7ba3-11e7-8573-9e5ed1d00ba0.jpg)

Supporterd features:
--------------------
* Mode [COOL/HEAT]
* Temperature [16-30]
* Fan [1-4], with 4 being Auto
* Swing [SWING_ON/SWING_OFF/SWING_SINGLE]
* State [ON/OFF]

Dependencies and Usage:
-----------------------
* This code was designed to work with ["ir-slinger"](https://github.com/bschwind/ir-slinger)
	* Follow the instructions to get "pigpio" - this library handles the pi's GPIOs
* Clone "Electra-AC-Remote" and "ir-slinger"
* Copy "irslinger.h" to the cloned "Electra-AC-Remote" directory
* Compile (change the IR-transmitter's gpio in "electraAcRemote.c" if needed):

	`gcc electraAcRemote.c electraAcRemoteEncoder.c -o electraAcRemote -lm -lpigpio -pthread -lrt`

* Once this is done you're free to test your AC remote:
	* Arguments: Fan, Mode, Temperature, State
	
	  For example: `./electraAcRemote 2 COOL 25 ON SWING_OFF`
	 
	 * The ON state is used for changing settings while the AC is already up and running
	 * The OFF state turns the AC on and off just like the power button on an actual remote

Integration with blynk:
-----------------------
[Blynk](https://github.com/blynkkk) is a great way to get you started with a comfortable interface for this remote.
Here's how I set up mine:
![blynk - ac remote - heat](https://user-images.githubusercontent.com/29211431/29036057-8ce84be8-7ba6-11e7-9676-90ed96466e1a.png) ![blynk - ac remote - cool](https://user-images.githubusercontent.com/29211431/29036058-8d9b1908-7ba6-11e7-9e43-50c29af920dc.png)
