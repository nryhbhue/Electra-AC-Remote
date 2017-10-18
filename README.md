# Electra-AC-Remote
This little library generates raw IR commands for your Electra air conditioner using Raspberry Pi
![electra remote](https://user-images.githubusercontent.com/29211431/29035330-d7b80a4e-7ba3-11e7-8573-9e5ed1d00ba0.jpg)

Background:
-----------
The IR protocol in this case uses Manchester coding. Basically, each 0 -> 1 transition in the generated IR signal is translated to 0 and each 1 -> 0 transition is translated to 1. The basic time frame for translation is a time period T = 2000ms with 50% duty cycle.

By using lirc with an IR receiver you'd get a series of space-pulse pairs. For example:

`2996     2986      950     1030      953     1046     955     2060     1918     1037      936     1055`

Instead of using the graphical representation of these pairs let's try a different approach where each space will be translated to 0's  and each pulse to 1's with a basic time unit of 1000ms (T/2). We'll get: 

`000 111 0 1 0 1 0 11 00 1 0 1`

This is the binary representation of the Manchester coding. Now let's take each pair and translate it to 'regular' binary as explained above:
(The first pair is omitted - this is the header of the signal)

`0 0 0 1 0 0`

And that's it, by repeating this process with entire IR commands it's possible to understand what each part in the binary string represents and encode it back to space-pulse pairs with the desired command.

What you'll need:
-----------------
* Raspberry Pi
* Small IR led circuit (google how to build one with a transistor, there are numerous instructables)
	* Parts (usually):
		* IR led (take apart an old remote and start desoldering or just order a batch from ebay. MIND THE POLARITY!)
		* Transistor (once again, if you're impatient take apart a power saving bulb)
		* Resistor (check the transistor's datasheet in order to understand which resistor to use, you don't want to draw too much current from your Pi or burn your led)
* Optional: blynk account (preferably on a local server for maximum energy) 
		
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
	* Arguments: Fan, Mode, Temperature, State, Swing
	
	  For example: `./electraAcRemote 2 COOL 25 ON SWING_OFF`
	 
	 * The ON state is used for changing settings while the AC is already up and running
	 * The OFF state turns the AC on and off just like the power button on an actual remote
	 
ESP8266 Support:
----------------
I added an arduino sketch to use with an esp8266 module, this also includes the Blynk implementation below. There is no need in "ir-slinger"'s library in this case, just add "electraAcRemoteEncoder.c" (change the extension to ".cpp" if needed) and "electraAcRemoteEncoder.h" files to your library in the Arduino IDE and flash your module after providing the Wifi and Blynk data.
The circuitry is the same as for the Raspberry Pi, but this time around the IR led is driven by the esp8266's gpio. I used an esp8266-01 module to test this, it's very limited gpio-wise so I connected the RX pin to the transistor's base since it's okay for it to be low if the module resets for some reason (as opposed to gpio 2 which will not enable the module to boot if it's pulled low).

Integration with blynk:
-----------------------
[Blynk](https://github.com/blynkkk) is a great way to get you started with a comfortable interface for this remote.
Here's how I set up mine:
![electra ac remote - cool](https://user-images.githubusercontent.com/29211431/29248242-180dd9bc-801c-11e7-8c08-30401ecb22a9.png)
![electra ac remote - heat](https://user-images.githubusercontent.com/29211431/29248243-1833ab74-801c-11e7-9f41-de71a279608c.png)
