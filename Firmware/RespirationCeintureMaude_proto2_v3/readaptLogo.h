#ifndef READAPT_LOGO_G
    #define READAPT_LOGO_G

// typedef struct{
// 	const uint8_t* address;
// 	uint16_t width;
// 	uint16_t height;
// }image_t;


const uint8_t irul_logo_map[] PROGMEM = {
    0xff, 0xfa, 0x3f, 0xfe, 0xff, 0xc2, 0x07, 0xfe, 0xff, 0x03, 0xf1, 0xfe, 0xff, 0x84, 0x09, 0xfe, 
	0xf8, 0x44, 0xe0, 0x7e, 0xf0, 0x45, 0x07, 0xfe, 0xe8, 0x25, 0x0c, 0x1e, 0xe7, 0x7e, 0x08, 0x9e, 
	0xc0, 0x40, 0x09, 0x0e, 0xc8, 0x4c, 0x01, 0x0e, 0x87, 0x88, 0xc1, 0x36, 0xb0, 0x98, 0xb1, 0x26, 
	0x80, 0x81, 0xf1, 0xc6, 0xfc, 0x43, 0xe1, 0x02, 0x04, 0x0f, 0xa9, 0x02, 0x76, 0x1d, 0xa9, 0x02, 
	0x0a, 0x3b, 0xa4, 0x82, 0xa8, 0x7f, 0x62, 0x86, 0xa3, 0xff, 0x69, 0xfe, 0xa0, 0xfe, 0xe4, 0x06, 
	0xa0, 0xfe, 0xe3, 0xee, 0x93, 0xfd, 0xe0, 0x0e, 0xc7, 0xfd, 0xe7, 0x9e, 0xef, 0xfd, 0xa0, 0x1e, 
	0xff, 0xfd, 0xa0, 0x3e, 0xff, 0xfd, 0x80, 0x7e, 0xff, 0xfb, 0xc0, 0xfe, 0xff, 0xfb, 0xc3, 0xfe, 
	0xff, 0xfb, 0x8f, 0xfe, 0xff, 0xfe, 0xff, 0xfe 
};

// 'arrow_up', 9x6px
const unsigned char arrowUp [] PROGMEM = {
	0xf7, 0x80, 0xe3, 0x80, 0xc1, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00
};
const uint16_t arrowUp_width = 9;
const uint16_t arrowUp_height = 6;


// 'arrow_down', 9x6px
const unsigned char arrowDown [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc1, 0x80, 0xe3, 0x80, 0xf7, 0x80
};
const uint16_t arrowDown_width = 9;
const uint16_t arrowDown_height = 6;


// 'arrow_left', 6x9px
const unsigned char arrowLeft [] PROGMEM = {
	0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x80, 0xc0, 0xe0, 0xf0
};
const uint16_t arrowLeft_width = 6;
const uint16_t arrowLeft_height = 9;


// 'arrow_right', 6x9px
const unsigned char arrowRight [] PROGMEM = {
	0x3c, 0x1c, 0x0c, 0x04, 0x00, 0x04, 0x0c, 0x1c, 0x3c
};
const uint16_t arrowRight_width = 6;
const uint16_t arrowRight_height = 9;


// 'arrow_right_small', 3x5px
const unsigned char arrowRightSmall [] PROGMEM = {
	0x60, 0x20, 0x00, 0x20, 0x60
};
const uint16_t arrowRightSmall_width = 3;
const uint16_t arrowRightSmall_height = 5;


// 'arrow_left_small', 3x5px
const unsigned char arrowLeftSmall [] PROGMEM = {
	0xc0, 0x80, 0x00, 0x80, 0xc0
};
const uint16_t arrowLeftSmall_width = 3;
const uint16_t arrowLeftSmall_height = 5;


// 'moon', 11x10px
const unsigned char moon [] PROGMEM = {
	0xe0, 0xe0, 0xf8, 0x60, 0xfc, 0x20, 0xfe, 0x00, 0xfe, 0x00, 0xfc, 0x00, 0x38, 0x00, 0x80, 0x20, 
	0xc0, 0x60, 0xe0, 0xe0
};
const uint16_t moon_width = 11;
const uint16_t moon_height = 110;



#endif