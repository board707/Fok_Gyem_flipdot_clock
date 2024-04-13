#pragma once
/*
 * Fok_Gyem.h
 *
 * Arduino Library for Fok_Gyem flip dot control board
 *
 * 
 * (c) b707 2024
 *
 */
class Fok_Gyem
	{
	public:
		//Instantiate the Module


		Fok_Gyem(const uint8_t digit_num, uint8_t* digit_pins, uint8_t* polarity_pins,
			const uint8_t latch_pin, const uint8_t clock_pin, const uint8_t data_pin, const uint8_t oe_pin);

		virtual ~Fok_Gyem();
		void init();
		void print_digit(uint8_t pos, uint8_t digit);
		void fillDisplay(uint8_t mask);
		


	private:

		void load_font_data(uint8_t col, uint8_t mask);
		void load_data(uint8_t col, uint8_t mask);
		void shift_clear_all();
		void write_data(uint8_t pos, bool polarity);

		// пины сдвиговых регистров
		const uint8_t latchPin;  // к выводу 12 регистра (ST_CP)
		const uint8_t clockPin;  // к выводу 11 регистра (SH_CP)
		const uint8_t dataPin;    // к выводу 14 регистра SD
		const uint8_t OE_pin;
		
		const uint8_t digit_num = 6;
		// массив пинов дл€ цифровых модулей
		// элементы 0-3 - врем€, 4-5 - число. 
		// ќт старших разр€дов к младшим слева направо
		uint8_t* digits;
		// ѕереключатели пол€рности
		uint8_t* polarity_sw;
		
		const uint8_t* font_ptr;
		static const uint8_t pixel_cols_num = 5;
		
		const uint8_t polarity_mask[pixel_cols_num] = {
				   0b10101010,
				   0b01010101,
				   0b10101010,
				   0b01010101,
				   0b10101010,

			};
	
	};

