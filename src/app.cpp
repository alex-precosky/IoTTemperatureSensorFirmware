#include "LowPower.h"

static const int XBEE_SLEEP_GPIO_PIN = 3;
static const int LED_GPIO_PIN = 7;
static const int TEMP_SENSOR_ADC_PIN = A0;
static const int BATTERY_VOLTAGE_ADC_PIN = A1;

static const int XBEE_WAKEUP_DELAY_MS = 10;

static void SelectInternalADCRef();

void setup() {
    pinMode(XBEE_SLEEP_GPIO_PIN, OUTPUT);
    Serial.begin(57600);
    analogReference(INTERNAL);
}

void loop() {
    digitalWrite(LED_GPIO_PIN, HIGH);

    // wake up the xbee and give it some time to come up
    digitalWrite(XBEE_SLEEP_GPIO_PIN, LOW);
    delay(XBEE_WAKEUP_DELAY_MS);

    SelectInternalADCRef();

    int sensorValue = analogRead(TEMP_SENSOR_ADC_PIN);
    int batteryValue = analogRead(BATTERY_VOLTAGE_ADC_PIN);

    // Convert ADC values to units
    float temperature = sensorValue * (1.1 / 1023.0) * 100;  // * 100 because the sensor outupts 10 mV per degree C above 0
    float batteryVoltage = batteryValue * (1.1 / 1023.0) * 5.5696;

    // Transmit the result
    Serial.print("sensor_data:station=alexfridge&names=temp,battVoltage&values=");
    Serial.print(temperature);
    Serial.print(",");
    Serial.print(batteryVoltage);
    Serial.println("&units=C,V");

    //sleep the xbee
    digitalWrite(XBEE_SLEEP_GPIO_PIN, HIGH);
    delay(20);

    digitalWrite(LED_GPIO_PIN, LOW);

    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
}

static void SelectInternalADCRef()
{
    // Use internal 1.1V analog reference for the temperature sensor
    analogReference(INTERNAL);
    delay(20);

    // Doing some readings after changing the reference voltage
    for(int i = 0; i < 50; i++) {
        (void)analogRead(BATTERY_VOLTAGE_ADC_PIN);
        (void)analogRead(TEMP_SENSOR_ADC_PIN);
    }

    delay(7);

}
