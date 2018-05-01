#ifndef GREENHOUSE_H
#define GREENHOUSE_H
#include <string>11
#include <sstream>


#define DATA_PIN p5

Mutex lightmutex;
Mutex watermutex;  

DigitalOut myled(LED1);
AnalogIn moisture(A0);
RawSerial  pi(USBTX, USBRX);
Servo myservo(p24);
TH02 MyTH02 (p9,p10,TH02_I2C_ADDR<<1);  
TSL2561 sensor(p28, p27); 

bool lights_on = false;           
bool watering = false;
//float prev_height = 0.0f;
float plant_height = 0.0f;
float temperature = 0.0f;
float humidity = 0.0f;
float moisture_level = 0.0f;
float lux = 0.0f;

//patch::to_string()
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

//function to water the plant
void water_plant() {
    watering = true;
    watermutex.lock();
    for(float p=0; p<1.5; p += 0.05) {
        myservo = p;
        wait(0.1);
    }
    wait(1);
    for(float p=1.5; p>0.0; p -= 0.05) {
        myservo = p;
        wait(0.1);
    }
    watermutex.unlock();
    watering = false;
}

//function to turn on/off neo-pixels
void switch_lights() {
    DigitalIn(DATA_PIN, PullDown);
    neopixel::PixelArray array(DATA_PIN);  
    uint16_t numPixels = 32;
    neopixel::Pixel pixels[numPixels];
    if (lights_on){
        for (int i=0; i<numPixels; ++i){
            pixels[i].red = 255;
            pixels[i].green = 255;
            pixels[i].blue = 255;
        }
    } else { 
        for (int i=0; i<numPixels; ++i){
            pixels[i].red = 0;
            pixels[i].green = 0;
            pixels[i].blue = 0;
        }
    }
    lightmutex.lock();
    array.update(pixels, numPixels);   
    lightmutex.unlock();     
}


void read_temp(){
    int iTemp;
    MyTH02.startTempConv(true,true);
    MyTH02.waitEndConversion();
    iTemp= MyTH02.getConversionValue();
    temperature = iTemp/10.0;
}
    
    
void read_humidity() {
    int iRH;
    MyTH02.startRHConv(true,true);
    MyTH02.waitEndConversion();
    iRH= MyTH02.getConversionValue();
    humidity = iRH/10.0;
}



#endif
