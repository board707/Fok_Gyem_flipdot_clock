#include "Arduino.h"
#include "TM1638_Module.h"

byte digit[] = {
 0b00111111,  // 0
 0b00000110,
 0b01011011,
 0b01001111,
 0b01100110,
 0b01101101,
 0b01111101,
 0b00000111,
 0b01111111,
 0b01101111,   // 9
 0b00000000,  // clear all
 0b01000000,  // minus "-"
	};



TM1638_Module::TM1638_Module(const uint8_t strobe,
	const uint8_t clock,
	const uint8_t data): strobe(strobe),clock(clock),data(data) {

	}

TM1638_Module::~TM1638_Module() 
	{}

void TM1638_Module::init() {
	pinMode(strobe, OUTPUT);
	pinMode(clock, OUTPUT);
	pinMode(data, OUTPUT);

	sendCommand(0x8f);  // activate and set brightness to max
	reset();
	}

void TM1638_Module::sendCommand(uint8_t value)
	{
	digitalWrite(strobe, LOW);
	shiftOut(data, clock, LSBFIRST, value);
	digitalWrite(strobe, HIGH);
	}

void TM1638_Module::tm1638_led(byte pos, byte state) {
	if ((pos > 7) || (state > 1)) return;
	sendCommand(0x44);  // set single address
	digitalWrite(strobe, LOW);
	shiftOut(data, clock, LSBFIRST, (0xc1 + pos * 2)); // 3rd LED
	shiftOut(data, clock, LSBFIRST, state);
	digitalWrite(strobe, HIGH);
	}

void TM1638_Module::tm1638_num(byte pos, byte num, bool dp) {
	if ((pos > 7) || (num > 11)) return;  // 0-9, space, minus/dash
	byte out = digit[num];
	if (dp) out |= 0x80;
	sendCommand(0x44);  // set single address
	digitalWrite(strobe, LOW);
	shiftOut(data, clock, LSBFIRST, (0xc0 + pos * 2)); // 3rd LED
	shiftOut(data, clock, LSBFIRST, out);
	digitalWrite(strobe, HIGH);
	}

void TM1638_Module::reset()
	{
	sendCommand(0x40); // set auto increment mode
	digitalWrite(strobe, LOW);
	shiftOut(data, clock, LSBFIRST, 0xc0);   // set starting address to 0
	for (uint8_t i = 0; i < 16; i++)
		{
		shiftOut(data, clock, LSBFIRST, 0x00);
		}
	digitalWrite(strobe, HIGH);
	}

uint8_t TM1638_Module::readButtons(void)
	{
	uint8_t buttons = 0;
	digitalWrite(strobe, LOW);
	shiftOut(data, clock, LSBFIRST, 0x42);

	pinMode(data, INPUT);

	for (uint8_t i = 0; i < 4; i++)
		{
		uint8_t v = shiftIn(data, clock, LSBFIRST) << i;
		buttons |= v;
		}

	pinMode(data, OUTPUT);
	digitalWrite(strobe, HIGH);
	return buttons;
	}
