#include <Arduino.h>
#include <Wire.h>

#include "HMC5883L.h"

uno::HMC5883L compass;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
    }

    // Initialize Initialize HMC5883L
    Serial.println("Initialize HMC5883L");
    while (!compass.begin())
    {
        Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
        delay(500);
    }
    // Set measurement range
    compass.setRange(uno::HMC5883L_RANGE_1_3GA);
    // Set measurement mode
    compass.setMeasurementMode(uno::HMC5883L_CONTINOUS);
    // Set data rate
    compass.setDataRate(uno::HMC5883L_DATARATE_30HZ);
    // Set number of samples averaged
    compass.setSamples(uno::HMC5883L_SAMPLES_8);
    // Set calibration offset. See HMC5883L_calibration.ino
    //
    // 20230411 ---------------------------------------------------- TESTME
    // Data obtained from calibration...
    // 254.00:-389.00:-428:417:-544:295:-5:-124
    // Moved compass in all direction until the offset
    // values became stable...
    // Surprisingly, the results align nicely ---------------------- results
    // with the magnetic north... More testing
    // required...
    compass.setOffset(-5, -124);
}

void loop()
{
    // uno::Vector vec = compass.readRaw();
    uno::Vector vec = compass.readNormalize();

    // Serial.print("x: ");
    // Serial.print(vec.XAxis);
    // Serial.print("x: ");
    // Serial.print(vec.YAxis);
    // Serial.print("x: ");
    // Serial.println(vec.ZAxis);

    // Calculate heading
    float heading = atan2(vec.YAxis, vec.XAxis);

    // Set declination angle on your location and fix heading
    // You can find your declination on: http://magnetic-declination.com/
    // (+) Positive or (-) for negative
    // For Bytom / Poland declination angle is 4'26E (positive)
    // Formula: (deg + (min / 60.0)) / (180 / M_PI);
    // float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
    // 20230411 ---------------------------------------------------- TESTME
    // This is author's location...
    // Albuquerque, NM, USA ---------------------------------------- tested
    // See results above...
    float declinationAngle = (8.0 + (3.0 / 60.0)) * DEG_TO_RAD;
    heading += declinationAngle;

    // Correct for heading < 0deg and heading > 360deg
    if (heading < 0)
        heading += 2 * PI;

    if (heading > 2 * PI)
        heading -= 2 * PI;

    // Convert to degrees
    float headingDegrees = heading * RAD_TO_DEG;

    // Output
    Serial.print(" Heading Radians = ");
    Serial.print(heading);
    Serial.print(" Degrees = ");
    Serial.print(headingDegrees);
    Serial.println();

    delay(500);
}