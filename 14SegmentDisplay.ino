/*
*
*  Project:     14 Segment Display with an WS2812B LED Strip
*  @author:     Leon van den Beukel
*  @link:       https://github.com/leonvandenbeukel/14-Segment-LED-Display
*  @license:    MIT - Copyright (c) 2018 Leon van den Beukel
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <SoftwareSerial.h>
#include <FastLED.h>
#include "Ascii.h"

#define NUM_LEDS 15
#define DATA_PIN 6
#define DELAY_TIME 600
#define CMD_BUFFER_SIZE 25

CRGB LEDs[NUM_LEDS];
CRGB color = CRGB::White;
SoftwareSerial BTserial(8, 9);
uint8_t currentColorIndex = 0;

void setup()
{
	Serial.begin(9600);
	while (!Serial) { /* Wait until serial is ready */ }

	// Initialize LED strip
	FastLED.delay(3000);
	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(LEDs, NUM_LEDS);

	BTserial.begin(9600);
	Serial.println("BTserial started at 9600");
}

void loop()
{
	if (Serial.available()) {
		processCharacter(Serial.read());
	}
	else {
		waitForInput();
	}

	if (BTserial.available())
	{
		processCharacter(BTserial.read());
	}
	else {
		waitForInput();
	}
}

void processCharacter(int character) {

	if (character == 126) {
		setNextColor();
		displayCharacter('.');
	}
	else {
		displayCharacter(character);
	}

	delay(DELAY_TIME);
}

void displayCharacter(int character) {

	int index = character - 32;
	uint16_t v = FourteenSegmentASCII[index];

	for (int i = 0; i < NUM_LEDS; i++)
	{
		uint16_t bit = 1 << i;
		uint16_t bitcheck = (v & bit) == bit;
		LEDs[i] = bitcheck == 1 ? color : CRGB::Black;
	}

	FastLED.show();
}

void waitForInput() {
	displayCharacter(' ');
	delay(100);
}

void setNextColor()
{
	currentColorIndex++;
	if (currentColorIndex == 7) {
		currentColorIndex = 0;
	}

	switch (currentColorIndex)
	{
	case 0:
		color = CRGB::White;
		break;
	case 1:
		color = CRGB::Red;
		break;
	case 2:
		color = CRGB::Green;
		break;
	case 3:
		color = CRGB::Blue;
		break;
	case 4:
		color = CRGB::Purple;
		break;
	case 5:
		color = CRGB::Cyan;
		break;
	case 6:
		color = CRGB::BlueViolet;
		break;
	default:
		color = CRGB::White;
		break;
	}
}