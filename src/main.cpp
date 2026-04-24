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
    float window[4] = {0,0,0,0};  //4-point Filter Window
    int index = 0;  // current index
    float movingSum = 0;  //averaged sum
  public:
    float filter(float input) {
      //Calculate Total Sum by subtracting oldest input and adding newest
      movingSum -= window[index];
      window[index] = input;
      movingSum += input;
      index = (index + 1)%4;   //Wrap index

      return movingSum / 4.0f;  // Average
    }
};
class BiquadFilter {
  private:
    float z1 = 0; //First Order Term
    float z2 = 0; //Second Order Term
  public:
    float b0,b1,b2,a1,a2; 

    BiquadFilter() {
      z1 = 0; z2 = 0;
      b0 = 1; b1 = 0; b2 = 0; a1 = 0; a2 = 0;
    }

    float filter(float in) {
      float out = (in*b0) + z1; //Filter Difference Equation

      //Calculate next first and second order terms
      z1 = (in*b1) - (out * a1) + z2; 
      z2 = (in*b2) - (out*a2);

      
      return out;
    }
};
//Object Initialization
MovingAverage4 averageFilter;
BiquadFilter band1;
BiquadFilter band2;
BiquadFilter band3;
BiquadFilter band4;
BiquadFilter band5;

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