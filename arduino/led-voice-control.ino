/*
Control LED with Arduino using Voice Recognition Module

Library Link: https://github.com/DFRobot/DFRobot_DF2301Q
*/

#include "DFRobot_DF2301Q.h"

#define Led 13

/**
   @brief DFRobot_URM13_RTU constructor
   @param serial - serial ports for communication, supporting hard and soft serial ports
   @param rx - UART The pin for receiving data
   @param tx - UART The pin for transmitting data
*/
#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))  // Use software serial
SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
DFRobot_DF2301Q_UART asr(/*softSerial =*/&softSerial);
#elif defined(ESP32)  // Use the hardware serial with remappable pin: Serial1
DFRobot_DF2301Q_UART asr(/*hardSerial =*/&Serial1, /*rx =*/D3, /*tx =*/D2);
#else                 // Use hardware serial: Serial1
DFRobot_DF2301Q_UART asr(/*hardSerial =*/&Serial1);
#endif

void setup() {
  Serial.begin(115200);

  pinMode(Led, OUTPUT);    //Init LED pin to output mode 
  digitalWrite(Led, LOW);  //Set LED pin to low 

  // Init the sensor
  while (!(asr.begin())) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
     @brief Reset module 
  */
  // asr.resetModule();

  /**
     @brief Set commands of the module
     @param setType - Set type
     @n       DF2301Q_UART_MSG_CMD_SET_VOLUME: Set volume, the set value range 1-7 
     @n       DF2301Q_UART_MSG_CMD_SET_ENTERWAKEUP: Enter wake-up state; set value 0
     @n       DF2301Q_UART_MSG_CMD_SET_MUTE Mute mode; set value 1: mute, 0: unmute
     @n       DF2301Q_UART_MSG_CMD_SET_WAKE_TIME ; Wake-up duration; the set value range 0-255s
     @param setValue - Set value, refer to the set type above for the range
  */
  asr.settingCMD(DF2301Q_UART_MSG_CMD_SET_MUTE, 0);
  asr.settingCMD(DF2301Q_UART_MSG_CMD_SET_VOLUME, 7);
  asr.settingCMD(DF2301Q_UART_MSG_CMD_SET_WAKE_TIME, 20);
  //asr.settingCMD(DF2301Q_UART_MSG_CMD_SET_ENTERWAKEUP, 0);

  /**
     @brief Play the corresponding reply audio according to the command word ID
     @param CMDID - Command word ID
  */
  asr.playByCMDID(23);
}

void loop() {
  /**
     @brief  Get the ID corresponding to the command word 
     @return Return the obtained command word ID, returning 0 means no valid ID is obtained
  */
  uint8_t CMDID = asr.getCMDID();
  switch (CMDID) {
    case 103:                                                  //If the command is “Turn on the light”
      digitalWrite(Led, HIGH);                                 //Turn on the LED
      Serial.println("received'Turn on the light',command flag'103'");  //Serial transmits "received"Turn on the light",command flag"103""
      break;

    case 104:                                                  //If the command is “Turn off the light”
      digitalWrite(Led, LOW);                                  //Turn off the LED
      Serial.println("received'Turn off the light',command flag'104'");  //The serial transmits "received"Turn off the light",command flag"104""
      break;

    default:
      if (CMDID != 0) {
        Serial.print("CMDID = ");  //Print command ID
        Serial.println(CMDID);
      }
  }
  delay(300);
}