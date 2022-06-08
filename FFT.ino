#include "arduinoFFT.h"

#define SAMPLES 128 
#define SAMPLING_FREQUENCY 2048


arduinoFFT FFT = arduinoFFT();

unsigned int samplingPeriod;
unsigned long microSeconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

int max_low = 390 , max_mid = 700 , max_high = 1000;
int lowLed = 9, midLed = 10 , highLed = 11;
double lowPeak = 0 ,midPeak=0 , highPeak =0;
double lowV,midV,highV;
int reduce =5;
void setup() {
  Serial.begin(115200);
  samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY));

  pinMode(lowLed,OUTPUT);
  pinMode(midLed,OUTPUT);
  pinMode(highLed,OUTPUT);

}

void loop() {

  for(int i=0 ; i<SAMPLES;i++){
    microSeconds = micros();

    vReal[i] = analogRead(0);
    vImag[i] = 0;

    while(micros() < (microSeconds+samplingPeriod)){
      
    }
  }

  FFT.Windowing(vReal , SAMPLES,FFT_WIN_TYP_HAMMING , FFT_FORWARD);
  FFT.Compute(vReal,vImag , SAMPLES , FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal,vImag,SAMPLES);

  double peak = FFT.MajorPeak(vReal,SAMPLES,SAMPLING_FREQUENCY);
  Serial.println(peak);
  
}


int detarmainPeak(int peak){
  if(peak >= max_low && peak <= max_mid){
  midPeak = peak;
  }
  else if(midPeak > 0){
    midPeak-=reduce;
  }
  if(peak > 80 && peak <= max_low){
  lowPeak = peak;
  
  }
  else if(lowPeak > 0){
    lowPeak-=30;
    
  }
  if(peak > max_mid){
  highPeak = peak;
  }
  else if(highPeak > 0){
    highPeak-=reduce;
  }
}
void lightEQ(){
  // 
  lowV = (0.82258)*lowPeak- 65.8;
  midV = (0.82258)*midPeak - 320.8;
  highV = (0.85)*highPeak - 595;

  if(lowV <=0) lowV=0;
  if(midV<=0) midV=0;
  if(highV<=0) highV=0;

  analogWrite(lowLed,lowV);
  analogWrite(midLed,midV);
  analogWrite(highLed,highV);
  
}
