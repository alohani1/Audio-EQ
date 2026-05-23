#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include <I2S.h>
#include <SPI.h>
#include <arduinoFFT.h>
#include <ResponsiveAnalogRead.h>

//Pin layout:
#define pin1 1    //Placeholder values for now
#define pin2 2
#define pin3 3
#define pin4 4
#define pin5 5

//FFT DEFINITIONS AND VARIABLES
#define SAMPLES 256
#define Fsamp 44100

double Real[SAMPLES];
double Im[SAMPLES];

int bufferIndex = 0;
volatile bool bufferReady = false;

//potentiometer setup
volatile float gains[5] = {0.0f,0.0f,0.0f,0.0f,0.0f}; //Initialize to 0

//define Pin positions
//put potentiometers in sleep mode when inactive
ResponsiveAnalogRead pots[5] = {
  ResponsiveAnalogRead(pin1, true),
  ResponsiveAnalogRead(pin2, true),
  ResponsiveAnalogRead(pin3, true),
  ResponsiveAnalogRead(pin4, true),
  ResponsiveAnalogRead(pin5, true),
};

//initiate ArduinoFFT
ArduinoFFT<double>FFT = ArduinoFFT<double>(Real, Im, SAMPLES, Fsamp);


// put function declarations here:
void performFFT(double*, double*, uint16_t);
//FFT function
void performFFT(double* realArr, double* ImArr, uint16_t sampleSize) {
  //Reset Imaginary Array
  for (int i = 0; i < sampleSize; i++) {
    ImArr[i] = 0.0;
  }
  //Perform FFT
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();
}

//Variable 
//Biquad Global Variables
const float FS = 176400.0;
const float Q = 0.707;
const float centerF[5] = {100.0, 400.0, 1000.0, 4000.0, 10000.0};
//FFT global variables
double vReal[SAMPLES];
double vImg[SAMPLES];
int windowIndex = 0;
bool windowFull = false;

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

    void calculateCoefficients(float f0, float gain, float Fs) {
      // Calculate basic biquad coefficients based on gain and quality
      float A = pow(10, gain/40.0f);
      float w = 2.0f * PI * (f0/Fs);
      float s = sin(w);
      float c = cos(w);
      float alpha = s/(2.0f * Q);
      float a0 = 1.0f + (alpha/A);       // 0th order output term

      //Normalize each coefficient by dividing by a0
      b0 = (1.0f + (alpha*A)) / a0;
      b1 = (-2.0f * c) / a0;
      b2 = (1.0f - (alpha*A)) / a0;
      a1 = (-2.0f * c)/ a0;
      a2 = (1.0f-(alpha/A)) / a0;
    }
};
//Object Initialization
MovingAverage4 averageFilter;
BiquadFilter EQbands[5];

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