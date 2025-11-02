# PCB Schematic Verification Checklist

## 🔌 POWER SUPPLY VERIFICATION

### ESP32 Power (CRITICAL)
Check your ESP32 section for these components:

- [ ] **100nF ceramic capacitor** between VDD and GND (close to ESP32)
- [ ] **10µF ceramic/tantalum capacitor** between VDD and GND  
- [ ] **10kΩ pull-up resistor** on EN pin (to 3.3V)
- [ ] **10kΩ pull-up resistor** on GPIO0 (to 3.3V) - for normal boot
- [ ] **Reset button** with 10kΩ pull-up and 100nF capacitor to ground

### Buck Converter Output
Check your 12V→5V converter section:

- [ ] **100µF-470µF electrolytic capacitor** at +5V output
- [ ] **100nF ceramic capacitor** in parallel with electrolytic
- [ ] **Current rating** adequate for your load (calculate total current)

### TFT Display Power
Check your TFT connector section:

- [ ] **100nF ceramic capacitor** at TFT power pins
- [ ] **10µF capacitor** at TFT power pins
- [ ] **3.3V or 5V supply** (check TFT requirements)

## 🎵 AUDIO SECTION VERIFICATION

### VS1053 MP3 Decoder Connections
Check your VS1053 chip connections against this pinout:

**Power & Ground:**
- [ ] VDD connected to 3.3V (not 5V!)
- [ ] Multiple GND connections
- [ ] **100nF decoupling capacitor** on VDD pin
- [ ] **10µF capacitor** on VDD pin

**SPI Interface to ESP32:**
- [ ] Pin 6 (XCS) → ESP32 GPIO (chip select control)
- [ ] Pin 7 (XDCS) → ESP32 GPIO (chip select data)  
- [ ] Pin 5 (SCLK) → ESP32 GPIO18 (your TFT_SCLK)
- [ ] Pin 32 (SI/MOSI) → ESP32 GPIO23 (your TFT_MOSI)
- [ ] Pin 31 (SO/MISO) → ESP32 GPIO19 (your TFT_MISO)
- [ ] Pin 4 (DREQ) → ESP32 GPIO (data request - needs free GPIO)

**⚠️ CRITICAL CHECK:** 
- [ ] VS1053 SCLK and MOSI are **NOT shorted together**
- [ ] Each SPI signal has its own trace to ESP32
- [ ] No copper bridges between SPI pins

**Audio Output:**
- [ ] LOUT/ROUT pins → audio filter circuit → PMA8610 input

### PMA8610 Audio Amplifier
Check your PMA8610 connections:

**Power:**
- [ ] VDD connected to 12V supply
- [ ] **100nF ceramic capacitor** on VDD pin
- [ ] **100µF electrolytic capacitor** on VDD pin
- [ ] GND connected to system ground

**Audio Input:**
- [ ] IN_L connected to VS1053 LOUT (through filter)
- [ ] IN_R connected to VS1053 ROUT (through filter)
- [ ] Input coupling capacitors if required (check datasheet)

**Audio Output:**
- [ ] OUT_L+/OUT_L- to left speaker
- [ ] OUT_R+/OUT_R- to right speaker
- [ ] Bootstrap capacitors if required (check datasheet)

**Enable/Control:**
- [ ] EN/SD pin connection (if used)

### Audio Filter Circuit
Check the circuit between VS1053 and PMA8610:

- [ ] **Resistor values**: Currently 100kΩ - consider reducing to 1kΩ-10kΩ
- [ ] **Capacitor values**: Check for proper AC coupling
- [ ] **Signal levels**: VS1053 output (~1V) compatible with PMA8610 input

## 🔍 ESP32 PIN ASSIGNMENTS

### Current Code Pin Usage (verify against your schematic):

**TFT Display (SPI):**
- [ ] GPIO19 → TFT MISO
- [ ] GPIO23 → TFT MOSI  
- [ ] GPIO18 → TFT SCLK
- [ ] GPIO32 → TFT CS
- [ ] GPIO2 → TFT DC ⚠️ (boot pin - consider moving)
- [ ] GPIO22 → TFT LED (backlight)

**Rotary Encoders:**
- [ ] GPIO17 → Selector A
- [ ] GPIO4 → Selector B
- [ ] GPIO35 → Selector Button (input-only ✅)
- [ ] GPIO36 → Volume A (input-only ✅)  
- [ ] GPIO39 → Volume B (input-only ✅)
- [ ] GPIO34 → Volume Button (input-only ✅)

**Control Buttons:**
- [ ] GPIO21 → Mode
- [ ] GPIO33 → Standby
- [ ] GPIO16 → Snooze  
- [ ] GPIO5 → Menu

**Reserved:**
- [ ] GPIO12,13,14,15 → Kept free for debugger ✅

**Available for VS1053:**
Free GPIOs you can use: GPIO25, GPIO26, GPIO27

## ⚠️ BOOT PIN CONCERNS

### GPIO2 (TFT_DC) Boot Issue
- [ ] **Problem**: GPIO2 affects ESP32 boot sequence
- [ ] **Solution**: Consider moving TFT_DC to GPIO27, GPIO26, or GPIO25
- [ ] **If keeping GPIO2**: Ensure no pull-down resistors

### Other Boot Pins
- [ ] GPIO0: Must have pull-up (10kΩ to 3.3V)
- [ ] GPIO4: Generally OK for encoder
- [ ] GPIO5: Generally OK for button

## 🎯 IMMEDIATE ACTION ITEMS

### Before Ordering PCBs:
1. **Add missing power supply capacitors** (most critical)
2. **Verify VS1053 SPI connections** (no shorts)
3. **Add VS1053 and PMA8610 decoupling capacitors**
4. **Consider moving TFT_DC** from GPIO2

### Can Fix Later:
1. **Audio filter resistor values** (reduce from 100kΩ)
2. **Audio signal level optimization**
3. **Bootstrap capacitors** (if needed)

## 📖 SEARCH TERMS FOR RESEARCH

**Power Supply Issues:**
- "ESP32 WiFi current spike capacitors"
- "Buck converter output filtering"
- "Decoupling capacitor placement PCB"

**Audio Section:**
- "VS1053 SPI interface schematic"
- "PMA8610 amplifier typical application"
- "Audio coupling capacitors calculation"

**Boot Issues:**  
- "ESP32 boot sequence GPIO strapping pins"
- "ESP32 GPIO2 boot mode selection"