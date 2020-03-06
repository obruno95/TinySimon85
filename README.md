# tinySimon85
An ATtiny85 based Simon game.

The tinySimon85 is a simple clone of famous “Simon” game for training your visual and acoustic memory.

The aim of this project is to create a simple but still playable toy which can be used to learn basic skills in electronics.

The heart of the game is ATtiny85. It is low power AVR 8-bit microcontroller with 1 kB flash memory, 64 bytes EEPROM and 64 bytes SRAM.
It has six I/O pins, from which one acts as a reset and is used to start a new game, four are connected to LEDs and buttons and the last one leads to the magnetodynamic buzzer. The device is powered from CR2032 lithium cell.

As mentioned, the device contains four LEDs of different colors (red, blue, yellow and green) connected with four button switches. 

After pressing each of those buttons, the LED lights up and information about the button state can be read by a microcontroller. The pins of a microcontroller are bi-directional, so they can be changed from input to output in any time and can control LEDs instead of the buttons (see simplified diagram).

The high-impedance (42 ohm) buzzer is connected in series with a capacitor (to eliminate DC voltage) to the output of internal 8-bit timer/counter. Like in an original game, each color is producing a particular tone when it is pressed or activated by the microcontroller

The watchdog timer is used also for buttons debouncing and for auto power-off after a minute of inactivity. In power-down mode (also after the game is over) the consumption is approximately 0.15 microamps.

The concept of this electronic set is to make it easy to build without any special tools or skills. The single-sided printed circuit board is optimized to be easily drawn by hand with a permanent marker and etched in a solution of ferric chloride

The game can run in 3 modes

• Simon: The classic Simon (by Hasbro) game. A random increasing sequence of colors and tones is generated and you have to memorize and repeat that. If you make a mistake, all the LEDs will blink blinks three times showing the game-over animation.

• Echo: This is a multi-player variant of Simon. The first player presses one button. The next player repeats that button and then adds another button. This continues, each player reproducing the sequence so far, and then adding another button to the end of the sequence before passing the tinySimon to the other player. The first player to get the sequence wrong loses.

• Quiz: This uses the tinySimon for quiz games of up to 4 players. Each player has a button; the questionmaster reads a question out, and as soon as one player thinks they know the answer they press their button. Their tone sounds, and the light stays on for three seconds to confirm who got in first. The lights are then cleared ready for the next round.

** Components:
- 1x tinySimon85 PCB
- 1x ATtiny85 
- 4x LED (blue, yellow, green, red)
- 4x 56R resistor
- 5x push-button
- 1x 3V active buzzer
- 1x Capacitor 10uf
- 1x 3V Lithium button cell (CR2032)


Based on: 
- Simon (Genius) game (by Hasbro)
- GameBone Handheld Electronic Game (by David Johnson-Davies)
- Simon game with ATtiny13 (by Vojtak)
