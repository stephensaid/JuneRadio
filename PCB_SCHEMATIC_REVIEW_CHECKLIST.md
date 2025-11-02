# JuneRadio PCB Schematic Review Checklist

## 🔌 POWER SUPPLY & DECOUPLING CAPACITORS (CRITICAL)

### Buck Converter Output (5V Rail)
**Location**: Right at the buck converter output pins

✅ **Already have** (from your schematic):
- [ ] C4: 470µF electrolytic ✅ 
- [ ] 100nF ceramic caps ✅

⚠️ **Verify placement**:
- [ ] 470µF cap is within 5mm of buck converter output pins
- [ ] At least one 100nF cap is within 2mm of buck converter output pins
- [ ] Voltage ratings: 470µF should be ≥10V, 100nF should be ≥10V

### ESP32 DevKit Power Input
**Location**: Where 5V connects to your ESP32 DevKit VIN pin

❌ **Need to add**:
- [ ] 100nF ceramic capacitor at DevKit VIN connection point
- [ ] 10µF ceramic capacitor at DevKit VIN connection point
- [ ] Short, thick traces from buck 5V to DevKit VIN
- [ ] Voltage ratings: Both caps should be ≥10V (for 5V input)

### ESP32 3.3V Rail (if accessible)
**Location**: At ESP32 DevKit 3.3V pins or onboard regulator output

❌ **Need to add** (if pins accessible):
- [ ] 100nF ceramic at each 3.3V VDD pin (or grouped VDD pins)
- [ ] 10µF ceramic near 3.3V regulator output
- [ ] Voltage ratings: ≥6.3V (10V preferred for safety)

**Note**: If using ESP32-DevKit module, it has some onboard caps, but adding external ones improves performance.

### TFT Display Power
**Location**: At TFT connector power pins

❌ **Need to add**:
- [ ] 100nF ceramic at TFT VCC pin
- [ ] 10µF ceramic at TFT VCC pin  
- [ ] Voltage rating depends on TFT: 6.3V for 3.3V, 10V for 5V

### Audio Amplifier Power (PMA8610)
**Location**: At PMA8610 VDD pin

❌ **Need to add**:
- [ ] 100nF ceramic at PMA8610 VDD (12V) pin
- [ ] 100µF electrolytic at PMA8610 VDD pin
- [ ] Voltage ratings: ≥16V (for 12V supply)
- [ ] Check PMA8610 datasheet for additional required caps

## 🎵 AUDIO SECTION VERIFICATION

### VS1053 MP3 Decoder Critical Checks

**Power Connections**:
- [ ] VS1053 VDD connected to 3.3V (NOT 5V!)
- [ ] VS1053 VDD has 100nF ceramic cap
- [ ] VS1053 VDD has 10µF ceramic cap
- [ ] Multiple GND connections from VS1053 to ground plane

**SPI Interface** (MOST CRITICAL - check for shorts):
- [ ] Pin 5 (SCLK) connects to ESP32 GPIO18 only
- [ ] Pin 32 (SI/MOSI) connects to ESP32 GPIO23 only  
- [ ] Pin 31 (SO/MISO) connects to ESP32 GPIO19 only
- [ ] **NO short circuits between SCLK and MOSI traces**
- [ ] **NO copper bridges between any SPI pins**
- [ ] Each SPI signal has its own dedicated trace

**Control Pins**:
- [ ] Pin 6 (XCS) connects to available ESP32 GPIO (suggest GPIO25)
- [ ] Pin 7 (XDCS) connects to available ESP32 GPIO (suggest GPIO26)
- [ ] Pin 4 (DREQ) connects to available ESP32 GPIO (suggest GPIO27)
- [ ] Pin 3 (XRESET) connects to ESP32 reset or available GPIO

**Audio Output**:
- [ ] LOUT/ROUT pins connect to audio filter circuit
- [ ] Filter circuit connects to PMA8610 input

### Audio Filter Circuit Review
**Current values** (may need adjustment):
- [ ] Resistors: Currently 100kΩ → Consider reducing to 1kΩ-10kΩ
- [ ] Capacitors: Check values for proper AC coupling
- [ ] Signal path: VS1053 → Filter → PMA8610 input

## 🔧 ESP32 PIN ASSIGNMENTS

### Verify Against Your Code
**Current pin assignments from your firmware**:

**TFT Display (SPI)**:
- [ ] GPIO19 → TFT MISO ✅
- [ ] GPIO23 → TFT MOSI ✅
- [ ] GPIO18 → TFT SCLK ✅
- [ ] GPIO32 → TFT CS ✅
- [ ] GPIO2 → TFT DC ⚠️ (boot-sensitive pin)
- [ ] GPIO22 → TFT LED ✅

**Rotary Encoders**:
- [ ] GPIO17 → Selector A ✅
- [ ] GPIO4 → Selector B ✅
- [ ] GPIO35 → Selector Button ✅ (input-only)
- [ ] GPIO36 → Volume A ✅ (input-only)
- [ ] GPIO39 → Volume B ✅ (input-only)
- [ ] GPIO34 → Volume Button ✅ (input-only)

**Control Buttons**:
- [ ] GPIO21 → Mode Button ✅
- [ ] GPIO33 → Standby Button ✅
- [ ] GPIO16 → Snooze Button ✅
- [ ] GPIO5 → Menu Button ✅

**Reserved for Debugger**:
- [ ] GPIO12, 13, 14, 15 kept free ✅

**Available for VS1053**:
- [ ] GPIO25 → Suggest for VS1053 XCS
- [ ] GPIO26 → Suggest for VS1053 XDCS  
- [ ] GPIO27 → Suggest for VS1053 DREQ

## ⚠️ BOOT PIN ISSUES

### GPIO2 (TFT_DC) Boot Concern
- [ ] **Problem**: GPIO2 affects ESP32 boot sequence
- [ ] **Current**: Used for TFT_DC in your code
- [ ] **Check**: No pull-down resistors on GPIO2
- [ ] **Alternative**: Consider moving TFT_DC to GPIO27/26/25

### Required Boot Support Components
- [ ] **GPIO0**: 10kΩ pull-up resistor to 3.3V (normal boot)
- [ ] **EN pin**: 10kΩ pull-up resistor to 3.3V
- [ ] **Reset button**: Connected to EN through 100nF capacitor
- [ ] **Reset button**: Has 10kΩ pull-up on EN side

## 🔍 PHYSICAL PCB LAYOUT CHECKS

### Capacitor Placement (Critical for Performance)
- [ ] **100nF ceramics**: Placed within 1-2mm of device power pins
- [ ] **Bulk electrolytics**: Placed within 5mm of power source
- [ ] **Ground connections**: Short traces or vias to ground plane
- [ ] **Power trace width**: Adequate for current (0.5mm minimum for 500mA)

### Ground Plane and Connections
- [ ] **Ground plane**: Continuous under analog sections
- [ ] **Ground vias**: Multiple connections, especially under caps
- [ ] **Split grounds**: Avoid if possible, use single ground plane

### SPI Signal Routing (VS1053 Critical)
- [ ] **Trace lengths**: Keep SPI traces short and similar length
- [ ] **Trace spacing**: Maintain spacing between SCLK/MOSI/MISO
- [ ] **Via usage**: Minimize vias on SPI signals
- [ ] **Crossover points**: Check for unintended connections

## 📦 COMPONENT SPECIFICATIONS

### Capacitor Types and Values
**Ceramic (MLCC) - for high-frequency decoupling**:
- [ ] 100nF (0.1µF) X7R or X5R, 0603 or 0805 package
- [ ] 10µF X5R or X7R, 0805 or 1206 package  
- [ ] Voltage ratings: 6.3V for 3.3V rails, 10V+ for 5V rails

**Electrolytic - for bulk energy storage**:
- [ ] 100µF-470µF low-ESR electrolytic
- [ ] Voltage ratings: 10V+ for 5V, 16V+ for 12V
- [ ] Consider tantalum for better performance (more expensive)

### Resistor Values
**Pull-up resistors**:
- [ ] 10kΩ for digital inputs (buttons, encoders)
- [ ] 10kΩ for boot pins (GPIO0, EN)

**Audio filter resistors**:
- [ ] Current: 100kΩ (may be too high)
- [ ] Suggested: 1kΩ-10kΩ for better signal transfer

## 🎯 PRIORITY ORDER FOR FIXES

### Before Ordering PCBs (Critical):
1. **Add ESP32 power decoupling caps** (100nF + 10µF at VIN)
2. **Verify VS1053 SPI connections** (no MOSI/SCLK shorts)
3. **Add VS1053 power caps** (100nF + 10µF at VDD)
4. **Add PMA8610 power caps** (100nF + 100µF at VDD)
5. **Add TFT power caps** (100nF + 10µF at connector)

### Can Fix in Next Revision:
1. **Audio filter resistor values** (reduce from 100kΩ)
2. **TFT_DC pin move** (from GPIO2 to GPIO27)
3. **PCB layout optimization**

## 📖 USEFUL SEARCH TERMS

**For power supply design**:
- "ESP32 decoupling capacitor placement"
- "buck converter output filtering capacitors"
- "PCB power supply noise reduction"

**For audio section**:
- "VS1053 reference design schematic"
- "PMA8610 typical application circuit"
- "audio coupling capacitor calculation"

**For PCB layout**:
- "high-speed digital PCB layout guidelines"
- "ESP32 PCB design best practices"
- "SPI signal routing PCB"

## ✅ SIGN-OFF CHECKLIST

When you've checked everything:
- [ ] All power supply caps added and placed correctly
- [ ] No short circuits in VS1053 SPI connections  
- [ ] All required pull-up resistors present
- [ ] Component values verified against datasheets
- [ ] PCB layout follows best practices
- [ ] Schematic matches your firmware pin assignments

**Ready to order PCBs when all critical items are checked! ✅**