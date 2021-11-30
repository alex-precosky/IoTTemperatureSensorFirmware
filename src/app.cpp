#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Arduino.h"
#include "DallasTemperature.h"
#include "LowPower.h"
#include "OneWire.h"

// Arduino pin assignments
const uint8_t LED_PIN = 7;
const uint8_t SLEEP_PIN = 3;
const uint8_t ONE_WIRE_PIN = 8;
const uint8_t BATTERY_VOLTAGE_PIN = A1;

const int ADC_REF_CHANGE_READINGS = 5; // ADC readings performed after changing ADC reference voltage

const float ADC_MAX = 1023;
const float ADC_REF_V = 1.1;
const float BATTERY_VOLTAGE_DIVIDER_FACTOR = 5.3; // The battery voltage is
                                                  // measured at the bottom of a
                                                  // 530 kohm + 100 kohm voltage
                                                  // divider

const uint32_t LED_BLINK_ON_TIME_MS = 1; // The LED is blinked on this long when transmitting a message
const uint32_t RADIO_POWERUP_SLEEP_TIME_MS = 20; // The XBee is given this long
                                                 // after the wakeup pin is
                                                 // asserted before it is used
                                                 // to send a message
const uint32_t RADIO_SEND_TIME_MS = 25; // The radio is given this much time to
                                        // send a message before its sleep pin
                                        // is asserted

static float read_temperature();
static float read_battery_voltage();

OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature temperature_sensors(&oneWire);

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(SLEEP_PIN, OUTPUT);

    oneWire.reset();
    temperature_sensors.begin();
    temperature_sensors.setWaitForConversion(false);
    read_temperature();

    /* power down for one DS18B20 750ms sample period so that the first reading
       from the DS18B20 in loop() will not be the startup value of 85 deg C */
    LowPower.powerDown(SLEEP_500MS, ADC_OFF, BOD_OFF);
    LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);

    Serial.begin(57600);
}

void loop()
{
    char serial_msg[150];
    char temperature_str[10];
    char voltage_str[10];

    // measure the battery voltage
    // Use internal 1.1V analog reference for the temperature sensor
    analogReference(INTERNAL);

    // Doing some readings after changing the reference voltage
    float batteryVoltage;
    for(int i = 0; i < ADC_REF_CHANGE_READINGS; i++)
        batteryVoltage = read_battery_voltage();

    // do temperature measurement
    float temperature = read_temperature();

    // prepare the result that will be sent
    // Transmit the result
    const signed char min_width = 4;
    const unsigned char decimal_places = 2;
    dtostrf(temperature, min_width, decimal_places, temperature_str);
    dtostrf(batteryVoltage, min_width, decimal_places, voltage_str);
    sprintf(serial_msg, "sensor_data:station=alexfridge&names=temp,battVoltage&values=%s,%s&units=C,V", temperature_str, voltage_str);

    // wake up the xbee and give it some time.
    // take this opportunity to blink the LED
    digitalWrite(SLEEP_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);
    delay(LED_BLINK_ON_TIME_MS);
    digitalWrite(LED_PIN, LOW);
    delay(RADIO_POWERUP_SLEEP_TIME_MS);

    // send the message
    Serial.println(serial_msg);

    // sleep the xbee
    delay(RADIO_SEND_TIME_MS);
    digitalWrite(SLEEP_PIN, HIGH);

    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
}

static float read_temperature()
{
    temperature_sensors.requestTemperatures();
    return temperature_sensors.getTempCByIndex(0);
}

static float read_battery_voltage()
{
    return analogRead(BATTERY_VOLTAGE_PIN) / ADC_MAX * BATTERY_VOLTAGE_DIVIDER_FACTOR * ADC_REF_V * 1.032663; // last one was a calibration factor for the prototype
}
