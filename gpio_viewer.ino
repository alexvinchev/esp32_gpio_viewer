#include "src/gpio_viewer.h"
#include <WiFi.h>
#include <SimpleRotary.h> // Install this library with the Arduino IDE Library Manager
#include <Servo.h>
#include "secrets.h"

GPIOViewer gpio_viewer;

Servo myservo; // create servo object to control a servo
int pos = 0;   // variable to store the servo position

#define ROTARY_PIN_A 23
#define ROTARY_PIN_B 22
#define ROTARY_PUSH_BUTTON 22 // Not used
SimpleRotary rotary(ROTARY_PIN_A, ROTARY_PIN_B, ROTARY_PUSH_BUTTON);

int test_digital_pins[] = {33, 25, 26};
const int testDigitalPinsCount = sizeof(test_digital_pins) / sizeof(test_digital_pins[0]);
int currentLed = 0; // Start with the first LED

const int freq = 200;
const int resolution = 16;

struct PWM_PINS
{
  int pin;
  int channel;
  uint16_t level;
};

PWM_PINS test_pwm_pins[] = {{15, 0}, {2, 1}, {0, 2}, {4, 3}};
const int testPWMPinsCount = sizeof(test_pwm_pins) / sizeof(test_pwm_pins[0]);

void setup()
{
  Serial.begin(115200);

  gpio_viewer.connectToWifi(ssid, password); // If your code aleady include connection to Wifi, you can comment this line
  gpio_viewer.setPort(5555);                 // You can set the http port, if not set default port is 8080
  gpio_viewer.setSamplingInterval(100);

  // Example - Your own setup code start here
  test2_setup();

  // Must be at the end of your setup
  gpio_viewer.setSamplingInterval(25); // You can set the sampling interval in ms, if not set default is 50ms
  gpio_viewer.begin();
}

void loop()
{
  test2_loop();
}

void test2_setup()
{
  myservo.attach(13); // attaches the servo on pin 13 to the servo object
}

void test2_loop()
{
  for (pos = 0; pos <= 180; pos += 1)
  { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);          // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1)
  {                     // goes from 180 degrees to 0 degrees
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);          // waits 15ms for the servo to reach the position
  }
}

void test1_setup()
{
  uint16_t amount = 0;
  for (int i = 0; i < testPWMPinsCount; i++)
  {
    amount += (65535 / testPWMPinsCount);
    ledcSetup(test_pwm_pins[i].channel, freq, resolution);
    ledcAttachPin(test_pwm_pins[i].pin, test_pwm_pins[i].channel);
    test_pwm_pins[i].level = amount;
  }
  for (int i = 0; i < testDigitalPinsCount; i++)
  {
    pinMode(test_digital_pins[i], OUTPUT);
    digitalWrite(test_digital_pins[i], LOW);
  }
  xTaskCreate(readRotaryEncoderTask, // Task function
              "ReadRotaryEncoder",   // Name of the task (for debugging)
              2048,                  // Stack size (bytes)
              NULL,                  // Parameter to pass to the function
              1,                     // Task priority
              NULL);
}
void test1_loop()
{
  for (int i = 0; i < testPWMPinsCount; i++)
  {
    ledcWrite(test_pwm_pins[i].channel, test_pwm_pins[i].level);
    delay(150);
  }
  delay(300);
  for (int i = 0; i < testPWMPinsCount; i++)
  {
    ledcWrite(test_pwm_pins[i].channel, test_pwm_pins[i].level / 2);
    delay(150);
  }
  delay(300);
}

void updateLeds()
{
  for (int i = 0; i < testDigitalPinsCount; i++)
  {
    digitalWrite(test_digital_pins[i], i == currentLed ? HIGH : LOW);
  }
}

void readRotaryEncoderTask(void *pvParameters)
{
  for (;;)
  { // Infinite loop
    readRotaryEncoder();
    vTaskDelay(pdMS_TO_TICKS(10)); // Delay for debouncing, adjust as needed
  }
}

void readRotaryEncoder(void)
{
  byte i;
  i = rotary.rotate();

  if (i == 1)
  {
    currentLed = (currentLed - 1 + testDigitalPinsCount) % testDigitalPinsCount;
    updateLeds();
    Serial.println("CounterClockwise");
  }

  if (i == 2)
  {
    currentLed = (currentLed + 1) % testDigitalPinsCount;
    updateLeds();
    Serial.println("Clockwise");
  }
}
