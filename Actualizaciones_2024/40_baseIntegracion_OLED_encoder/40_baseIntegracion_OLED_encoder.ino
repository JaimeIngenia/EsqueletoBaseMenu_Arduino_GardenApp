#include "U8glib.h"
#define CLK 2
#define DT 3
#define SW 5

//ENCODER

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;


//OLED

U8GLIB_SSD1306_128X64 u8g( U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST);

// 'Proyecto nuevo', 128x64px
const unsigned char epd_bitmap_Proyecto_nuevo [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x08, 0x8a, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x12, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x11, 0x00, 0x20, 0x3c, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x14, 0x80, 0xa0, 0x22, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x10, 0x70, 0x20, 0x22, 0x38, 0x71, 0xe3, 0xc3, 0x87, 0x13, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x14, 0x48, 0xa0, 0x22, 0x44, 0x89, 0x12, 0x24, 0x48, 0x94, 0x22, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x10, 0x48, 0x20, 0x22, 0x04, 0x81, 0x12, 0x24, 0x40, 0x98, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x10, 0x30, 0x20, 0x22, 0x3c, 0x71, 0x12, 0x24, 0x47, 0x90, 0x22, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x14, 0x00, 0xa0, 0x22, 0x44, 0x09, 0x12, 0x24, 0x48, 0x90, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x10, 0x00, 0x20, 0x22, 0x44, 0x89, 0x12, 0x24, 0x48, 0x90, 0x22, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x12, 0xfd, 0x20, 0x3c, 0x3c, 0x71, 0x13, 0xc3, 0x87, 0x90, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0xfc, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x04, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 
	0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 
	0x5f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x44, 0x40, 0x00, 0x3e, 0x00, 0x00, 0x30, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 
	0x44, 0xa0, 0x00, 0x33, 0x00, 0x00, 0x30, 0x03, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x5f, 0x20, 0x00, 0x33, 0x1e, 0x33, 0x33, 0x03, 0x01, 0xe3, 0xe1, 0xe1, 0xe3, 0x30, 0x00, 0x62, 
	0x40, 0x20, 0x20, 0x33, 0x33, 0x37, 0x36, 0x03, 0x03, 0x33, 0x33, 0x33, 0x33, 0x70, 0x00, 0x60, 
	0x40, 0x10, 0xa0, 0x3e, 0x03, 0x38, 0x3c, 0x01, 0xe3, 0x33, 0x33, 0x03, 0x33, 0x80, 0x00, 0x62, 
	0x41, 0x92, 0xa0, 0x30, 0x1f, 0x30, 0x38, 0x00, 0x33, 0xf3, 0x31, 0xe3, 0x33, 0x00, 0x00, 0x60, 
	0x42, 0x52, 0xa0, 0x30, 0x33, 0x30, 0x3c, 0x00, 0x33, 0x03, 0x30, 0x33, 0x33, 0x00, 0x00, 0x62, 
	0x5a, 0x52, 0xa0, 0x30, 0x33, 0x30, 0x36, 0x03, 0x33, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0x60, 
	0x41, 0x80, 0xa0, 0x30, 0x1f, 0x30, 0x33, 0x01, 0xe1, 0xe3, 0x31, 0xe1, 0xe3, 0x00, 0x00, 0x62, 
	0x40, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc2, 
	0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xff, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x06, 0x00, 0x20, 0x7c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x08, 0x3f, 0x20, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x10, 0x44, 0xc0, 0x10, 0x89, 0x33, 0xc3, 0x80, 0x78, 0xe2, 0x23, 0xc7, 0x00, 0x00, 0x00, 0x02, 
	0x10, 0xaa, 0x00, 0x10, 0x89, 0x42, 0x24, 0x40, 0x89, 0x12, 0x24, 0x48, 0x80, 0x00, 0x00, 0x00, 
	0x10, 0x92, 0x40, 0x10, 0x89, 0x82, 0x24, 0x40, 0x88, 0x12, 0x24, 0x48, 0x80, 0x00, 0x00, 0x02, 
	0x10, 0x92, 0x40, 0x10, 0x89, 0x02, 0x24, 0x40, 0x88, 0xf2, 0x24, 0x4f, 0x80, 0x00, 0x00, 0x00, 
	0x10, 0xaa, 0x40, 0x10, 0x89, 0x02, 0x24, 0x40, 0x89, 0x12, 0x24, 0x48, 0x00, 0x00, 0x00, 0x02, 
	0x08, 0x44, 0x40, 0x10, 0x89, 0x02, 0x24, 0x40, 0x79, 0x12, 0x23, 0xc8, 0x80, 0x00, 0x00, 0x00, 
	0x08, 0x38, 0x40, 0x10, 0x71, 0x03, 0xc3, 0x80, 0x08, 0xf1, 0xc0, 0x47, 0x00, 0x00, 0x00, 0x02, 
	0x04, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x04, 0x40, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x02, 
	0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// **************************************************************************************************************   

// 'icon_3dcube', 16x16px
const unsigned char epd_bitmap_icon_3dcube [] PROGMEM = {
	0x00, 0x00, 0x01, 0x80, 0x07, 0x60, 0x19, 0x18, 0x61, 0x06, 0x51, 0x0a, 0x45, 0xa2, 0x41, 0x02, 
	0x45, 0x22, 0x41, 0x02, 0x45, 0xa2, 0x51, 0x0a, 0x61, 0x06, 0x19, 0x18, 0x07, 0x60, 0x01, 0x80
};
// 'icon_battery', 16x16px
const unsigned char epd_bitmap_icon_battery [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x40, 0x04, 0x5b, 0x66, 0x5b, 0x66, 
	0x5b, 0x66, 0x40, 0x04, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'icon_dashboard', 16x16px
const unsigned char epd_bitmap_icon_dashboard [] PROGMEM = {
	0x1f, 0xf8, 0x20, 0x04, 0x44, 0x52, 0x90, 0x01, 0x88, 0x01, 0xa4, 0x05, 0x83, 0x81, 0xa2, 0x45, 
	0x82, 0x41, 0x81, 0x81, 0xa0, 0x05, 0x80, 0x01, 0x97, 0xe9, 0x47, 0xe2, 0x20, 0x04, 0x1f, 0xf8
};
// 'icon_fireworks', 16x16px
const unsigned char epd_bitmap_icon_fireworks [] PROGMEM = {
	0x00, 0x00, 0x00, 0x08, 0x00, 0x94, 0x10, 0x08, 0x10, 0x00, 0x6c, 0x00, 0x10, 0x10, 0x10, 0x10, 
	0x00, 0x00, 0x00, 0xc6, 0x00, 0x00, 0x00, 0x10, 0x04, 0x10, 0x0a, 0x00, 0x04, 0x00, 0x00, 0x00
};
// 'icon_gps_speed', 16x16px
const unsigned char epd_bitmap_icon_gps_speed [] PROGMEM = {
	0x00, 0x00, 0x03, 0xf0, 0x00, 0x08, 0x01, 0xe4, 0x00, 0x12, 0x00, 0xca, 0x06, 0x2a, 0x07, 0x2a, 
	0x07, 0x8a, 0x07, 0xc2, 0x07, 0xc0, 0x0a, 0x00, 0x1f, 0x00, 0x20, 0x80, 0x7f, 0xc0, 0x00, 0x00
};
// 'icon_turbo', 16x16px
const unsigned char epd_bitmap_icon_turbo [] PROGMEM = {
	0x00, 0x0e, 0x07, 0xf9, 0x08, 0x01, 0x30, 0x01, 0x41, 0xf9, 0x82, 0x26, 0x85, 0x50, 0x84, 0x92, 
	0x84, 0x92, 0x85, 0x52, 0x42, 0x22, 0x41, 0xc2, 0x20, 0x02, 0x10, 0x04, 0x08, 0x18, 0x07, 0xe0
};
// 'icon_parksensor', 16x16px
const unsigned char epd_bitmap_icon_parksensor [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x22, 0x00, 0x25, 0x00, 0xf9, 0x00, 0x01, 0x01, 0x00, 0x85, 
	0x0c, 0x95, 0x12, 0x95, 0xd2, 0x95, 0x0c, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'icon_knob_over_oled', 16x16px
const unsigned char epd_bitmap_icon_knob_over_oled [] PROGMEM = {
	0x00, 0x00, 0x1f, 0xf0, 0x13, 0x50, 0x1b, 0xb0, 0x11, 0x50, 0x1f, 0xf0, 0x03, 0x80, 0x01, 0x00, 
	0x00, 0x00, 0x09, 0x20, 0x49, 0x24, 0x20, 0x08, 0x00, 0x01, 0x80, 0x02, 0x00, 0x00, 0x00, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 384)

const unsigned char* bitmap_icons[8] = {
	epd_bitmap_icon_3dcube,
	epd_bitmap_icon_battery,
	epd_bitmap_icon_dashboard,
	epd_bitmap_icon_fireworks,
	epd_bitmap_icon_gps_speed,
	epd_bitmap_icon_knob_over_oled,
	epd_bitmap_icon_parksensor,
	epd_bitmap_icon_turbo
};

// **************************************************************************************************************

// 'scrollbar_backgrounf', 7x64px
const unsigned char epd_bitmap_scrollbar_backgrounf [] PROGMEM = {
	0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 
	0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 
	0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 
	0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00
};
// 'item_sel_background', 128x21px
const unsigned char epd_bitmap_item_sel_background [] PROGMEM = {
	0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 
	0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
	0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
	0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80
};


// **************************************************************************************************************

const int NUM_ITEMS = 8;

char menu_items[NUM_ITEMS][20]={
  {"3D Cube"},
	{"Battery"},
	{"Dashboard"},
	{"Fireworks"},
	{"GPS Speed"},
	{"Big Knob"},
	{"Park Sensor"},
	{"Turbo Gauge"}
};



// **************************************************************************************************************

int item_selected = 0; // En cual item del menú esta
int item_sel_previous; 
int item_sel_next; 

int current_screen = 0;   // 0 = menu, 1 = screenshot, 2 = qr
int demo_mode = 0; // when demo mode is set to 1, it automatically goes over all the screens, 0 = control menu with buttons
int demo_mode_state = 0; // demo mode state = which screen and menu item to display
int demo_mode_delay = 0; // demo mode delay = used to slow down the screen switching


// **************************************************************************************************************

void setup() {

  //ENCODER

  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  Serial.begin(9600);
  lastStateCLK = digitalRead(CLK);
  
  //OLED

  //u8g.setFont(u8g_font_tpssb);
  u8g.setColorIndex(1);
  u8g2.begin();
  u8g2.setBitmapMode(1);

}

void loop() {

    
  // ------------------------------------------------------------------------------------------------
  //               ENCOER****************************************************************************
  // ------------------------------------------------------------------------------------------------

  checkEncoderAndButton();

  // ------------------------------------------------------------------------------------------------
  //               OLED******************************************************************************
  // ------------------------------------------------------------------------------------------------

  item_sel_previous = item_selected - 1 ;

  if( item_sel_previous < 0 )
  {
    item_sel_previous = NUM_ITEMS - 1 ; // previous item be below fisrt = make it the last
  }

  item_sel_next = item_selected + 1;

  if( item_sel_next >= NUM_ITEMS )
  {
    item_sel_next - 0 ; //next item would be after the last = make it the first
  }


  u8g.firstPage();

  do{

      //u8g.drawBitmapP(4, 2, 128/8, 64, bitmap_icons[2]); 
     
    
      //*********************

      //Previus Item
      u8g.setFont(u8g_font_7x14);
      // u8g.drawStr(26,15,"Dashboard");
      // u8g.drawStr(26,15,menu_items[2]);
      u8g.drawStr(26,15,menu_items[item_sel_previous]);
      //u8g.drawBitmapP(4, 2, 16/8, 16, bitmap_icons[2]); // draw dashboard icon
      u8g.drawBitmapP(4, 2, 16/8, 16, bitmap_icons[item_sel_previous]); // draw dashboard icon


      //Selected Item
      u8g.setFont(u8g_font_7x14);
      // u8g.drawStr(26,37,"Park sensor");
      // u8g.drawStr(26,37,menu_items[6]);
      u8g.drawStr(26,37,menu_items[item_selected]);
      // u8g.drawBitmapP(4, 24, 16/8, 16, bitmap_icons[6]); // draw parking sensor icon
      u8g.drawBitmapP(4, 24, 16/8, 16, bitmap_icons[item_selected]); // draw parking sensor icon

      //Next Item
      u8g.setFont(u8g_font_7x14);
      // u8g.drawStr(26,59,"Turbo Gauge");
      // u8g.drawStr(26,59,menu_items[7]);
      u8g.drawStr(26,59,menu_items[item_sel_next]);
      // u8g.drawBitmapP(4, 46, 16/8, 16, bitmap_icons[7]); // draw turbo gauge icon
      u8g.drawBitmapP(4, 46, 16/8, 16, bitmap_icons[item_sel_next]); // draw turbo gauge icon

      //*********************

      u8g.drawBitmapP(0, 22, 128/8, 21, epd_bitmap_item_sel_background); // selected item background
      u8g.drawBitmapP(120, 0, 8/8, 64, epd_bitmap_scrollbar_backgrounf); // scrolled background

  } while( u8g.nextPage() );





}

// Función para verificar el encoder y el botón
void checkEncoderAndButton() {
    currentStateCLK = digitalRead(CLK);

    // Si el último y el actual estado de CLK son diferentes, entonces ocurrió un pulso
    // Reaccionar a solo 1 cambio de estado para evitar conteo doble.
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {
        // Si el estado de DT es diferente que el estado de CLK entonces
        // el encoder esta rotando en sentido contrario a las agujas del reloj, por lo que se decrementa
        if (digitalRead(DT) != currentStateCLK) {
            counter--;
            currentDir = "CCW";
              item_selected = item_selected - 1 ;

              if( item_selected < 0 )
              {
                item_selected = NUM_ITEMS - 1 ; 
              }

        } else {
            // El encodes esta rotando en sentido a las agujas del reloj, por lo que incrementa.
            counter++;
            currentDir = "CW";

              item_selected = item_selected + 1 ;

              if( item_selected >= NUM_ITEMS )
              {
                item_selected = 0 ; 
              }
        }

        Serial.print("Direction: ");
        Serial.print(currentDir);
        Serial.print("  | Counter: ");
        Serial.println(counter);
    }

    // Recuerda el último estado de CLK
    lastStateCLK = currentStateCLK;

    // Lee el estado del botón
    int btnState = digitalRead(SW);

    // Si detectamos una señal baja (LOW), el botón fue presionado
    if (btnState == LOW) {
        // Si pasaron 50ms desde el último pulso bajo, significa
        // que el botón ha sido presionado, liberado y presionado nuevamente
        if (millis() - lastButtonPress > 50) {
            Serial.println("Button pressed!****************************************************");
        }

        // Recordar el último evento de presión de botón.
        lastButtonPress = millis();
    }

    // Poner un pequeño delay para ayudar a eliminar lecturas erróneas.
    delay(1);
}
