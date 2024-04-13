#pragma once
/*
 *
 * Departures pixel 5x7 Font
 *
 * created with FontCreator
 * written by F. Maximilian Thiele
 *
 * http://www.apetech.de/fontCreator
 * me@apetech.de
 *
 * File Name           : font.h
 * Date                : 11.02.2024
 * Font size in bytes  : 402
 * Font width          : 5
 * Font height         : -7
 * Font first char     : 48
 * Font last char      : 59
 * Font used chars     : 11
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */

#define FONT_DATA_START 17

const uint8_t Departures[] PROGMEM = {
    0x01, 0x92, // size
    0x05, // width
    0xF9, // height
    0x30, // first char
    0x0B, // char count

    // char widths
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05,

    // font data
    0x7C, 0x82, 0x82, 0x82, 0x7C, // 48 '0'
    0x00, 0x84, 0xFE, 0x80, 0x00, // 49
    0x84, 0xC2, 0xA2, 0x92, 0x8C, // 50
    0x44, 0x82, 0x92, 0x92, 0x6C, // 51
    0x30, 0x28, 0x24, 0xFE, 0x20, // 52
    0x4E, 0x8A, 0x8A, 0x8A, 0x72, // 53
    0x7C, 0x92, 0x92, 0x92, 0x64, // 54
    0x06, 0x02, 0xF2, 0x0A, 0x06, // 55
    0x6C, 0x92, 0x92, 0x92, 0x6C, // 56
    0x4C, 0x92, 0x92, 0x92, 0x7C, // 57 '9'
    0x00, 0x00, 0x00, 0x00, 0x00 // clear all

    };

