// Pin definitions
#define SELECTOR_A_PIN      17
#define SELECTOR_B_PIN       4
#define SELECTOR_BTN_PIN    13		// PIN 22 does not seem to be working properly

#define VOLUME_A_PIN        36
#define VOLUME_B_PIN        39
#define VOLUME_BTN_PIN      34

#define TFT_MISO 			19	 // TFT PIN 9
#define TFT_MOSI 			23	 // TFT PIN 6
#define TFT_SCLK 			18	 // TFT PIN 7
#define TFT_CS   			15	 // TFT PIN 3
#define TFT_DC   			 2   // TFT PIN 5
#define TFT_RST  			-1	 // TFT PIN 4, ESP32 PIN EN
#define TFT_LED  			 3	 // TFT PIN 8

#define MODE_PIN   			21
#define STANDBY_PIN  		33
#define SNOOZE_PIN			16
#define MENU_PIN			 5


#define FS_NO_GLOBALS
#define f012m "fonts/Roboto-Med-012"
#define f009r "fonts/Roboto-Reg-009"
#define f012r "fonts/Roboto-Reg-012"
#define f015r "fonts/Roboto-Reg-015"
#define f018r "fonts/Roboto-Reg-018"
#define f024r "fonts/Roboto-Reg-024"
#define f036r "fonts/Roboto-Reg-036"
#define fM048m "fonts/RobotoM-Med-48" 		// numbers only
#define fM090m "fonts/RobotoM-Med-90"		// numbers only

#define DEG2RAD 0.0174532925

#define volume_lower_bound 	 0
#define volume_upper_bound 	10

#define LOCALTZ_POSIX	"CET-1CEST,M3.5.0,M10.5.0/3"		// Time in Malta

/****** Set global variables  ******/
// PWM Constants for LED control of TFT display
const int ledFreq 					= 5000;
const int ledChannel 				= 0;
const int ledResolution 			= 8;
const int tftDelay 					= 10 * 60;   // 10 seconds // TFT delay before turning down brightness
const int tftSleepBrightness 		= 30 * 60;   // TFT brightness after delay
const int weatherScreenTimeout  	=  5 * 60;   // weather screen will return to time screen after this number of minutes
const time_t weatherUpdateFrequency	= 10 * 60;   // update weather every 10 minutes (1 minute = 60)

const String menu_main_name       	= "Main Menu" ;
const String menu_settings_name   	= "Settings"  ;

// DarkSky Weather User configuration
String api_key 		= "01d65d9657ba6415b710acc9c7a0fd54";
String latitude 	=  "35.8987477"; // 90.0000 to -90.0000 negative for Southern hemisphere
String longitude 	= "14.4730606"; // 180.000 to -180.000 negative for West
String units 		= "si";
String language 	= "en";

// OpenWeather User configuration
String OPEN_WEATHER_MAP_APP_ID = "e4c90fd05cb42bca67ce2a2d34020544";
/*
Go to https://openweathermap.org/find?q= and search for a location. Go through the
result set and select the entry closest to the actual location you want to display
data for. It'll be a URL like https://openweathermap.org/city/2657896. The number
at the end is what you assign to the constant below.
 */
String OPEN_WEATHER_MAP_LOCATION_ID = "2563191";
/*
Arabic - ar, Bulgarian - bg, Catalan - ca, Czech - cz, German - de, Greek - el,
English - en, Persian (Farsi) - fa, Finnish - fi, French - fr, Galician - gl,
Croatian - hr, Hungarian - hu, Italian - it, Japanese - ja, Korean - kr,
Latvian - la, Lithuanian - lt, Macedonian - mk, Dutch - nl, Polish - pl,
Portuguese - pt, Romanian - ro, Russian - ru, Swedish - se, Slovak - sk,
Slovenian - sl, Spanish - es, Turkish - tr, Ukrainian - ua, Vietnamese - vi,
Chinese Simplified - zh_cn, Chinese Traditional - zh_tw.
*/
String OPEN_WEATHER_MAP_LANGUAGE = "en";
boolean IS_METRIC = true;
