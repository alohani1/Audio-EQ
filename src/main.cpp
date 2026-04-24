#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include <I2S.h>
#include <SPI.h>
#include <arduinoFFT.h>
#include <ResponsiveAnalogRead.h>


// put function declarations here:
int myFunction(int, int);

//Variable Initializations
const float FS = 176400.0;
const float Q = 0.707;

//class declarations
class MovingAverage4 {
  private:
    float window[4] = {0,0,0,0};
    int index = 0;
    float movingSum = 0;
  public:
    float filter(float input) {
      movingSum -= window[index];
      window[index] = input;
      movingSum += input;
      index = (index + 1)%4;

      return movingSum / 4.0f;
    }
};

MovingAverage4 averageFilter;

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}