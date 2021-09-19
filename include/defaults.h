
#ifndef defaults_h
#define defaults_h

#include <Arduino.h>

#define GRID_ON 0   // -1 for true or 0 for false

#define DEBUG_DEBUG -1
#define DEBUG_INFO 0
//TODO: improve INFO, DEBUG, ERROR reporting

#define INTERRUPT_ATTR IRAM_ATTR

// ***************************************************************************
// TFT Settings
// ***************************************************************************
// Color definitions using RGB565 (not RGB888)
// Colour Picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
// Hexadecimal to Decimal converter: https://www.rapidtables.com/convert/number/hex-to-decimal.html
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_DKGRAY      0x18e3      /*  25,  28,  25   #191C19 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */

#define F10L   "fonts/NotoSans-Light-10"
#define F12L   "fonts/NotoSans-Light-12"
#define F15L   "fonts/NotoSans-Light-15"
#define F18L   "fonts/NotoSans-Light-18"
#define F24L   "fonts/NotoSans-Light-24"
#define F36L   "fonts/NotoSans-Light-36"
#define F14B   "fonts/NotoSans-Bold-14"
#define F90L   "fonts/NotoSans-Light-90"		 // numbers only

// ***************************************************************************
// Encoder and button Pin settings
// ***************************************************************************
// Save Pins 12, 13, 14, 15 for Debugger
#define SELECTOR_A_PIN      17
#define SELECTOR_B_PIN       4
#define SELECTOR_BTN_PIN    35		// PIN 22 does not seem to be working properly

#define VOLUME_A_PIN        36
#define VOLUME_B_PIN        39
#define VOLUME_BTN_PIN      34

#define MODE_PIN   			    21
#define STANDBY_PIN  		    33
#define SNOOZE_PIN			    16
#define MENU_PIN			       5

// ***************************************************************************
// Other settings
// ***************************************************************************

#define FS_NO_GLOBALS
#define DEG2RAD               0.0174532925
#define VOLUME_LOWER_BOUND 	  0
#define VOLUME_UPPER_BOUND 	  10

// PWM Constants for LED control of TFT display
#define LED_FREQ              5000
#define LED_CHANNEL 				  0
#define LED_RESOLUTION 			  8

#define LOCALTZ_POSIX	"CET-1CEST,M3.5.0,M10.5.0/3"		// Time in Malta

/****** Set global variables  ******/
const static DRAM_ATTR int tftDelay 					          = 10;                  // 10 seconds // TFT delay before turning down brightness
const static DRAM_ATTR int tftSleepBrightness 		      = 0;                   // TFT brightness after delay - Value of brightness for the TFT LED (not time)
const static DRAM_ATTR int weatherScreenTimeout  	      = 1 * 60;              // weather screen will return to time screen after this number of minutes
const static DRAM_ATTR time_t weatherUpdateFrequency	  = 10 * 60;             // update weather every 10 minutes (1 minute = 60)
const static DRAM_ATTR time_t alarmSnooze               = 10 * 60;              // time to snooze alarms
const static DRAM_ATTR String menu_main_name       	    = "Main Menu";
const static DRAM_ATTR String menu_settings_name   	    = "Settings";


// ***************************************************************************
// OpenWeather User configuration
/* Go to https://openweathermap.org/find?q= and search for a location. Go through the
   result set and select the entry closest to the actual location you want to display
   data for. It'll be a URL like https://openweathermap.org/city/2657896. The number
   at the end is what you assign to the constant below.
// ***************************************************************************/
const static DRAM_ATTR String OPEN_WEATHER_MAP_APP_ID = "e4c90fd05cb42bca67ce2a2d34020544";
/*
Arabic - ar, Bulgarian - bg, Catalan - ca, Czech - cz, German - de, Greek - el,
English - en, Persian (Farsi) - fa, Finnish - fi, French - fr, Galician - gl,
Croatian - hr, Hungarian - hu, Italian - it, Japanese - ja, Korean - kr,
Latvian - la, Lithuanian - lt, Macedonian - mk, Dutch - nl, Polish - pl,
Portuguese - pt, Romanian - ro, Russian - ru, Swedish - se, Slovak - sk,
Slovenian - sl, Spanish - es, Turkish - tr, Ukrainian - ua, Vietnamese - vi,
Chinese Simplified - zh_cn, Chinese Traditional - zh_tw.
*/
const static DRAM_ATTR String OPEN_WEATHER_MAP_LANGUAGE = "en";
const static DRAM_ATTR bool IS_METRIC = true;

// Set both your longitude and latitude to at least 4 decimal places
const static DRAM_ATTR String OPEN_WEATHER_MAP_LATITUDE  = "35.9423"; // 90.0000 to -90.0000 negative for Southern hemisphere
const static DRAM_ATTR String OPEN_WEATHER_MAP_LONGITUDE = "14.2379"; // 180.000 to -180.000 negative for West


#endif // defaults_h
