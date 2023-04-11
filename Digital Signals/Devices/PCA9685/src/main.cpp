#include <Arduino.h>
#include "Wire.h"

#include "Adafruit_PWMServoDriver.h"
#include "PWM.h"
#include "Statistics.h"
#include "Timer.h"
#include "LinearMap.h"
#include "RandomNumber.h"

// CAUTION - BEWARE
// DO NOT USE MORE THAN ONE STEP
// AT ANY ONE TIME....
// Tests Steps 1, 2, or 3
#define TEST_STEP_4

nmr::Statistics<float> stat;
int statSize = 10;
// Wide range of random values... used for statistics
// These values will be swapped out for others....
float tuples[]{3.2, 5.3, 7.0, 2.2, 9.8, 4.6, 0.5, 4.5, 7.4, 6.5};

// Begin counter at zero...
uint8_t cnt = 0;

// Loop timer
nmr::Timer loopTimer;

// Linear Mapping
nmr::LinearMap<float> mapLeft;
nmr::LinearMap<float> mapRight;

// Random Number
nmr::RandomNumber<float> randNum;

// PWM Signal Calculations
dsg::PWM<float> pwmSignal;
uint8_t PWM_Level;
// Used for STEP (1)
#define ARDUINO_PWM_GENERATOR_PIN 5
#define ARDUINO_INPUT_PIN 12

// Controller Board: Adafruit PCA9685 16-Channel Servo Driver.
//                   * Address = 0x40
//                   * Frequency: Use about 50-200 Hz
//                   Servo Motors uses low frequencies...
//                   Start at 50, record your values, and then
//                   increase the frequency and compare results.
//
// (1) Arduino Self Testing: Simply connect the Arduino
// GeneratorPin to the Arduino InputPin. - WITHOUT I2C
// CONNECTIONS -
//
// DO NOT USE the Steps (1), (2) or (3) at the same time...
//
// (2) Adafruit Driver Testing: Simply connect the Adafruit Driver
// PWM pin on the driver from port 15 to the Arduino InputPin. Remember
// to connect the GND (ground) between the driver output to Arduino's
// GND. Connect Adafruit PWM pin to the Arduino InputPin, thereby removing
// Arduino's Generator connection.  - WITH I2C CONNECTIONS -
//
// PENDING STEP 3
//
// (3) Adafruit servo motor (BE CAREFUL - SERVO CALIBRATION)...
// Instead of using the last output port, you may choose any port
// for the servo motor...
// Remember to program in the port number...
// Simply connect the PWM pin on the driver from any port
// number to the Servo motor.  - WITH I2C CONNECTIONS -
// CAUTION: ALWAYS BEGIN MOTOR TEST AT THE CENTER POSITION
// AND WORK OUTWARDS FROM THE CENTER.... BE READY TO DISCONNECT
// POWER TO THE SERVO IF IT ROTATE BEYOND ITS LIMITS....
// The following source is active and maintained by Adafruit.
// Source: Adafruit PCA9685 16-Channel Servo Driver by Bill Earl pg.16
//
// This method uses the default address 0x40
Adafruit_PWMServoDriver servo = Adafruit_PWMServoDriver();
// Following values were emperically obtained by experimenting...
// starting only at the center with a rational guess not to damage
// the servo...
#define SERVO_LEFT 480   // OFFSET FROM-CENTER 45 degrees (135 degrees)
#define SERVO_CENTER 360 // CENTER at 90 degrees between 0 and 180...
#define SERVO_RIGHT 235  // OFFSET FROM-CENTER 45 degrees (45 degrees)
// Adafruit
#define SERVO_FREQ 50
#define SERVO_NUM_3 3   // for Servo Motor (STEP 3)
#define SERVO_NUM_15 15 // for Arduino Input (STEP 2)

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
    }

    // I2C - TWOWIRE
    Wire.begin();

    // Adafruit Servo Driver
    servo.begin();
    // The following values works for the
    // servo motor being used... Yours might
    // be different... Read Adafruit's PDF
    // file...
    servo.setOscillatorFrequency(23000000);
    servo.setPWMFreq(SERVO_FREQ);

    // Using Arduino Uno input pin 12
    // Used for calculating PWM signals
    pwmSignal = dsg::PWM<float>(12);

    // Setup Arduino Pins
    pinMode(ARDUINO_PWM_GENERATOR_PIN, OUTPUT);
    // Will use adafruit drive to generate PWM signals...
    pinMode(ARDUINO_INPUT_PIN, INPUT);
    // Used to normalize values
    stat = nmr::Statistics<float>(tuples, statSize);
    // OK to use delay() within setup() method

    // Mapping Degrees to PWM
    mapLeft = nmr::LinearMap<float>(90.0, 135.0, SERVO_CENTER, SERVO_LEFT);
    mapRight = nmr::LinearMap<float>(45.0, 90.0, SERVO_RIGHT, SERVO_CENTER);

    // Randon angle from 30 to 150 degrees
    randNum = nmr::RandomNumber<float>(30.0, 150.0);

    delay(10);
}

// Statistical indices
int index = 0;
bool stable()
{
    bool flag = false;

    stat.Queue(pwmSignal.DutyCyclePercent(), index);
    // Increment index
    index++;
    // Reset index
    if (index >= statSize)
        index = 0;

    // Debug
    // Serial.print(stat.Average());
    // Serial.print(" ");
    // Serial.println(stat.StandardDeviation());
    //
    // Is data stable?
    if (stat.StandardDeviation() < (float)0.1)
    {
        flag = true;
    }

    return flag;
}

void DebugPeriodFrequncy(int cnt)
{
    Serial.print(cnt);
    Serial.print(" Period (T): ");
    Serial.print(pwmSignal.Period());
    Serial.print(" Frequency (f): ");
    Serial.println(pwmSignal.Frequency());
}

void DebugDutyCycle(int cnt, float volt)
{
    Serial.print(cnt);
    Serial.print(" Volt: ");
    Serial.print(volt);
    Serial.print(" Duty cycle (%): ");
    Serial.println(String(pwmSignal.DutyCyclePercent(), 2));
    if (cnt == 4)
        Serial.println("");
}

void loop()
{
    pwmSignal.Update();

#ifdef TEST_STEP_1
    // STEP (1) TESTING ARDUINO GENERATOR SELF-TESTING
    // Trigger at every 250ms
    if (loopTimer.isTimer(250))
    {
        if (cnt == 0)
        {
            // Repeat several times
            // because debug requires a generator
            // as with analogWrite() or servo class
            DebugPeriodFrequncy(cnt);

            // PREVENT LOOPING OR INCLUDE NEXT STEP
            cnt++;
        }
        else if (cnt == 1)
        {
            if (stable())
            {
                DebugDutyCycle(cnt, 1.25);
                // Reset to repeat test
                stat.QueueReset();

                // PREVENT LOOPING OR INCLUDE NEXT STEP
                cnt++;
            }
            else
            {
                PWM_Level = pwmSignal.Volts2pwmLevel(1.25);
                analogWrite(ARDUINO_PWM_GENERATOR_PIN, PWM_Level);
            }
        }
        else if (cnt == 2)
        {
            if (stable())
            {
                DebugDutyCycle(cnt, 2.5);
                // Reset to repeat test
                stat.QueueReset();

                // PREVENT LOOPING OR INCLUDE NEXT STEP
                cnt++;
            }
            else
            {
                PWM_Level = pwmSignal.Volts2pwmLevel(2.5);
                analogWrite(ARDUINO_PWM_GENERATOR_PIN, PWM_Level);
            }
        }
        else if (cnt == 3)
        {
            if (stable())
            {
                DebugDutyCycle(cnt, 3.75);
                // Reset to repeat test
                stat.QueueReset();

                // PREVENT LOOPING OR INCLUDE NEXT STEP
                cnt++;
            }
            else
            {
                PWM_Level = pwmSignal.Volts2pwmLevel(3.75);
                analogWrite(ARDUINO_PWM_GENERATOR_PIN, PWM_Level);
            }
        }
        else if (cnt == 4)
        {
            if (stable())
            {
                DebugDutyCycle(cnt, 4.99);
                // Reset to repeat test
                stat.QueueReset();

                // PREVENT LOOPING OR INCLUDE NEXT STEP
                cnt = 0;
            }
            else
            {
                PWM_Level = pwmSignal.Volts2pwmLevel(4.99);
                analogWrite(ARDUINO_PWM_GENERATOR_PIN, PWM_Level);
            }
        }
    }
#endif

#ifdef TEST_STEP_2
    // STEP (2) CAUTION - TESTING ADAFRUIT DRIVER TO ARDUINO INPUT
    // Trigger at every 250ms
    if (loopTimer.isTimer(250))
    {
        if (cnt == 0)
        {
            // Repeat several times
            // because debug requires a generator
            // as with analogWrite() or servo class
            DebugPeriodFrequncy(cnt);

            // PREVENT LOOPING OR INCLUDE NEXT STEP
            cnt = 1;
        }

        if (cnt == 1)
        {
            if (stable())
            {
                DebugDutyCycle(cnt, 0);
                // Reset to repeat test
                stat.QueueReset();

                // PREVENT LOOPING OR INCLUDE NEXT STEP
                cnt = 2;
            }
            else
            {
                servo.setPWM(SERVO_NUM_15, 0, SERVO_CENTER);
            }
        }

        if (cnt == 2)
        {
            if (stable())
            {
                DebugDutyCycle(cnt, 0);
                // Reset to repeat test
                stat.QueueReset();

                // PREVENT LOOPING OR INCLUDE NEXT STEP
                cnt = 3;
            }
            else
            {
                servo.setPWM(SERVO_NUM_15, 0, SERVO_LEFT);
            }
        }

        if (cnt == 3)
        {
            if (stable())
            {
                DebugDutyCycle(cnt, 0);
                // Reset to repeat test
                stat.QueueReset();

                // PREVENT LOOPING OR INCLUDE NEXT STEP
                cnt = 0;
            }
            else
            {
                servo.setPWM(SERVO_NUM_15, 0, SERVO_RIGHT);
            }
        }
    }
#endif

#ifdef TEST_STEP_3
    // STEP (3) CAUTION - SERVO MOTOR - CALIBRATION
    // Calibrating SERVO_LEFT for 135 degrees
    // Calibrating  SERVO_CENTER for 90 degrees between 0 and 180...
    // Calibrating  SERVO_RIGHT for 45 degrees
    // SET the actual values above...
    // Trigger at every 2s
    if (loopTimer.isTimer(2000))
    {
        if (cnt == 0)
        {
            // CENTER: Calibrate to 90 degrees
            servo.setPWM(SERVO_NUM_3, 0, SERVO_CENTER);
            Serial.print("Center: ");
            Serial.println(SERVO_CENTER);
            cnt++;
        }
        else if (cnt == 1)
        {
            // LEFT: Calibrate to 45 degrees offset (135 degrees)
            servo.setPWM(SERVO_NUM_3, 0, SERVO_LEFT);
            Serial.print("LEFT: ");
            Serial.println(SERVO_LEFT);
            cnt++;
        }
        else if (cnt == 2)
        {
            // CENTER: Calibrate to 90 degrees
            servo.setPWM(SERVO_NUM_3, 0, SERVO_CENTER);
            Serial.print("Center: ");
            Serial.println(SERVO_CENTER);
            cnt++;
        }
        else if (cnt == 3)
        {
            // RIGHT: Calibrate to 45 degrees offset (45 degrees)
            servo.setPWM(SERVO_NUM_3, 0, SERVO_RIGHT);
            Serial.print("RIGHT: ");
            Serial.println(SERVO_RIGHT);
            cnt = 0;
        }
    }
#endif

#ifdef TEST_STEP_4
    // STEP (4) Mapping - SERVO MOTOR
    if (loopTimer.isTimer(3000))
    {
        // Angle
        float angle = randNum.Random();

        // Map angle to tick
        uint16_t tick;
        if (angle > 90.0)
            tick = mapLeft.Map(angle);
        else
            tick = mapRight.Map(angle);

        // Set PWM
        servo.setPWM(SERVO_NUM_3, 0, tick);

        // Debug
        Serial.print("angle: ");
        Serial.print(angle);
        Serial.print(" tick: ");
        Serial.println(tick);
    }
#endif
}
