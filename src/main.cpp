// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2022 Thomas Basler and others
 */
#include "Configuration.h"
#include "Display_Graphic.h"
#include "InverterSettings.h"
#include "Led_Single.h"
#include "MessageOutput.h"
#include "MqttHandleDtu.h"
#include "MqttHandleHass.h"
#include "MqttHandleInverter.h"
#include "MqttSettings.h"
#include "NetworkSettings.h"
#include "NtpSettings.h"
#include "PinMapping.h"
#include "SunPosition.h"
#include "Utils.h"
#include "WebApi.h"
#include "defaults.h"
#include <Arduino.h>
#include <LittleFS.h>

// Anfang Christian:
// Serial Port for IR
#define RXD2 21
#define TXD2 22
//globe Variable zum speichern von Daten welche empfangen werden

unsigned char W_Bezug[] ={0x07, 0x01, 0x00, 0x01, 0x08, 0x00, 0xFF}; //Energie Summe, Bezug; OBIS 1.0-1.8.0
unsigned char W_Export[] ={0x07, 0x01, 0x00, 0x02, 0x08, 0x00, 0xFF}; //Energie Summe, Export; OBIS 1.0-2.8.0
unsigned char P_aktuell[]={0x07, 0x01, 0x00, 0x10, 0x07, 0x00, 0xFF}; //Momentane Wirkleistung; OBIS ???
unsigned char ServerID[] ={0x07, 0x01, 0x00, 0x00, 0x00, 0x09, 0xFF}; //ServerID; OBIS ???

unsigned char W_Bezug_buff[19];
unsigned char W_Export_buff[15];
unsigned char P_aktuell_buff[11];
unsigned char ServerID_buff[15];
unsigned long Bezug_Summe; 
unsigned long Export_Summe; 
unsigned long Paktuell_Summe; 


int index_Bezug =0;
int index_Export =0;
int index_P_aktuell =0;
int i;
unsigned char wert; 
//Ende Christian

void setup()
{
    // Initialize serial output
    Serial.begin(SERIAL_BAUDRATE);
    while (!Serial)
        yield();
    MessageOutput.println();
    MessageOutput.println("Starting OpenDTU");

    // Anfang Christian:
    // Initialize serial2 input
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    while (!Serial2)
        yield();
    MessageOutput.println();
    MessageOutput.println("Serial2 OK");
    // Ende Christian

    // Initialize file system
    MessageOutput.print("Initialize FS... ");
    if (!LittleFS.begin(false)) { // Do not format if mount failed
        MessageOutput.print("failed... trying to format...");
        if (!LittleFS.begin(true)) {
            MessageOutput.print("success");
        } else {
            MessageOutput.print("failed");
        }
    } else {
        MessageOutput.println("done");
    }

    // Read configuration values
    MessageOutput.print("Reading configuration... ");
    if (!Configuration.read()) {
        MessageOutput.print("initializing... ");
        Configuration.init();
        if (Configuration.write()) {
            MessageOutput.print("written... ");
        } else {
            MessageOutput.print("failed... ");
        }
    }
    if (Configuration.get().Cfg_Version != CONFIG_VERSION) {
        MessageOutput.print("migrated... ");
        Configuration.migrate();
    }
    CONFIG_T& config = Configuration.get();
    MessageOutput.println("done");

    // Load PinMapping
    MessageOutput.print("Reading PinMapping... ");
    if (PinMapping.init(String(Configuration.get().Dev_PinMapping))) {
        MessageOutput.print("found valid mapping ");
    } else {
        MessageOutput.print("using default config ");
    }
    const PinMapping_t& pin = PinMapping.get();
    MessageOutput.println("done");

    // Initialize WiFi
    MessageOutput.print("Initialize Network... ");
    NetworkSettings.init();
    MessageOutput.println("done");
    NetworkSettings.applyConfig();

    // Initialize NTP
    MessageOutput.print("Initialize NTP... ");
    NtpSettings.init();
    MessageOutput.println("done");

    // Initialize SunPosition
    MessageOutput.print("Initialize SunPosition... ");
    SunPosition.init();
    MessageOutput.println("done");

    // Initialize MqTT
    MessageOutput.print("Initialize MqTT... ");
    MqttSettings.init();
    MqttHandleDtu.init();
    MqttHandleInverter.init();
    MqttHandleHass.init();
    MessageOutput.println("done");

    // Initialize WebApi
    MessageOutput.print("Initialize WebApi... ");
    WebApi.init();
    MessageOutput.println("done");

    // Initialize Display
    MessageOutput.print("Initialize Display... ");
    Display.init(
        static_cast<DisplayType_t>(pin.display_type),
        pin.display_data,
        pin.display_clk,
        pin.display_cs,
        pin.display_reset);
    Display.setOrientation(config.Display_Rotation);
    Display.enablePowerSafe = config.Display_PowerSafe;
    Display.enableScreensaver = config.Display_ScreenSaver;
    Display.setContrast(config.Display_Contrast);
    Display.setStartupDisplay();
    MessageOutput.println("done");

    // Initialize Single LEDs
    MessageOutput.print("Initialize LEDs... ");
    LedSingle.init();
    MessageOutput.println("done");

    // Check for default DTU serial
    MessageOutput.print("Check for default DTU serial... ");
    if (config.Dtu_Serial == DTU_SERIAL) {
        MessageOutput.print("generate serial based on ESP chip id: ");
        uint64_t dtuId = Utils::generateDtuSerial();
        MessageOutput.printf("%0x%08x... ",
            ((uint32_t)((dtuId >> 32) & 0xFFFFFFFF)),
            ((uint32_t)(dtuId & 0xFFFFFFFF)));
        config.Dtu_Serial = dtuId;
        Configuration.write();
    }
    MessageOutput.println("done");

    InverterSettings.init();
}

void loop()
{
    NetworkSettings.loop();
    yield();
    InverterSettings.loop();
    yield();
    MqttHandleDtu.loop();
    yield();
    MqttHandleInverter.loop();
    yield();
    MqttHandleHass.loop();
    yield();
    WebApi.loop();
    yield();
    Display.loop();
    yield();
    SunPosition.loop();
    yield();
    MessageOutput.loop();
    yield();
    LedSingle.loop();
    yield();
    //Anfang Christian
    while (Serial2.available()>0)
    {
      wert=Serial2.read();
      
      //Bezug
      if (wert == W_Bezug[index_Bezug] )
      {
        index_Bezug++;
      }
      else
      {
        index_Bezug=0;
      }

      //Export
      if (wert == W_Export[index_Export] )
      {
        index_Export++;
      }
      else
      {
        index_Export=0;
      }

      //Momentanleistung
      if (wert == P_aktuell[index_P_aktuell] )
      {
        index_P_aktuell++;
      }
      else
      {
        index_P_aktuell=0;
      }


    
      if (index_Bezug==7) 
      {
        MessageOutput.print("Bezug gefunden  [0,1 Wh]: ");
        Serial2.readBytes(W_Bezug_buff, 19);
        Bezug_Summe= ((unsigned long)W_Bezug_buff[18]+(unsigned long)256*(unsigned long)W_Bezug_buff[17]+(unsigned long)256*(unsigned long)256*(unsigned long)W_Bezug_buff[16]+(unsigned long)256*(unsigned long)256*(unsigned long)256*(unsigned long)W_Bezug_buff[15]);
        MessageOutput.println(Bezug_Summe);
      }
      
      if (index_Export==7) 
      {
        MessageOutput.print("Export gefunden [0,1 Wh]: ");
        Serial2.readBytes(W_Export_buff, 15);
        Export_Summe= (unsigned long)((unsigned long)W_Export_buff[14]+(unsigned long)256*(unsigned long)W_Export_buff[13]+(unsigned long)256*(unsigned long)256*(unsigned long)W_Export_buff[12]+(unsigned long)256*(unsigned long)256*(unsigned long)256*(unsigned long)W_Export_buff[11]);
        MessageOutput.println(Export_Summe);
      }
       
      if (index_P_aktuell==7) 
      {
        MessageOutput.println("P_Aktuell gefunden   [W]: ");
        Serial2.readBytes(P_aktuell_buff, 11);
        Paktuell_Summe= (unsigned long)((unsigned long)P_aktuell_buff[10]+(unsigned long)256*(unsigned long)P_aktuell_buff[9]+(unsigned long)256*(unsigned long)256*(unsigned long)P_aktuell_buff[8]+(unsigned long)256*(unsigned long)256*(unsigned long)256*(unsigned long)P_aktuell_buff[7]);
        MessageOutput.println(Paktuell_Summe);
      }  
    }
    //Ende Christian



}