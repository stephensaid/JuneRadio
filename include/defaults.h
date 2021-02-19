#ifndef defaults_h
#define defaults_h

#include <Arduino.h>

#define DevMode 0

// ***************************************************************************
// TFT Settings
// ***************************************************************************
// Color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_DKGRAY      0x31A6      /*  54,  54,  54 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */

// #define USER_SETUP_LOADED
// #define ILI9341_DRIVER

// //Pin definitions
// #define TFT_MISO 			      19	 // TFT PIN 9
// #define TFT_MOSI 			      23	 // TFT PIN 6
// #define TFT_SCLK 			      18	 // TFT PIN 7
// #define TFT_CS   			      15	 // TFT PIN 3
// #define TFT_DC   			       2   // TFT PIN 5
// #define TFT_RST  			      -1	 // TFT PIN 4, ESP32 PIN EN
#define TFT_LED  			       22 //3	 // TFT PIN 8

#define f012m   "fonts/Roboto-Med-012"
#define f009r   "fonts/Roboto-Reg-009"
#define f012r   "fonts/Roboto-Reg-012"
#define f015r   "fonts/Roboto-Reg-015"
#define f018r   "fonts/Roboto-Reg-018"
#define f024r   "fonts/Roboto-Reg-024"
#define f036r   "fonts/Roboto-Reg-036"
#define fM048m  "fonts/RobotoM-Med-48" 	 // numbers only
#define fM090m  "fonts/RobotoM-Med-90"		 // numbers only

// ***************************************************************************
// Encoder and button Pin settings
// ***************************************************************************
#define SELECTOR_A_PIN      17
#define SELECTOR_B_PIN       4
#define SELECTOR_BTN_PIN    13		// PIN 22 does not seem to be working properly

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
#define DEG2RAD 0.0174532925
#define volume_lower_bound 	 0
#define volume_upper_bound 	10

#define LOCALTZ_POSIX	"CET-1CEST,M3.5.0,M10.5.0/3"		// Time in Malta

/****** Set global variables  ******/
// PWM Constants for LED control of TFT display
const int ledFreq 					         = 5000;
const int ledChannel 				         = 0;
const int ledResolution 			       = 8;
const int tftDelay 					         = 10;        // 10 seconds // TFT delay before turning down brightness
const int tftSleepBrightness 		     = 30 * 60;   // TFT brightness after delay
const int weatherScreenTimeout  	   =  5 * 60;   // weather screen will return to time screen after this number of minutes
const time_t weatherUpdateFrequency	 = 10 * 60;   // update weather every 10 minutes (1 minute = 60)

const String menu_main_name       	 = "Main Menu" ;
const String menu_settings_name   	 = "Settings"  ;


// ***************************************************************************
// OpenWeather User configuration
/* Go to https://openweathermap.org/find?q= and search for a location. Go through the
   result set and select the entry closest to the actual location you want to display
   data for. It'll be a URL like https://openweathermap.org/city/2657896. The number
   at the end is what you assign to the constant below.
// ***************************************************************************/
static String OPEN_WEATHER_MAP_APP_ID       = "e4c90fd05cb42bca67ce2a2d34020544";
static String OPEN_WEATHER_MAP_LOCATION_ID  = "2563191";
/*
Arabic - ar, Bulgarian - bg, Catalan - ca, Czech - cz, German - de, Greek - el,
English - en, Persian (Farsi) - fa, Finnish - fi, French - fr, Galician - gl,
Croatian - hr, Hungarian - hu, Italian - it, Japanese - ja, Korean - kr,
Latvian - la, Lithuanian - lt, Macedonian - mk, Dutch - nl, Polish - pl,
Portuguese - pt, Romanian - ro, Russian - ru, Swedish - se, Slovak - sk,
Slovenian - sl, Spanish - es, Turkish - tr, Ukrainian - ua, Vietnamese - vi,
Chinese Simplified - zh_cn, Chinese Traditional - zh_tw.
*/
static String OPEN_WEATHER_MAP_LANGUAGE   = "en";
static boolean IS_METRIC                  = true;

// ***************************************************************************/
// Event handles
// ***************************************************************************/
static uint8_t timeScreenHandle;      // 1
static uint8_t weatherScreenHandle;   // 2
static uint8_t radioScreenHandle;     // 3
static uint8_t topbarHandle;          // 4
static uint8_t weatherUpdateHandle;   // 5
static uint8_t alarm1Handle;          // 6
static uint8_t alarm2Handle;          // 7
static uint8_t alarm3Handle;          // 8
static uint8_t alarm1SnoozeHandle;    // 9
static uint8_t alarm2SnoozeHandle;    // 10
static uint8_t alarm3SnoozeHandle;    // 11
// // 12
// // 13
// // 14
static uint8_t nowEvent;              // 15

#endif // defaults_h
