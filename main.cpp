#include "mbed.h"
#include "th02.h"
#include "TSL2561.h"
#include "ultrasonic.h"
#include "Servo.h"
#include "neopixel.h"
#include "stdlib.h"
#include "rtos.h"
#include "greenhouse.h"

using namespace std;

DigitalOut temp_alert(p21);
float tankHeight = 0.0f;      //INITIALIZE!!!

//Thread t1;
//Thread t2;

void dist(int distance){             //save plant height in inches!
    //prev_height = plant_height; 
    plant_height = tankHeight - (distance*0.0393701);
} 
ultrasonic mu(p6, p7, .1, 1, &dist);


void Irrigation(void const *args) {
    while(1){
        moisture_level = moisture;
        if (!watering && moisture_level > 0.3f)
            water_plant();
        Thread::wait(2000);         //SET CORRECT INTERVAL
    }
}

void Status(void const *args) {
  while(1){
      read_temp();
      read_humidity();
      mu.checkDistance();
      lux = sensor.lux();
      moisture_level = moisture;
      
      if (temperature < 15.0f || temperature > 30.0f)
        temp_alert = 1;
      else
        temp_alert = 0;
        
      if (moisture_level > 0.8f)
        water_plant();

      if (lux < 5.0f){
        lights_on = true;
        switch_lights();
      } else {
        lights_on = false;
        switch_lights();
      }
      
      Thread::wait(2000);           //SET CORRECT INTERVAL
    }  
}


int main() {
    char c=0;
    pi.baud(9600);
    mu.startUpdates();
    
    //t1.start(Irrigation);
    //t2.start(Status);
    Thread t1(Irrigation);
    Thread t2(Status);
    
    while(1) {
        c = pi.getc();
        if (c != 0) {
            switch(c){
                 case '1':
                    {
                    lights_on = true;
                    switch_lights();
                    break;
                    }
                case '0':
                    {
                    lights_on = false;
                    switch_lights();
                    break;
                    }
                case '2':
                    {
                    water_plant();
                    break;
                    }
                case '3':
                    {
                    //std::string s = patch::to_string((float)moisture_level);
                    std::string s = patch::to_string((float)sensor.lux());
                    pi.printf("%s",s);
                    break;
                    }
                case '4':
                    {
                    read_temp();
                    std::string s = patch::to_string((float)temperature);
                    pi.printf("%s",s);
                    break;
                    }
                case '5':
                    {
                    read_humidity();
                    std::string s = patch::to_string((float)humidity);
                    pi.printf("%s",s);
                    break;
                    }
                case '6':
                    {
                    mu.checkDistance();
                    std::string s = patch::to_string((float)sonar_dist);
                    pi.printf("%s",s);
                    break;
                    }
                default:
                    break;
                    
            }
            //pi.putc(0);
        }
        wait(1);
    }
     
    return 0;
}















