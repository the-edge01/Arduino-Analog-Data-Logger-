#include <SD.h> //has functions for writing data to the SD card

//Attritibutes for controlling data collection of the program
const int analogStartPin=A0;//lowest analog pin for collection
const int numberOfPins=1;//number of pins to collect starting at start pin, (includes start pin)
const int timeBetweenReads=100-15;//miliseconds between reads 
const int collectionMultiplier=1;//multiplier if voltage division resistors are used to make it a 5v signal.  1=no division, 2=single division (half voltage)
const int dataCollectionPin=7;//give this pin a 5V signal to collect data
const int ledPin=13;//status LED (13 is internal LED)

//(c) Eric Johnson 2015
//536ejohnson@gmail.com
//MIT Licence
//slightly based off https://www.arduino.cc/en/Tutorial/Datalogger

void setup(){
  pinMode(ledPin,OUTPUT);
  pinMode(dataCollectionPin,INPUT);
  Serial.begin(9600);//set up serial so computer can see live data
  while(!SD.begin(4)){//attempt to start SD Card
  Serial.println("SD failed");
    for(int i=0; i<100;i++){//blink LED if failed
      digitalWrite(ledPin,HIGH);
      delay(50);
      digitalWrite(ledPin,LOW);
      delay(50);
    }
  }
  digitalWrite(ledPin,LOW);//
  Serial.println("SD started");
}

void loop(){
  //write header to file
  writeStringLineSD("Time,Analog Datas");
  while(true){//always loop
    Serial.println("loop dely");
    delay(timeBetweenReads);
    if(digitalRead(dataCollectionPin)==HIGH){//collect data
      Serial.println("collect data");
      digitalWrite(ledPin,HIGH);
      
      String data="";
      //add time to string
      data+=String(millis());
      data+=",";
      for(int i=analogStartPin; i<analogStartPin+numberOfPins;i++){//for each pin we want to read
        long aValue=analogRead(i);//10 bit value
        aValue=aValue*5000/1023;//scale to 5000 millivolts
        aValue=aValue*collectionMultiplier;//account for voltage division
        data+=String(aValue);
        data+=",";
      }
      writeStringLineSD(data);//write data to disk.
      
    }
    else //do not read data
    {
      Serial.println("not collect data");
      digitalWrite(ledPin,LOW);
      delay(250);
    }
  }
}
void writeStringLineSD(String s){
  File dataFile = SD.open("datalog.txt", FILE_WRITE);//should append
  if (dataFile) {// if the file is available, write to it:
    dataFile.println(s);
    dataFile.close();
    // print to the serial port too:
    Serial.println("datra to disk   "+s);
   }
   else {// if the file isn't open, pop up an error:
     Serial.println("error opening datalog.txt");
     
     while(true){//blink LED forever until reset
      digitalWrite(ledPin,HIGH);
      delay(50);
      digitalWrite(ledPin,LOW);
      delay(50);
    }
   }
}
