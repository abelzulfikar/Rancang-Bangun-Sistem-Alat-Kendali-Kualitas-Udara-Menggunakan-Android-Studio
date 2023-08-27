

//Include the library
#include <MQUnifiedsensor.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//Definitions
#define placa "ESP-32"
#define Voltage_Resolution 3.3
#define pin 35 //Analog input 0 of your arduino
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 12 // For ESP32
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  

const int pinGP2Y = 34;
int pinLED = 27;
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

//Declare Sensor
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

char jenisgas[6][10] = {"CO","Alcohol","CO2","Toluen","NH4","Aceton"};
float gasA [6] = {605.18 , 77.255 , 110.47 , 44.947 , 102.2 , 34.668};
float gasB [10] = {-3.937 , -3.18 , -2.862 , -3.445 , -2.473 , -3.369};
int CO = 0;

#define relaypin 5
 
LiquidCrystal_I2C lcd(0x27,16,2);
 

 
byte pic_termo[8] = //icon for termometer
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B00000
};
 
byte pic_humd[8] = //icon for water droplet
{
    B00000,
  B00010,
  B00101,
  B01000,
  B10100,
  B00010,
  B00101,
  B00000
};
 
byte lonceng[8] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};
 
byte lonceng1[8] = {
  B11011,
  B10001,
  B10001,
  B10001,
  B10001,
  B00000,
  B11111,
  B11011
};

void setup() {
  //Init the serial port communication - to debug the library
  Serial.begin(115200); //Init serial port
  pinMode(relaypin, OUTPUT);
  pinMode(pinLED,OUTPUT);
  
  //Set math model to calculate the PPM concentration and the value of constants
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135.setA(gasA[CO]); MQ135.setB(gasB[CO]); // Configure the equation to to calculate CO concentration
  /*
    Exponential regression:
  GAS      | a      | b
  CO       | 605.18 | -3.937  
  Alcohol  | 77.255 | -3.18 
  CO2      | 110.47 | -2.862
  Toluen  | 44.947 | -3.445
  NH4      | 102.2  | -2.473
  Aceton  | 34.668 | -3.369
  */
  
  /*****************************  MQ Init ********************************************/ 
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 
  MQ135.init(); 
  /* 
    //If the RL value is different from 10K please assign your RL value with the following method:
    MQ135.setRL(10);
  */
  /*****************************  MQ CAlibration ********************************************/ 
  // Explanation: 
   // In this routine the sensor will measure the resistance of the sensor supposedly before being pre-heated
  // and on clean air (Calibration conditions), setting up R0 value.
  // We recomend executing this routine only on setup in laboratory conditions.
  // This routine does not need to be executed on each restart, you can load your R0 value from eeprom.
  // Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ135.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 
  MQ135.serialDebug(false);


  
  lcd.begin();
  lcd.setBacklight(255);
  lcd.setCursor(00,00);
  lcd.print("Kendali Kualitas");
  lcd.setCursor(00,1);
  lcd.print("     Udara ");
  delay(1000);
  lcd.clear();
  lcd.setCursor(00,00);
  lcd.print("Zulfikar Bella A");
  lcd.setCursor(00,1);
  lcd.print("Rosnabila Enggar");
  delay(2000);
  lcd.clear();
  lcd.createChar(1,pic_termo);
  lcd.createChar(2,pic_humd);
  lcd.createChar(3,lonceng);
  lcd.createChar(4,lonceng1);
}

void loop() {
  delay(1000);
  MQ135.update(); // Update data, the arduino will read the voltage from the analog pin
  float Udara = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  Serial.print(jenisgas[CO]);Serial.print (" : ");Serial.print(Udara);Serial.println(" ppm");
  MQ135.serialDebug(); // Will print the table on the serial port
  MQ135.init(); 

  digitalWrite(pinLED,LOW); // Turn On the LED
  delayMicroseconds(samplingTime);
  
  float voMeasured = analogRead(pinGP2Y); // Read the dust value
  delayMicroseconds(deltaTime);
  
  digitalWrite(pinLED,HIGH); // Turn Off the LED
  delayMicroseconds(sleepTime);
  
  float calcVoltage = voMeasured * (3.3/4096);
  // linear equation taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  float dustDensity = (0.17 * calcVoltage - 0.1)*1000;
  if (dustDensity < 0){
    dustDensity = 0;
  }
  Serial.print("Raw signal value (0-4045): "); Serial.print(voMeasured);
  Serial.print(" - Voltage: "); Serial.print(calcVoltage);
  Serial.print(" - Density : "); Serial.println(dustDensity);



  lcd.setCursor (00,00);
  lcd.print("UDARA  &  DEBU");
  lcd.setCursor (00,1);
  lcd.write(1);
  lcd.setCursor (1,1);
  lcd.print(Udara);

  lcd.setCursor (5,1);
  lcd.print("       ");
  lcd.setCursor (14,1);
  lcd.print("  ");
  
  lcd.setCursor (9,1);
  lcd.write(2);
  lcd.setCursor (10,1);
  lcd.print(dustDensity);

  if (Udara >= 101 ){
    lcd.clear();
    while(1){
      // Menghitung Level AQI berdasarkan nilai ppm CO2
      MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
      MQ135.setA(605.18); MQ135.setB(-3.937);
      MQ135.update(); // Update data, the arduino will read the voltage from the analog pin
  float DataUdara = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

      digitalWrite(relaypin, HIGH); 
      delay(3000);
      lcd.setCursor (00,00);
      lcd.write(1);
      lcd.print(" Tidak Sehat");
      
      lcd.setCursor (1,1);
      lcd.print(DataUdara);
      lcd.setCursor (5,1);
      lcd.print("      ");
      lcd.setCursor (9,1);
      lcd.print("FAN ON");
      
      lcd.setCursor (8,1);
      lcd.write(3);
      delay(2000);
      lcd.setCursor (8,1);
      lcd.write(4);
      delay(2000);
 
      
      if (DataUdara <= 100) {
        digitalWrite(relaypin, LOW); 
        lcd.clear();
        break;
        }
    }
  }

  if (dustDensity >= 101 ){
    lcd.clear();
    while(1){
digitalWrite(relaypin, HIGH);
  digitalWrite(pinLED,LOW); // Turn On the LED
  delayMicroseconds(280);
  
  float voMeasured = analogRead(pinGP2Y); // Read the dust value
  delayMicroseconds(40);
  
  digitalWrite(pinLED,HIGH); // Turn Off the LED
  delayMicroseconds(9680);
  
  float calcVoltage = voMeasured * (3.3/4096);
  // linear equation taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  float dustDensity = (0.17 * calcVoltage - 0.1)*1000;
  if (dustDensity < 0){
    dustDensity = 0;
  }
      
      lcd.setCursor (00,00);
      lcd.write(2);
      lcd.print(" Tidak Sehat");
      lcd.setCursor (00,1);
      
      lcd.setCursor (1,1);
      lcd.print(dustDensity);
      lcd.setCursor (5,1);
      lcd.print("      ");
      lcd.setCursor (9,1);
      lcd.print("FAN ON");
      
      lcd.setCursor (8,1);
      lcd.write(3);

      lcd.setCursor (8,1);
      lcd.write(4);
      delay(2000);
 
       
      
      if (dustDensity <= 100) {
        digitalWrite(relaypin, LOW); 
        lcd.clear();
        break;
        }
    }
  }

}
