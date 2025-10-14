/***************************************************
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_ThinkInk.h"

#ifdef ARDUINO_ADAFRUIT_FEATHER_RP2040_THINKINK // detects if compiling for
                                                // Feather RP2040 ThinkInk
#define EPD_DC PIN_EPD_DC       // ThinkInk 24-pin connector DC
#define EPD_CS PIN_EPD_CS       // ThinkInk 24-pin connector CS
#define EPD_BUSY PIN_EPD_BUSY   // ThinkInk 24-pin connector Busy
#define SRAM_CS -1              // use onboard RAM
#define EPD_RESET PIN_EPD_RESET // ThinkInk 24-pin connector Reset
#define EPD_SPI &SPI1           // secondary SPI for ThinkInk
#else
#define EPD_DC 10
#define EPD_CS 9
#define EPD_BUSY 7 // can set to -1 to not use a pin (will wait a fixed delay)
#define SRAM_CS 6
#define EPD_RESET 8  // can set to -1 and share with microcontroller Reset!
#define EPD_SPI &SPI // primary SPI
#endif

// 1.54" Monochrome displays with 200x200 pixels and SSD1681 chipset
// ThinkInk_154_Mono_D67 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 1.54" Monochrome displays with 200x200 pixels and SSD1608 chipset
// ThinkInk_154_Mono_D27 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 1.54" Monochrome displays with 152x152 pixels and UC8151D chipset
// ThinkInk_154_Mono_M10 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 2.13" Monochrome displays with 250x122 pixels and SSD1675 chipset
//ThinkInk_213_Mono_B72 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 2.13" Monochrome displays with 250x122 pixels and SSD1675B chipset
// ThinkInk_213_Mono_B73 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 2.13" Monochrome displays with 250x122 pixels and SSD1680 chipset
// ThinkInk_213_Mono_BN display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);
// ThinkInk_213_Mono_B74 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// The GDEY0213B74 is like the B74 above but is not 'shifted down' by 8 pixels
// ThinkInk_213_Mono_GDEY0213B74 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 2.13" Monochrome displays with 212x104 pixels and UC8151D chipset
// ThinkInk_213_Mono_M21 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 2.66" Monochrome display with 296x152 pixels and SSD1680 chipset
// ThinkInk_266_Grayscale4_MFGN display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY,
//                                      EPD_SPI);

// 2.9" 4-level Grayscale (use mono) displays with 296x128 pixels and SSD1680 chip
// ThinkInk_290_Grayscale4_EAAMFGN display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 2.9" 4-level Grayscale (use mono) displays with 296x128 pixels and IL0373 chipset
// ThinkInk_290_Grayscale4_T5 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS,
// EPD_BUSY, EPD_SPI);

// 2.9" Monochrome displays with 296x128 pixels and UC8151D chipset
// ThinkInk_290_Mono_M06 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 3.7" Monochrome Display with 420x240 pixels and UC8253 chipset
// ThinkInk_370_Mono_BAAMFGN display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS,
// EPD_BUSY, EPD_SPI);

// 4.2" Monochrome displays with 400x300 pixels and SSD1619 chipset
// ThinkInk_420_Mono_BN display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 4.2" Monochrome displays with 400x300 pixels and UC8276 chipset
// ThinkInk_420_Mono_M06 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 4.2" Grayscale/Monochrome displays with 400x300 pixels and SSD1683 chipset
// ThinkInk_420_Grayscale4_MFGN display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 5.83" Monochrome displays with 648 x 480 pixels and UC8179 chipset
// ThinkInk_583_Mono_AAAMFGN display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

//#include <Adafruit_SPIDevice.h>

const uint16_t horizStartChBank = 50;
const uint16_t horizChCount = 8;
const uint16_t vertStartLine = 159;
const uint16_t vertLineCount = 64;
const uint16_t vertEndLine = vertStartLine + vertLineCount - 1;

const uint16_t horizStartPixel = 8 * horizStartChBank;
const uint16_t horizPixelCount = 8 * horizChCount;
const uint16_t horizEndPixel = horizStartPixel + horizPixelCount - 1;

#define LOBYTE(x) (x & 0xFF)
#define HIBYTE(x) ((x >> 8) && 0xFF)

uint8_t fc = 10; // Frame Count

uint8_t lut_vcom[] = {
  0b00000000, fc,  0,  0,  0, 1,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
};

uint8_t lut_w2w[] = {
  0b00000000, fc,  0,  0,  0, 1,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
};

uint8_t lut_k2w[] = {
  0b10000000, fc,  0,  0,  0, 1,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
};

uint8_t lut_w2k[] = {
  0b01000000, fc,  0,  0,  0, 1,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
};

uint8_t lut_k2k[] = {
  0b00000000, fc,  0,  0,  0, 1,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
  0b00000000,  0,  0,  0,  0, 0,
};


class ThinkInk_750_Mono_AAAMFGN_Custom : public ThinkInk_750_Mono_AAAMFGN
{
  public:
  
    ThinkInk_750_Mono_AAAMFGN_Custom(int16_t SID, int16_t SCLK, int16_t DC, int16_t RST,
                            int16_t CS, int16_t SRCS, int16_t MISO,
                            int16_t BUSY = -1)
      : ThinkInk_750_Mono_AAAMFGN(SID, SCLK, DC, RST, CS, SRCS, MISO, BUSY){};

  ThinkInk_750_Mono_AAAMFGN_Custom(int16_t DC, int16_t RST, int16_t CS, int16_t SRCS,
                            int16_t BUSY = -1, SPIClass* spi = &SPI)
      : ThinkInk_750_Mono_AAAMFGN(DC, RST, CS, SRCS, BUSY, spi){};
  
   void enablePartialMode()
  {
    EPD_command(0x20, lut_vcom, sizeof(lut_vcom));
    EPD_command(0x21, lut_w2w, sizeof(lut_w2w));
    EPD_command(0x22, lut_k2w, sizeof(lut_k2w));
    EPD_command(0x23, lut_w2k, sizeof(lut_w2k));
    EPD_command(0x24, lut_k2k, sizeof(lut_k2k));

    uint8_t psrData = 0b110111; // LUT from register
    EPD_command(0x00, &psrData, 1);

    //uint8_t pllData = 0b00001011; // 100 Hz -> makes black much dimmer than default of 50 Hz
    //EPD_command(0x30, &pllData, 1);

    uint8_t cmdBuf[] = { 0x90, 0x07 }; // VCOM - disable border?
    EPD_command(0x50, cmdBuf, sizeof(cmdBuf));

    uint8_t buf[] = {
      HIBYTE(horizStartPixel), LOBYTE(horizStartPixel), // HRST
      HIBYTE(horizEndPixel), LOBYTE(horizEndPixel), // HRED
      HIBYTE(vertStartLine), LOBYTE(vertStartLine), // VRST
      HIBYTE(vertEndLine), LOBYTE(vertEndLine), // VRED
      0x00        // PT_SCAN inside only
    };
    EPD_command(0x90, buf, sizeof(buf)); // PTL

    EPD_command(0x91); // PTIN
  }

  void disablePartialMode()
  {
    EPD_command(0x92); // PTOUT
  }

  bool pixelValue = false;

  void displayPartial()
  {
    powerUp();
    enablePartialMode();
    
    size_t bytesToSend = horizPixelCount * vertLineCount / 8;
    Serial.printf("Writing %u bytes of %u to buffer1\n", bytesToSend, pixelValue);
    memset(buffer1, pixelValue ? 0xFF : 0x00, bytesToSend);
    setRAMAddress(0, 0);
    writeRAMFramebufferToEPD(buffer1, bytesToSend, 0);

    pixelValue = !pixelValue;
    delay(2);
    
    Serial.printf("Writing %u bytes of %u to buffer2\n", bytesToSend, pixelValue);
    memset(buffer2, pixelValue ? 0xFF : 0x00, bytesToSend);
    setRAMAddress(0, 0);
    writeRAMFramebufferToEPD(buffer2, bytesToSend, 1);

    Serial.printf("Display refresh\n");
    uint8_t end = 1;
    //EPD_command(0x11); // DSP
    EPD_command(0x12); // DRF

    busy_wait();

    if (_busy_pin <= -1) {
      delay(default_refresh_delay);
    }

    disablePartialMode();
    
    powerDown();
  }
};

// 7.5" Monochrome displays with 800 x 480 pixels and UC8179 chipset
ThinkInk_750_Mono_AAAMFGN_Custom display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

void setup() {
  Serial.begin(115200);
  //while (!Serial) {
    delay(10);
  //}
  Serial.println("Adafruit EPD full update test in mono");
  display.begin(THINKINK_MONO);
}

void loop() {
  Serial.println("Banner demo");
  
  display.clearBuffer();
  display.display();
  delay(2000);

  while (true)
  {
    display.displayPartial();
    //delay(100);
  }
/*
  Serial.println("B/W rectangle demo");
  
  display.fillRect(display.width() / 2, 0, display.width() / 2,
                   display.height(), EPD_BLACK);
  display.display();

  delay(2000);

  Serial.println("Text demo");
  // large block of text
  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext(
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur "
      "adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, "
      "fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor "
      "neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet "
      "ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a "
      "tortor imperdiet posuere. ",
      EPD_BLACK);
  display.display();

  delay(2000);

  display.clearBuffer();
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, EPD_BLACK);
  }

  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i, EPD_BLACK);
  }
  display.display();

  delay(2000);
*/
}

void testdrawtext(const char *text, uint16_t color) {
  display.setCursor(0, 0);
  display.setTextColor(color);
  display.setTextWrap(true);
  display.print(text);
}
