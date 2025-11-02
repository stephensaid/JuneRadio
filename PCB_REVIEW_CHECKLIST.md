# PCB Review Checklist - JuneRadio Project

## PIN MAPPING VERIFICATION (from code)

### ESP32 DevKit Pin Usage
```
TFT Display (SPI):
├── MISO: GPIO 19 (TFT_MISO)
├── MOSI: GPIO 23 (TFT_MOSI) 
├── SCLK: GPIO 18 (TFT_SCLK)
├── CS:   GPIO 32 (TFT_CS)
├── DC:   GPIO 2  (TFT_DC) ⚠️ BOOT PIN!
├── LED:  GPIO 22 (TFT_LED)
└── RST:  -1 (tied to EN)

Rotary Encoders:
├── Selector: A=GPIO17, B=GPIO4, BTN=GPIO35 ✅
└── Volume:   A=GPIO36, B=GPIO39, BTN=GPIO34 ✅ (input-only pins)

Control Buttons:
├── Mode:    GPIO 21
├── Standby: GPIO 33  
├── Snooze:  GPIO 16
└── Menu:    GPIO 5

Reserved:
└── GPIO 12,13,14,15 (ESP-PROG debugger)
```

## 🔴 CRITICAL FIXES NEEDED

### 1. VS1053 MP3 Decoder Issues
- [ ] Verify VS1053 pinout against datasheet
- [ ] Check MOSI/SCLK connections (no shorts)
- [ ] Ensure proper SPI pin assignments:
  - CS (Chip Select)
  - MOSI (Master Out Slave In) 
  - MISO (Master In Slave Out)
  - SCLK (Serial Clock)
  - DREQ (Data Request) - needs ESP32 GPIO connection

### 2. ESP32 Power Supply 
- [ ] Add 10µF-22µF bulk capacitor near ESP32 VDD
- [ ] Add 100nF ceramic capacitors on power pins
- [ ] Verify ESP32-DevKit has onboard 3.3V regulator

### 3. Boot Pin Issues
- [ ] GPIO 0: Add 10k pull-up to 3.3V (normal boot)
- [ ] GPIO 2 (TFT_DC): ⚠️ Consider moving TFT_DC to different pin
- [ ] Add 10k pull-up on EN pin
- [ ] Reset button with 10k pull-up + 0.1µF cap to ground

### 4. Buck Converter Output
- [ ] Add 100µF-470µF electrolytic at output
- [ ] Add 100nF ceramic in parallel
- [ ] Verify current rating (WiFi needs up to 500mA spikes)

## ⚠️ HIGH PRIORITY FIXES

### 5. Audio Amplifier (PMA8610)
- [ ] Add decoupling caps per datasheet
- [ ] Check if bootstrap caps needed
- [ ] Verify 12V power handling

### 6. Audio Filter Circuit  
- [ ] Review 100k resistor values (too high?)
- [ ] Consider 1k-10k range for better signal transfer
- [ ] Check signal levels compatibility

### 7. TFT Display
- [ ] Add 100nF + 10µF caps at TFT connector
- [ ] Verify 3.3V supply adequacy

## ✅ THINGS THAT LOOK GOOD

### 8. Switchboard
- [x] 10k pull-up resistors on switches ✅
- [x] 100nF debouncing capacitors ✅  
- [x] Rotary encoder circuits ✅
- [x] Input-only pins used correctly ✅

### 9. Inter-board Connections
- [x] JST connectors labeled ✅
- [x] Power/ground distribution ✅
- [x] Signal naming consistency ✅

## 📋 VERIFICATION QUESTIONS

1. **ESP32 Module Type**: Confirm ESP32-DevKit vs bare chip
2. **VS1053 Pinout**: Verify against official datasheet  
3. **PMA8610 Variant**: Check specific datasheet requirements
4. **PCB Layout**: Review trace routing (especially SPI signals)
5. **Power Budget**: Calculate total current draw vs supply capacity

## 🎯 BEFORE ORDERING PCBs

### Critical Path:
1. Fix VS1053 MOSI/SCLK short circuit
2. Add all missing decoupling capacitors  
3. Address boot pin configuration
4. Verify power supply chain

### Recommended Changes:
1. Consider moving TFT_DC from GPIO 2 to avoid boot issues
2. Add all ESP32 support components
3. Review audio circuit resistor values
4. Double-check VS1053 pinout

## 💡 CODE COMPATIBILITY NOTES

The current firmware expects:
- SPI display on specific pins (configurable)
- Input-only pins for volume encoder (correct)
- Reserved debugger pins (good practice)
- Specific button/encoder assignments

Any pin changes require updating:
- `platformio.ini` build flags
- `include/defaults.h` pin definitions