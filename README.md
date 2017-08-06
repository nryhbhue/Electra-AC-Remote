# Electra-AC-Remote
This little library generates raw IR commands for your Electra air conditioner (Raspberry Pi)

Supporterd features:
--------------------
* Mode [COOL/HEAT]
* Temperature [16-30]
* Fan [1-4], with 4 being Auto
* State [ON/OFF]

Dependencies and Usage:
-----------------------
* This code was designed to work with ["ir-slinger"](https://github.com/bschwind/ir-slinger)
	* Follow the instructions to get "pigpio" - this library handles the pi's GPIOs
* Clone "Electra-AC-Remote" and "ir-slinger"
* Copy "irslinger.h" to the cloned "Electra-AC-Remote" directory
* Compile (change the IR-transmitter's gpio in "electraAcRemote.c" if needed):
	
	gcc electraAcRemote.c electraAcRemoteEncoder.c -o electraAcRemote -lm -lpigpio -pthread -lrt

* Once this is done you're free to test your AC remote:
	* Arguments: Fan, Mode, Temperature, State
	
	  For example: ./electraAcRemote 2 COOL 25 ON
	 
	 * The ON state is used for changing settings while the AC is already up and running
	 * The OFF state turns the AC on and off just like the power button on an actual remote
