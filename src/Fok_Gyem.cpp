#include "Arduino.h"
#include "Fok_Gyem.h"
#include "font.h"



Fok_Gyem::Fok_Gyem(const uint8_t digit_num, uint8_t* digit_pins, uint8_t* polarity_pins,
	const uint8_t latch_pin, const uint8_t clock_pin, const uint8_t data_pin, const uint8_t oe_pin)
	
	: latchPin(latch_pin), clockPin(clock_pin), dataPin(data_pin), OE_pin(oe_pin), digit_num(digit_num), 
	digits(digit_pins), polarity_sw(polarity_pins)
	
	{ }


Fok_Gyem::~Fok_Gyem()
	{}

void Fok_Gyem::init() {
	font_ptr = Departures;
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	pinMode(OE_pin, OUTPUT);
	for (byte i = 0; i < digit_num; i++) {
		pinMode(digits[i], OUTPUT);
		}
	for (byte i = 0; i < 2; i++) {
		pinMode(polarity_sw[i], OUTPUT);
		}
	shift_clear_all();

	}

void Fok_Gyem::print_digit(uint8_t pos, uint8_t digit) {
	
	for (uint8_t i = 0; i < pixel_cols_num; i++) {
		uint8_t mask = pgm_read_byte(font_ptr + FONT_DATA_START + digit * 5 + i);
		uint8_t neg_mask = mask ^ polarity_mask[i];
		uint8_t pos_mask = ~neg_mask;
		load_font_data(i, pos_mask);
		write_data(pos, 0);
		delay(30);
		load_font_data(i, neg_mask);
		write_data(pos, 1);
		delay(30);
		}

	}

// fill every digit with bitmask
void Fok_Gyem::fillDisplay(uint8_t mask) {
	for (uint8_t i = 0; i < pixel_cols_num; i++) {

		uint8_t neg_mask = mask ^ polarity_mask[i];
		uint8_t pos_mask = ~neg_mask;
		load_font_data(i, pos_mask);
		for (uint8_t d = 0; d < digit_num; d++) {
			write_data(d, 0);
			delay(10);
			}
		load_font_data(i, neg_mask);
		for (uint8_t d = 0; d < digit_num; d++) {
			write_data(d, 1);
			delay(10);
			}
		}

	}

// clear all shift registers
void Fok_Gyem::shift_clear_all() {
	load_font_data(0, 0);
	}


// load bit data to shift register for single flip dot column (do not output to solenoids)
void Fok_Gyem::load_font_data(uint8_t col, uint8_t mask) {
	if (col >= pixel_cols_num) return;
	uint8_t out = mask;
	digitalWrite(latchPin, LOW); // начинаем передачу данных
	for (uint8_t i = 0; i < pixel_cols_num; i++) {
		if (i == col) { out = mask; }
		else { out = 0; }
		shiftOut(dataPin, clockPin, MSBFIRST, out);
		}
	digitalWrite(latchPin, HIGH);
	}

// same as load_font_data(), but inverse column order and use LSBFIRST
void Fok_Gyem::load_data(uint8_t col, uint8_t mask) {
	if (col >= pixel_cols_num) return;
	uint8_t out = mask;
	digitalWrite(latchPin, LOW); // начинаем передачу данных
	for (uint8_t i = 0; i < pixel_cols_num; i++) {
		if (i == ((pixel_cols_num - col) - 1)) { out = mask; }
		else { out = 0; }
		shiftOut(dataPin, clockPin, LSBFIRST, out);
		}
	digitalWrite(latchPin, HIGH);
	}

// write data from shift register to flip dots solenoids
void Fok_Gyem::write_data(uint8_t pos, bool polarity) {
	if (pos >= digit_num) return;

	digitalWrite(digits[pos], HIGH);
	digitalWrite(polarity_sw[polarity], HIGH);
	delay(30);
	digitalWrite(polarity_sw[polarity], LOW);
	digitalWrite(digits[pos], LOW);
	}
