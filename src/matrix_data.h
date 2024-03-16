/**
 * @file matrix_data.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief Модуль для работы с матричными экранами
 * @version 1.0
 * @date 11.03.2024
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <avr/pgmspace.h>

// цифры 6x8
static const uint8_t PROGMEM font_digit[] = {
    0x7E, 0x85, 0x89, 0x91, 0xA1, 0x7E, // 0 0x00
    0x00, 0x00, 0x41, 0xFF, 0x01, 0x00, // 1 0x01
    0x43, 0x85, 0x89, 0x89, 0x89, 0x73, // 2 0x02
    0x42, 0x81, 0x91, 0x91, 0x91, 0x6E, // 3 0x03
    0x0C, 0x14, 0x24, 0x44, 0x84, 0xFF, // 4 0x04
    0xF2, 0xA1, 0xA1, 0xA1, 0xA1, 0x9E, // 5 0x05
    0x7E, 0x91, 0x91, 0x91, 0x91, 0x4E, // 6 0x06
    0xC0, 0x80, 0x87, 0x88, 0x90, 0xE0, // 7 0x07
    0x6E, 0x91, 0x91, 0x91, 0x91, 0x6E, // 8 0x08
    0x72, 0x89, 0x89, 0x89, 0x89, 0x7E, // 9 0x09
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // space 0x0A
    0x22, 0x14, 0x08, 0x14, 0x22, 0x00, // x 0x0B
    0x08, 0x04, 0x02, 0x0C, 0x30, 0x40  // √ 0x0C
};

// шрифт 5х7
static const uint8_t PROGMEM font_5_7[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x3E, 0x55, 0x51, 0x55, 0x3E, //
    0x3E, 0x6B, 0x6F, 0x6B, 0x3E, //
    0x0C, 0x1E, 0x3C, 0x1E, 0x0C, //
    0x08, 0x1C, 0x3E, 0x1C, 0x08, //
    0x1C, 0x4A, 0x7F, 0x4A, 0x1C, //
    0x18, 0x5C, 0x7F, 0x5C, 0x18, //
    0x00, 0x1C, 0x1C, 0x1C, 0x00, //
    0x7F, 0x63, 0x63, 0x63, 0x7F, //
    0x00, 0x1C, 0x14, 0x1C, 0x00, //
    0x7F, 0x63, 0x6B, 0x63, 0x7F, //
    0x30, 0x48, 0x4D, 0x33, 0x07, //
    0x06, 0x29, 0x79, 0x29, 0x06, //
    0x20, 0x50, 0x3F, 0x02, 0x0C, //
    0x60, 0x7F, 0x05, 0x35, 0x3F, //
    0x2A, 0x1C, 0x77, 0x1C, 0x2A, //
    0x00, 0x7F, 0x3E, 0x1C, 0x08, //
    0x08, 0x1C, 0x3E, 0x7F, 0x00, //
    0x14, 0x22, 0x7F, 0x22, 0x14, //
    0x00, 0x5F, 0x00, 0x5F, 0x00, //
    0x06, 0x09, 0x7F, 0x01, 0x7F, //
    0x4A, 0x55, 0x55, 0x55, 0x29, //
    0x60, 0x60, 0x60, 0x60, 0x60, //
    0x54, 0x62, 0x7F, 0x62, 0x54, //
    0x08, 0x04, 0x7E, 0x04, 0x08, //
    0x08, 0x10, 0x3F, 0x10, 0x08, //
    0x08, 0x08, 0x2A, 0x1C, 0x08, //
    0x08, 0x1C, 0x2A, 0x08, 0x08, //
    0x1C, 0x10, 0x10, 0x10, 0x10, //
    0x1C, 0x3E, 0x08, 0x3E, 0x1C, //
    0x30, 0x3C, 0x3F, 0x3C, 0x30, //
    0x06, 0x1E, 0x7E, 0x1E, 0x06, //
    0x00, 0x00, 0x00, 0x00, 0x00, // space 0x20
    0x00, 0x00, 0x5F, 0x00, 0x00, // ! 0x21
    0x00, 0x07, 0x00, 0x07, 0x00, // " 0x22
    0x14, 0x7F, 0x14, 0x7F, 0x14, // # 0x23
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // $ 0x24
    0x23, 0x13, 0x08, 0x64, 0x62, // % 0x25
    0x36, 0x49, 0x56, 0x20, 0x50, // & 0x26
    0x00, 0x00, 0x07, 0x00, 0x00, // ' 0x27
    0x00, 0x1C, 0x22, 0x41, 0x00, // ( 0x28
    0x00, 0x41, 0x22, 0x1C, 0x00, // ) 0x29
    0x14, 0x08, 0x3E, 0x08, 0x14, // * 0x2A
    0x08, 0x08, 0x3E, 0x08, 0x08, // + 0x2B
    0x00, 0xA0, 0x60, 0x00, 0x00, // , 0x2C
    0x08, 0x08, 0x08, 0x08, 0x08, // - 0x2D
    0x00, 0x60, 0x60, 0x00, 0x00, // . 0x2E
    0x20, 0x10, 0x08, 0x04, 0x02, // / 0x2F
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0 0x30
    0x44, 0x42, 0x7F, 0x40, 0x40, // 1 0x31
    0x42, 0x61, 0x51, 0x49, 0x46, // 2 0x32
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3 0x33
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4 0x34
    0x27, 0x45, 0x45, 0x45, 0x39, // 5 0x35
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6 0x36
    0x01, 0x71, 0x09, 0x05, 0x03, // 7 0x37
    0x36, 0x49, 0x49, 0x49, 0x36, // 8 0x38
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9 0x39
    0x00, 0x6C, 0x6C, 0x00, 0x00, // : 0x3A
    0x00, 0xAC, 0x6C, 0x00, 0x00, // ; 0x3B
    0x08, 0x14, 0x22, 0x41, 0x00, // < 0x3C
    0x14, 0x14, 0x14, 0x14, 0x14, // = 0x3D
    0x00, 0x41, 0x22, 0x14, 0x08, // > 0x3E
    0x02, 0x01, 0x51, 0x09, 0x06, // ? 0x3F
    0x3E, 0x41, 0x5D, 0x55, 0x5E, // @ 0x40
    0x7C, 0x12, 0x11, 0x12, 0x7C, // A 0x41
    0x7F, 0x49, 0x49, 0x49, 0x36, // B 0x42
    0x3E, 0x41, 0x41, 0x41, 0x22, // C 0x43
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D 0x44
    0x7F, 0x49, 0x49, 0x49, 0x41, // E 0x45
    0x7F, 0x09, 0x09, 0x09, 0x01, // F 0x46
    0x3E, 0x41, 0x49, 0x49, 0x7A, // G 0x47
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H 0x48
    0x00, 0x41, 0x7F, 0x41, 0x00, // I 0x49
    0x20, 0x40, 0x41, 0x3F, 0x01, // J 0x4A
    0x7F, 0x08, 0x14, 0x22, 0x41, // K 0x4B
    0x7F, 0x40, 0x40, 0x40, 0x60, // L 0x4C
    0x7F, 0x02, 0x0C, 0x02, 0x7F, // M 0x4D
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N 0x4E
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O 0x4F
    0x7F, 0x09, 0x09, 0x09, 0x06, // P 0x50
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q 0x51
    0x7F, 0x09, 0x19, 0x29, 0x46, // R 0x52
    0x46, 0x49, 0x49, 0x49, 0x31, // S 0x53
    0x03, 0x01, 0x7F, 0x01, 0x03, // T 0x54
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U 0x55
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V 0x56
    0x3F, 0x40, 0x3C, 0x40, 0x3F, // W 0x57
    0x63, 0x14, 0x08, 0x14, 0x63, // X 0x58
    0x07, 0x08, 0x70, 0x08, 0x07, // Y 0x59
    0x61, 0x51, 0x49, 0x45, 0x43, // Z 0x5A
    0x00, 0x7F, 0x41, 0x41, 0x00, // [ 0x5B
    0x02, 0x04, 0x08, 0x10, 0x20, /* \ 0x5C */
    0x00, 0x41, 0x41, 0x7F, 0x00, // ] 0x5D
    0x04, 0x02, 0x01, 0x02, 0x04, // ^ 0x5E
    0x40, 0x40, 0x40, 0x40, 0x40, // _ 0x5F
    0x00, 0x01, 0x02, 0x04, 0x00, // ` 0x60
    0x20, 0x54, 0x54, 0x54, 0x78, // a 0x61
    0x7F, 0x48, 0x44, 0x44, 0x38, // b 0x62
    0x38, 0x44, 0x44, 0x44, 0x48, // c 0x63
    0x38, 0x44, 0x44, 0x48, 0x7F, // d 0x64
    0x38, 0x54, 0x54, 0x54, 0x18, // e 0x65
    0x08, 0x7E, 0x09, 0x01, 0x02, // f 0x66
    0x08, 0x54, 0x54, 0x58, 0x3C, // g 0x67
    0x7F, 0x08, 0x04, 0x04, 0x78, // h 0x68
    0x00, 0x44, 0x7D, 0x40, 0x00, // i 0x69
    0x20, 0x40, 0x44, 0x3D, 0x00, // j 0x6A
    0x7F, 0x10, 0x10, 0x28, 0x44, // k 0x6B
    0x00, 0x41, 0x7F, 0x40, 0x00, // l 0x6C
    0x7C, 0x04, 0x78, 0x04, 0x78, // m 0x6D
    0x7C, 0x08, 0x04, 0x04, 0x78, // n 0x6E
    0x38, 0x44, 0x44, 0x44, 0x38, // o 0x6F
    0x7C, 0x14, 0x14, 0x14, 0x08, // p 0x70
    0x08, 0x14, 0x14, 0x0C, 0x7C, // q 0x71
    0x7C, 0x08, 0x04, 0x04, 0x08, // r 0x72
    0x48, 0x54, 0x54, 0x54, 0x24, // s 0x73
    0x04, 0x3F, 0x44, 0x40, 0x20, // t 0x74
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u 0x75
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v 0x76
    0x3C, 0x40, 0x38, 0x40, 0x3C, // w 0x77
    0x44, 0x28, 0x10, 0x28, 0x44, // x 0x78
    0x0C, 0x50, 0x50, 0x50, 0x3C, // y 0x79
    0x44, 0x64, 0x54, 0x4C, 0x44, // z 0x7A
    0x00, 0x08, 0x36, 0x41, 0x00, // { 0x7B
    0x00, 0x00, 0x7F, 0x00, 0x00, // | 0x7C
    0x00, 0x41, 0x36, 0x08, 0x00, // } 0x7D
    0x02, 0x01, 0x02, 0x04, 0x02, // ~ 0x7E
    0x70, 0x48, 0x44, 0x48, 0x70, //
    0x00, 0x0E, 0x11, 0x0E, 0x00, //
    0x00, 0x12, 0x1F, 0x10, 0x00, //
    0x00, 0x12, 0x19, 0x16, 0x00, //
    0x00, 0x11, 0x15, 0x0B, 0x00, //
    0x00, 0x07, 0x04, 0x1F, 0x00, //
    0x00, 0x17, 0x15, 0x09, 0x00, //
    0x00, 0x0E, 0x15, 0x09, 0x00, //
    0x00, 0x01, 0x1D, 0x03, 0x00, //
    0x00, 0x0A, 0x15, 0x0A, 0x00, //
    0x00, 0x12, 0x15, 0x0E, 0x00, //
    0x00, 0x04, 0x04, 0x04, 0x00, //
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //
    0x3E, 0x00, 0x00, 0x00, 0x00, //
    0x3E, 0x3E, 0x00, 0x00, 0x00, //
    0x3E, 0x3E, 0x00, 0x3E, 0x00, //
    0x3E, 0x3E, 0x00, 0x3E, 0x3E, //
    0x80, 0x80, 0x80, 0x80, 0x80, //
    0xC0, 0xC0, 0xC0, 0xC0, 0xC0, //
    0xD0, 0xD0, 0xD0, 0xD0, 0xD0, //
    0xD8, 0xD8, 0xD8, 0xD8, 0xD8, //
    0xDA, 0xDA, 0xDA, 0xDA, 0xDA, //
    0xDB, 0xDB, 0xDB, 0xDB, 0xDB, //
    0x40, 0x00, 0x40, 0x00, 0x40, // … 0x96
    0x60, 0x00, 0x40, 0x00, 0x40, //
    0x60, 0x00, 0x70, 0x00, 0x40, //
    0x60, 0x00, 0x70, 0x00, 0x78, //
    0x7C, 0x00, 0x40, 0x00, 0x40, //
    0x7C, 0x00, 0x7E, 0x00, 0x40, //
    0x7C, 0x00, 0x7E, 0x00, 0x7F, //
    0x1C, 0x77, 0x41, 0x41, 0x41, //
    0x41, 0x41, 0x41, 0x41, 0x41, //
    0x41, 0x41, 0x41, 0x7F, 0x00, //
    0x1C, 0x77, 0x41, 0x5D, 0x5D, //
    0x41, 0x41, 0x41, 0x5D, 0x5D, //
    0x5D, 0x5D, 0x41, 0x5D, 0x5D, //
    0x5D, 0x5D, 0x41, 0x7F, 0x00, //
    0x22, 0x1C, 0x14, 0x1C, 0x22, // ¤ 0xA4
    0x00, 0x08, 0x1C, 0x08, 0x00, //
    0x00, 0x00, 0x77, 0x00, 0x00, // ¦ 0xA6
    0x46, 0x5D, 0x55, 0x5D, 0x31, // § 0xA7
    0x7C, 0x55, 0x54, 0x55, 0x44, // Ё 0xA8
    0x08, 0x08, 0x2A, 0x08, 0x08, //
    0x00, 0x14, 0x08, 0x14, 0x00, //
    0x08, 0x14, 0x22, 0x08, 0x14, // « 0xAB
    0x7F, 0x41, 0x71, 0x31, 0x1F, //
    0x03, 0x05, 0x7F, 0x05, 0x03, //
    0x22, 0x14, 0x7F, 0x55, 0x22, //
    0x02, 0x55, 0x7D, 0x05, 0x02, //
    0x06, 0x09, 0x09, 0x06, 0x00, // ° 0xB0
    0x44, 0x44, 0x5F, 0x44, 0x44, // ± 0xB1
    0x1C, 0x14, 0x1C, 0x22, 0x7F, //
    0x20, 0x3E, 0x61, 0x3E, 0x20, //
    0x20, 0x50, 0x3F, 0x02, 0x0C, //
    0x80, 0x7C, 0x20, 0x3C, 0x40, // µ 0xB5
    0x44, 0x3C, 0x04, 0x7C, 0x44, // π 0xB6
    0x00, 0x00, 0x08, 0x00, 0x00, // · 0xB7
    0x38, 0x55, 0x54, 0x55, 0x18, // ё 0xB8
    0x7E, 0x08, 0x10, 0x7F, 0x01, // № 0xB9
    0x08, 0x10, 0x08, 0x04, 0x02, //
    0x14, 0x08, 0x22, 0x14, 0x08, // » 0xBB
    0x0E, 0x06, 0x0A, 0x10, 0x20, //
    0x20, 0x10, 0x0A, 0x06, 0x0E, //
    0x38, 0x30, 0x28, 0x04, 0x02, //
    0x02, 0x04, 0x28, 0x30, 0x38, //
    0x7E, 0x11, 0x11, 0x11, 0x7E, // А 0xC0
    0x7F, 0x49, 0x49, 0x49, 0x31, // Б 0xC1
    0x7F, 0x49, 0x49, 0x49, 0x36, // В 0xC2
    0x7F, 0x01, 0x01, 0x01, 0x03, // Г 0xC3
    0xC0, 0x7F, 0x41, 0x7F, 0xC0, // Д 0xC4
    0x7F, 0x49, 0x49, 0x49, 0x41, // Е 0xC5
    0x77, 0x08, 0x7F, 0x08, 0x77, // Ж 0xC6
    0x41, 0x49, 0x49, 0x49, 0x36, // З 0xC7
    0x7F, 0x10, 0x08, 0x04, 0x7F, // И 0xC8
    0x7C, 0x21, 0x12, 0x09, 0x7C, // Й 0xC9
    0x7F, 0x08, 0x14, 0x22, 0x41, // К 0xCA
    0x40, 0x3E, 0x01, 0x01, 0x7F, // Л 0xCB
    0x7F, 0x02, 0x0C, 0x02, 0x7F, // М 0xCC
    0x7F, 0x08, 0x08, 0x08, 0x7F, // Н 0xCD
    0x3E, 0x41, 0x41, 0x41, 0x3E, // О 0xCE
    0x7F, 0x01, 0x01, 0x01, 0x7F, // П 0xCF
    0x7F, 0x09, 0x09, 0x09, 0x06, // Р 0xD0
    0x3E, 0x41, 0x41, 0x41, 0x22, // С 0xD1
    0x01, 0x01, 0x7F, 0x01, 0x01, // Т 0xD2
    0x07, 0x48, 0x48, 0x48, 0x3F, // У 0xD3
    0x0E, 0x11, 0x7F, 0x11, 0x0E, // Ф 0xD4
    0x63, 0x14, 0x08, 0x14, 0x63, // Х 0xD5
    0x7F, 0x40, 0x40, 0x7F, 0xC0, // Ц 0xD6
    0x07, 0x08, 0x08, 0x08, 0x7F, // Ч 0xD7
    0x7F, 0x40, 0x7F, 0x40, 0x7F, // Ш 0xD8
    0x7F, 0x40, 0x7F, 0x40, 0xFF, // Щ 0xD9
    0x01, 0x7F, 0x48, 0x48, 0x30, // Ъ 0xDA
    0x7F, 0x48, 0x48, 0x30, 0x7F, // Ы 0xDB
    0x7F, 0x48, 0x48, 0x48, 0x30, // Ь 0xDC
    0x22, 0x41, 0x49, 0x49, 0x3E, // Э 0xDD
    0x7F, 0x08, 0x3E, 0x41, 0x3E, // Ю 0xDE
    0x46, 0x29, 0x19, 0x09, 0x7F, // Я 0xDF
    0x20, 0x54, 0x54, 0x54, 0x78, // а 0xE0
    0x3C, 0x4A, 0x4A, 0x49, 0x31, // б 0xE1
    0x7C, 0x54, 0x54, 0x54, 0x28, // в 0xE2
    0x7C, 0x04, 0x04, 0x04, 0x0C, // г 0xE3
    0xC0, 0x78, 0x44, 0x7C, 0xC0, // д 0xE4
    0x38, 0x54, 0x54, 0x54, 0x18, // е 0xE5
    0x6C, 0x10, 0x7C, 0x10, 0x6C, // ж 0xE6
    0x44, 0x54, 0x54, 0x54, 0x28, // з 0xE7
    0x7C, 0x20, 0x10, 0x08, 0x7C, // и 0xE8
    0x7C, 0x40, 0x26, 0x10, 0x7C, // й 0xE9
    0x7C, 0x10, 0x10, 0x28, 0x44, // к 0xEA
    0x40, 0x38, 0x04, 0x04, 0x7C, // л 0xEB
    0x7C, 0x08, 0x10, 0x08, 0x7C, // м 0xEC
    0x7C, 0x10, 0x10, 0x10, 0x7C, // н 0xED
    0x38, 0x44, 0x44, 0x44, 0x38, // о 0xEE
    0x7C, 0x04, 0x04, 0x04, 0x7C, // п 0xEF
    0x7C, 0x14, 0x14, 0x14, 0x08, // р 0xF0
    0x38, 0x44, 0x44, 0x44, 0x48, // с 0xF1
    0x04, 0x04, 0x7C, 0x04, 0x04, // т 0xF2
    0x0C, 0x50, 0x50, 0x50, 0x3C, // у 0xF3
    0x18, 0x24, 0xFC, 0x24, 0x18, // ф 0xF4
    0x44, 0x28, 0x10, 0x28, 0x44, // х 0xF5
    0x7C, 0x40, 0x40, 0x7C, 0xC0, // ц 0xF6
    0x0C, 0x10, 0x10, 0x10, 0x7C, // ч 0xF7
    0x7C, 0x40, 0x7C, 0x40, 0x7C, // ш 0xF8
    0x7C, 0x40, 0x7C, 0x40, 0xFC, // щ 0xF9
    0x04, 0x7C, 0x50, 0x50, 0x20, // ъ 0xFA
    0x7C, 0x50, 0x50, 0x20, 0x7C, // ы 0xFB
    0x7C, 0x50, 0x50, 0x50, 0x20, // ь 0xFC
    0x28, 0x44, 0x54, 0x54, 0x38, // э 0xFD
    0x7C, 0x10, 0x38, 0x44, 0x38, // ю 0xFE
    0x48, 0x34, 0x14, 0x14, 0x7C  // я 0xFF
};

#define DISP_DATE_DISPLAY_INTERVAL_TAG 0
#define DISP_ANIMATION_TAG 1
#define DISP_ALARM_TAG 2
#define DISP_BRIGHTNESS_TAG 3
#define DISP_LIGHT_THRESHOLD_TAG 4
#define DISP_COLOR_TAG 5

#ifdef USE_RU_LANGUAGE
static const uint8_t PROGMEM day_of_week[] = {
    0xC2, 0xD1, 0xCA, // "ВСК";
    0xCF, 0xCD, 0xC4, // "ПНД";
    0xC2, 0xD2, 0xD0, // "ВТР";
    0xD1, 0xD0, 0xC4, // "СРД";
    0xD7, 0xD2, 0xC2, // "ЧТВ";
    0xCF, 0xD2, 0xCD, // "ПТН";
    0xD1, 0xC1, 0xD2  // "СБТ";
};

static const uint8_t PROGMEM tags[] = {
    0xC0, 0xE2, 0xE4, // Авд - автовывод даты
    0xC0, 0xED, 0xEC, // Анм - анимация
    0xC1, 0xE4, 0xEA, // Бдк - будильник
    0xDF, 0xF0, 0xEA, // Ярк - яркость
    0xCF, 0xF0, 0xE3, // Прг - порог переключения яркости
    0xD6, 0xE2, 0xF2  // Цвт - цвет символов
};

static const uint8_t PROGMEM months[] = {
    0xFF, 0xED, 0xE2, // янв
    0xF4, 0xE5, 0xE2, // фев
    0xEC, 0xE0, 0xF0, // мар
    0xE0, 0xEF, 0xF0, // апр
    0xEC, 0xE0, 0xFF, // мая
    0xE8, 0xFE, 0xED, // июн
    0xE8, 0xFE, 0xEB, // июл
    0xE0, 0xE2, 0xE3, // авг
    0xF1, 0xE5, 0xED, // сен
    0xEE, 0xEA, 0xF2, // окт
    0xED, 0xEE, 0xFF, // ноя
    0xE4, 0xE5, 0xEA  // дек
};

#else
static const uint8_t PROGMEM day_of_week[] = {
    0x53, 0x55, 0x4E, // "SUN";
    0x4D, 0x4F, 0x4E, // "MON";
    0x54, 0x55, 0x45, // "TUE";
    0x57, 0x45, 0x44, // "WED";
    0x54, 0x48, 0x55, // "THU";
    0x46, 0x52, 0x49, // "FRI";
    0x53, 0x41, 0x54  // "SAT";
};

static const uint8_t PROGMEM tags[] = {
    0x44, 0x64, 0x69, // Ddi - Date display interval
    0x41, 0x6E, 0x69, // Ani - Animation
    0x41, 0x6C, 0x6D, // Alm - Alarm
    0x42, 0x72, 0x73, // Brs - Brightness
    0x54, 0x68, 0x72, // Thr - Threshold
    0x43, 0x6C, 0x72  // Clr - Color
};

static const uint8_t PROGMEM months[] = {
    0x4A, 0x61, 0x6E, // Jan
    0x46, 0x65, 0x62, // Feb
    0x4D, 0x61, 0x72, // Mar
    0x41, 0x70, 0x72, // Apr
    0x4D, 0x61, 0x79, // May
    0x4A, 0x75, 0x6E, // Jun
    0x4A, 0x75, 0x6C, // Jul
    0x41, 0x75, 0x67, // Aug
    0x53, 0x65, 0x70, // Sep
    0x4F, 0x63, 0x74, // Oct
    0x4E, 0x6F, 0x76, // Nov
    0x44, 0x65, 0x63  // Dec
};
#endif

/**
 * @brief Определение дня недели по дате в интервале 2000-2099 года, 0 - воскресенье
 *
 * @param _day дата
 * @param _month месяц
 * @param _year год
 * @return номер дня недели в интервале 0 (воскресенье) .. 6 (суббота)
 */
uint8_t
getDayOfWeek(uint8_t _day, uint8_t _month, uint16_t _year)
{
  static const uint8_t PROGMEM m[12] = {6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

  uint8_t y1 = _year % 2000;
  // вычислить индекс года
  uint8_t t1 = y1 / 12 + y1 % 12 + y1 % 12 / 4;
  // получить индекс месяца
  uint8_t t2 = pgm_read_byte(&m[_month - 1]);
  // uint8_t t2 = m[_month - 1];
  // сделать поправку на високосный год, если нужно
  if (_month < 3 && y1 % 4 == 0)
  {
    t2--;
  }

  return ((t1 + t2 + _day) % 7);
}

/**
 * @brief изменение порядка следования битов в байте
 *
 * @param b байт для реверса
 * @return результат
 */
uint8_t reverseByte(uint8_t b)
{
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return (b);
}

/**
 * @brief класс для формирования данных бегущей строки
 *
 */
class StringData
{
private:
  uint8_t *data;
  uint8_t data_count = 0;

public:
  StringData() {}

  /**
   * @brief инициализация строки
   *
   * @param _data_count размер строки в байтах (столбцах, с учетом ширины
   * символа и величины межсимвольного интервала)
   * @return возвращает true, если память для строки успешно выделена; иначе возвращает false
   */
  bool stringInit(uint8_t _data_count)
  {
    bool result = false;
    stringFree();

    {
      data = (uint8_t *)calloc(_data_count, sizeof(uint8_t));
      if (data != NULL)
      {
        result = true;
        data_count = _data_count;
      }
    }

    return (result);
  }

  /**
   * @brief освобождение памяти, занимаемой строкой
   *
   * @return возвращает true, если память успешно освобождена
   */
  bool stringFree()
  {
    if (data != NULL)
    {
      free(data);
      data_count = 0;
    }

    return (data == NULL);
  }

  /**
   * @brief получение битовой маски столбца
   *
   * @param index индекс столбца
   * @return результат
   */
  uint8_t getData(uint8_t index)
  {
    uint8_t result = 0;
    if (data != NULL)
    {
      result = (index < data_count) ? data[index] : 0;
    }
    return (result);
  }

  /**
   * @brief установка битовой маски столбца
   *
   * @param index индекс столбца
   * @param _data битовая маска столбца
   */
  void setData(uint8_t index, uint8_t _data)

  {
    if ((data != NULL) && (index < data_count))
    {
      data[index] = _data;
    }
  }

  /**
   * @brief получение размера строки в столбцах
   *
   * @return результат
   */
  uint8_t getDataLenght()
  {
    return (data_count);
  }
};

StringData sData; // данные бегущей строки
