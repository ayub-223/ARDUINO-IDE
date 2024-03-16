#include <RTClib.h>

RTC_DS3231 rtc;

const int temperaturePin = A0;
const int ledPin = 13;
int ledState = LOW;
int interval = 250; // Initial interval for LED blinking
DateTime previousTime;

void setup() {
    Serial.begin(9600);

    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    pinMode(ledPin, OUTPUT);
}

void loop() {
    DateTime now = rtc.now(); // Get the current time from the RTC module

    float voltage = getVoltage(temperaturePin); // Read the voltage from the LM35 temperature sensor
    float tempC = (voltage - 0.5) * 100.0; // Convert the voltage to temperature in Celsius

    if (tempC < 30) {
        interval = 250; // Set the LED blinking interval to 250ms if temperature is below 30 degrees Celsius
    } else {
        interval = 500; // Set the LED blinking interval to 500ms if temperature is above 30 degrees Celsius
    }

    if (now.unixtime() - previousTime.unixtime() >= interval / 1000) {
        // Check if the difference in Unix timestamps between the current time and previous time is greater than or equal to the interval converted to seconds
        previousTime = now; // Update the previous time to the current time
        ledState = (ledState == LOW) ? HIGH : LOW; // Toggle the LED state (ON to OFF or OFF to ON)
        digitalWrite(ledPin, ledState); // Control the onboard LED based on the toggled LED state
    }
}

float getVoltage(int pin) {
    return (analogRead(pin) * 5.0) / 1023.0;
}
