#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "LowPower.h"

const uint8_t LED_PIN = 7;
const uint8_t SLEEP_PIN = 3;
const uint8_t ONE_WIRE_PIN = 8;
const uint8_t BATTERY_VOLTAGE_PIN = A1;

float read_temperature();
float read_battery_voltage();

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

    // wait the sample time so that the first reading in loop() will not be the startup value of 85 deg C
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
    for(int i = 0; i < 5; i++)
        batteryVoltage = read_battery_voltage();

    // do temperature measurement
    float temperature = read_temperature();

    // prepare the result that will be sent
    // Transmit the result
    dtostrf(temperature, 4, 2, temperature_str);
    dtostrf(batteryVoltage, 4, 2, voltage_str);
    sprintf(serial_msg, "sensor_data:station=alexfridge&names=temp,battVoltage&values=%s,%s&units=C,V", temperature_str, voltage_str);

    // wake up the xbee and give it some time.
    // take this opportunity to blink the LED
    digitalWrite(SLEEP_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);
    delay(1);
    digitalWrite(LED_PIN, LOW);
    delay(20);

    // send the message
    Serial.println(serial_msg);

    //sleep the xbee
    delay(25);
    digitalWrite(SLEEP_PIN, HIGH);

    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
}

float read_temperature()
{
    temperature_sensors.requestTemperatures();
    return temperature_sensors.getTempCByIndex(0);
}

float read_battery_voltage()
{
    return analogRead(BATTERY_VOLTAGE_PIN) / 1024.0 * 5.3 * 1.1 * 1.032663; // last one was a calibration factor for the prototype
}
