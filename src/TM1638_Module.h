#pragma once
/*
 * TM1638_Module.h
 *
 * Arduino Library for the TM1638_Module board
 *
 * Writed by examples from the internet
 * b707 2024
 * 
 */
#define TM1638_Space 10
#define TM1638_Dash 11
class TM1638_Module
    {
	public:
		//Instantiate the Module


		TM1638_Module(const uint8_t strobe,
					const uint8_t clock,
					const uint8_t data);

		virtual ~TM1638_Module();
		void init();
		void tm1638_led(byte pos, byte state);
		void tm1638_num(byte pos, byte num, bool dp);
		uint8_t readButtons(void);
		void reset();
		

	private:
		void sendCommand(uint8_t value);
		
		const uint8_t strobe;
		const uint8_t clock;
		const uint8_t data;

    };

