#include "TM1638_Module.h"
#include <DS3231.h>
#include "Fok_Gyem.h"


//#define DEBUG
#ifdef DEBUG

#define SERIALBEGIN(...)   Serial.begin(__VA_ARGS__)
#define DPRINT(...)        Serial.print(__VA_ARGS__)
#define DPRINTLN(...)      Serial.println(__VA_ARGS__)
#define DRINTF(...)        Serial.print(F(__VA_ARGS__))
#define DPRINTLNF(...)     Serial.println(F(__VA_ARGS__)) //printing text using the F macro
#define DELAY(...)         delay(__VA_ARGS__)

#define DEBUG_PRINT(...)   Serial.print(F(#__VA_ARGS__" = ")); Serial.print(__VA_ARGS__); Serial.print(F(" ")) 
#define DEBUG_PRINTLN(...) DEBUG_PRINT(__VA_ARGS__); Serial.println()
#else

#define SERIALBEGIN(...)  
#define DPRINT(...)       
#define DPRINTLN(...)     
#define DPRINTF(...)      
#define DPRINTLNF(...)    
#define DELAY(...)        

#define DEBUG_PRINT(...)    
#define DEBUG_PRINTLN(...)  

#endif

constexpr uint8_t MIN_MODE_VALUE = 0x00;

// Clock control modes for TM1638 board
enum TMode { TEST_MODE = MIN_MODE_VALUE, CLOCK_MODE, SET_TIME, SET_DATE, LAST_MODE };

constexpr uint8_t MAX_MODE_VALUE = LAST_MODE;

TMode& operator ++(TMode& AValue, int) {
	uint8_t tmp = static_cast<uint8_t>(AValue);
	if (++tmp >= MAX_MODE_VALUE) tmp = MIN_MODE_VALUE;
	AValue = static_cast<TMode>(tmp);
	return AValue;
	}


TMode Mode = TMode::CLOCK_MODE;
// пины сдвиговых регистров
#define DATA_PIN 12   // к выводу 14 регистра SD
#define CLOCK_PIN 13 // к выводу 11 регистра (SH_CP)
#define LAT_PIN  10  // к выводу 12 регистра (ST_CP)
#define OE_PIN 11
// число модулей цифр
#define NUM_DIGITS 6
// массив пинов для цифровых модулей
// элементы 0-3 - время, 4-5 - число. 
// От старших разрядов к младшим слева направо
uint8_t digitPins[NUM_DIGITS] = {9, 8, 7, 6, 2, 3};
// Переключатели полярности
uint8_t polarityPins[] = {4, 5};

// Обьект управления блинкерными индикаторами
Fok_Gyem Fok_digits(NUM_DIGITS, digitPins, polarityPins, LAT_PIN, CLOCK_PIN, DATA_PIN, OE_PIN);

// Обьекты платы ТМ1638 и RTC
TM1638_Module tm_module(A0, A1, A2);   // Pins: strobe, clock, data
DS3231  myRTC;

// переменные настройки времени и даты
uint8_t clk_pos = 0;
uint8_t clock_set_vals[2];
bool setup_mode = false;

uint8_t date_pos = 0;
uint8_t date_set_vals[3];
bool date_setup_mode = false;

bool h12Flag, pmFlag, pmCent;

void showTime(uint16_t t, uint8_t dir, uint16_t prev_t = 5555);

// смена режима оторбражения блока ТМ1638
void changeMode(TMode m = TMode::LAST_MODE) {
	if (m != TMode::LAST_MODE) Mode = m;
	else Mode++;
	uint16_t c_time;
	tm_module.reset();
	tm_module.tm1638_num(0, Mode, 0);
	switch (Mode) {
		
		case TMode::CLOCK_MODE:
			c_time = myRTC.getHour(h12Flag, pmFlag) * 100 + myRTC.getMinute();
			showTime(c_time, 0);
			break;
		
		case TMode::SET_TIME:
			setup_mode = false;
			incrClkPos();
			break;

		case TMode::SET_DATE:
			date_setup_mode = false;
			incrDatePos();
			break;
		default:
			break;
		}
	}

// функции настройки даты
void incrDatePos() {
	if (date_setup_mode == false) {
		date_pos = 0;
		date_set_vals[0] = myRTC.getDate();
		date_set_vals[1] = myRTC.getMonth(pmCent);
		date_set_vals[2] = myRTC.getYear();
		date_setup_mode = true;
		printDate(date_set_vals[0], date_set_vals[1], date_set_vals[2]);
		}
	else { date_pos++; }
	if (date_pos > 2) date_pos = 0;
	
	for (uint8_t i = 0; i < 8; i++) {
		if (i == date_pos * 3) {
			tm_module.tm1638_led(i, 1);
			tm_module.tm1638_led(i+1, 1);
			i++;
			}
		else 
		tm_module.tm1638_led(i, 0);
		
		}
	}
void changeDateValue(uint8_t incr_flag) {
	const uint8_t limits[] = { 31,12,99 };

	if (incr_flag == 1) {
		if (++date_set_vals[date_pos] > limits[date_pos]) date_set_vals[date_pos] = 1;
		}
	else {
		if (date_set_vals[date_pos] < 2) date_set_vals[date_pos] = limits[date_pos];
		else date_set_vals[date_pos]--;
		}
	printDate(date_set_vals[0], date_set_vals[1], date_set_vals[2]);	
	}

void printDate(uint8_t c_day, uint8_t c_mon, uint8_t c_year) {
	
	tm_module.tm1638_num(0, c_day / 10, 0);
	tm_module.tm1638_num(1, c_day % 10, 0);
	tm_module.tm1638_num(2, TM1638_Dash, 0);
	tm_module.tm1638_num(3, c_mon / 10, 0);
	tm_module.tm1638_num(4, c_mon % 10, 0);
	tm_module.tm1638_num(5, TM1638_Dash, 0);
	tm_module.tm1638_num(6, c_year / 10, 0);
	tm_module.tm1638_num(7, c_year % 10, 0);
	}

// функции настройки времени
void incrClkPos() {
	if (setup_mode == false) {
		clk_pos = 0;
		clock_set_vals[0] = myRTC.getHour(h12Flag, pmFlag);
		clock_set_vals[1] = myRTC.getMinute();
		uint16_t curr_time = clock_set_vals[0] * 100 + clock_set_vals[1];
		showTime(curr_time, 0);
		
		setup_mode = true;
		}
	else { clk_pos++; }
	if (clk_pos > 2) clk_pos = 0;
	tm_module.tm1638_led(4, (clk_pos == 0) ? 1 : 0);
	for (uint8_t i = 0; i < 3; i++) {
		tm_module.tm1638_led(i+5, (i == clk_pos) ? 1 : 0);
		}
	}
void changePosValue(uint8_t incr_flag) {
	switch (clk_pos) {
		case 0:
			if (incr_flag == 1) {
				if (++clock_set_vals[0] > 23) clock_set_vals[0] = 0;
				}
			else {
				if (clock_set_vals[0] == 0) clock_set_vals[0] = 23;
				else clock_set_vals[0]--;
				}
			break;
		case 1:
			if (incr_flag == 1) {
				clock_set_vals[1] += 10;
				if (clock_set_vals[1] > 59) clock_set_vals[1]-=60;
				}
			else {
				if (clock_set_vals[1] < 10) clock_set_vals[1] += 50;
				else clock_set_vals[1] -= 10;
				}
			break;
		case 2:
			if (incr_flag == 1) {
				if (++clock_set_vals[1] > 59) clock_set_vals[1] = 0;
				}
			else {
				if (clock_set_vals[1] == 0) clock_set_vals[1] = 59;
				else clock_set_vals[1]--;
				}
			break;
		}
	uint16_t curr_time = clock_set_vals[0] * 100 + clock_set_vals[1];
	showTime(curr_time, 0);
	}

// вывод даты на блинкерные индикаторы
void showDate(uint8_t d, uint8_t prev_d) {
	uint16_t pos = 0;
	bool min_out = true;
	if (prev_d == 55) min_out = false;
	while (pos < 2) {
		uint8_t digit = d % 10;
		
			uint8_t p_digit = prev_d % 10;
			if ((min_out == false) || (p_digit != digit)) {
				Fok_digits.print_digit(5 - pos, digit);
				}

			prev_d /= 10;
			
		//tm_module.tm1638_num(7 - pos, digit, 0);
		d /= 10;
		pos++;
		}
	}

// вывод времени на блинкерные индикаторы
void showTime(uint16_t t, uint8_t dir, uint16_t prev_t) {
	uint16_t pos = 0;
	bool min_out = true;
	if (prev_t == 5555) min_out = false;
	while (pos < 4) {
		uint8_t digit = t % 10;
		if (dir == 1) {
			uint8_t p_digit = prev_t % 10;
			if ((min_out == false) || (p_digit != digit)) {
				Fok_digits.print_digit(3 - pos, digit);
				}
			
			prev_t /= 10;
			}
		tm_module.tm1638_num(7 - pos, digit, 0);
		t /= 10;
		pos++;
		}
	}


// установка времени на RTC
void setTime() {
	myRTC.setClockMode(false);  // set to 24h
	myRTC.setHour(clock_set_vals[0]);
	myRTC.setMinute(clock_set_vals[1]);
	setup_mode = false;
	uint16_t curr_time = clock_set_vals[0] * 100 + clock_set_vals[1];
	showTime(curr_time, 1);
	
	}

// установка даты на RTC
void setCalendarDate() {
	myRTC.setClockMode(false);  // set to 24h
	myRTC.setDate(date_set_vals[0]);
	myRTC.setMonth(date_set_vals[1]);
	myRTC.setYear(date_set_vals[2]);
	date_setup_mode = false;
	}

void setup() {
	Wire.begin();
	Fok_digits.init();
	tm_module.init();
	SERIALBEGIN(9600);
	}

void loop() {
	static uint8_t prev_but = 0;
	static uint16_t prev_time = 5555;
	static uint8_t prev_day = 55;
	uint16_t curr_time;
	uint8_t curr_day;
	uint8_t buttons = tm_module.readButtons();
	static uint32_t prev_millis = 0;
	uint32_t interval = 1000;
	static uint8_t out_dev = 1;
	
	
	switch (Mode) {
		
		case TMode::TEST_MODE:
			if ((buttons & 1) && (!(prev_but & 1))) {
				Fok_digits.fillDisplay(255);
				delay(500);
				Fok_digits.fillDisplay(0);
				delay(500);
				prev_time = 5555;
				prev_day = 55;
				changeMode();
				}
			
			break;
		case TMode::CLOCK_MODE:
			// work with RTC
			if ((buttons & 0x10) && (!(prev_but & 0x10))) { changeMode();incrClkPos(); }
			if (millis() - prev_millis > interval) {
				
				curr_time = myRTC.getHour(h12Flag, pmFlag) * 100 + myRTC.getMinute();
				curr_day = myRTC.getDate();
				if (curr_time != prev_time) {
					showTime(curr_time, out_dev, prev_time);interval = 50000;
					prev_time = curr_time;
					}
				else {
					interval = 1000;
					}
				if (curr_day != prev_day) {
				showDate(curr_day,  prev_day);
				prev_day = curr_day;
					}
				prev_millis = millis();
				}
			break;
		case TMode::SET_TIME:
			if ((buttons & 0x10) && (!(prev_but & 0x10))) { incrClkPos(); }
			if ((buttons & 0x20) && (!(prev_but & 0x20))) { changePosValue(0); }
			if ((buttons & 0x40) && (!(prev_but & 0x40))) { changePosValue(1); }
			if ((buttons & 0x80) && (!(prev_but & 0x80))) { setTime(); changeMode(TMode::SET_TIME);}
			break;
		case TMode::SET_DATE:
			//if ((buttons & 0x10) && (!(prev_but & 0x10))) { printDate(); }
			if ((buttons & 0x10) && (!(prev_but & 0x10))) { incrDatePos(); }
			if ((buttons & 0x20) && (!(prev_but & 0x20))) { changeDateValue(0); }
			if ((buttons & 0x40) && (!(prev_but & 0x40))) { changeDateValue(1); }
			if ((buttons & 0x80) && (!(prev_but & 0x80))) { setCalendarDate(); changeMode(TMode::SET_TIME); }
			break;
		case TMode::LAST_MODE:
			changeMode();
			break;
		}
	//
	if ((buttons & 0x08) && (!(prev_but & 0x08))) { changeMode(); }
	
	prev_but = buttons;
}
