/*
  ReadAnalogVoltage

  Reads an analog input off channel0, converts it to voltage, and prints the
  result to the Serial Monitor. Attach the center pin of a potentiometer to pin
  A0, and the outside pins to +3V3 and ground.

*/
#include "Arduino.h"
#include "MCP3x6x.h"

#if defined ARDUINO_AVR_PROMICRO8
MCP3561 mcp(2, 3, 10);
#elif defined ARDUINO_GRAND_CENTRAL_M4
SPIClass mySPI = SPIClass(&sercom5, 125, 126, 99, SPI_PAD_0_SCK_3, SERCOM_RX_PAD_2);
MCP3561 mcp(84, 81, 98, &mySPI);
#elif defined ADAFRUIT_METRO_M0_EXPRESS
SPIClass mySPI(&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);
MCP3561 mcp(8, 7, 10, &mySPI, 11, 12, 13);
#elif defined ARDUINO_ARCH_ESP8266
MCP3561 mcp(D1, D2, SS);
// #elif
// todo: might need further cases, didn't check for all boards
#else
MCP3561 mcp;
#endif

void mcp_wrapper() { mcp.IRQ_handler(); }

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println(__FILE__);

  if (!mcp.begin()) {
    Serial.println("failed to initialize MCP");
    while (1)
      ;
  }
  mcp.enableScanChannel(MCP_DIFFA);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on default analog channel:
  int32_t adcdata = mcp.analogRead(MCP_DIFFA);

  // Convert the analog reading
  double voltage = adcdata * mcp.getReference() / mcp.getMaxValue();

  // print out the value you read:
  Serial.print("voltage: ");
  Serial.println(voltage, 10);
  // pause program for one second
  delay(1000);
}
