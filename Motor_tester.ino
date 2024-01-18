/*
/           Motor Tester Arduino
/
*/
 
/*******************************************************************
/*********************** Macros ************************************
/*******************************************************************
*/
#define VOLTAGE_SENSOR_PIN        A0  
#define CURRENT_SENSOR_PIN        A1  
#define TEMP_AMBIENT_SENSOR_PIN   A2  
#define TEMP_MOTOR_SENSOR_PIN     A3  
#define RELAY_UP_PIN              8  
#define RELAY_DOWN_PIN            9  
 
#define ADC_RESOLUTION            1024.0
#define ADC_VREF_mV               5000.0
 
#define R1                      30000.0
#define R2                      7500.0
#define VOLTAGE_DIVIDER         (1 / (R2 / (R1 + R2)))
 
#define NO_OF_MEASURE           10   //define number of measures
#define MEASURE_DELAY           100  //measure delay in ms
 
/*******************************************************************
/*********************** Function definitions **********************
/*******************************************************************
*/
void ReadAndPrintValues (void);
 

/*******************************************************************
/*********************** Variables *********************************
/*******************************************************************
*/
// Integer for ADC value
uint16_t adc_value = 0;
 
uint8_t cycles = 0;
 
// Floats for ADC voltage & Input voltage
double adc_voltage = 0;
double voltage = 0;
double current = 0;
 
// Float for Reference Voltage
double ref_voltage = 5.0;
 
//Current sensor macros and viables
#define mVperAmp                66.0 // use 100 for 20A Module and 66 for 30A Module
#define ACSoffset               2500.0
 
double current_sensor_voltage = 0;
double temp_sensor_miliVolt   = 0;
double ambient_temp = 0;
double motor_temp = 0;
 
/*******************************************************************
/*********************** Setup *************************************
/*******************************************************************
*/
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(VOLTAGE_SENSOR_PIN, INPUT);
  pinMode(CURRENT_SENSOR_PIN, INPUT);
  pinMode(TEMP_AMBIENT_SENSOR_PIN, INPUT);
  pinMode(TEMP_MOTOR_SENSOR_PIN, INPUT);
  pinMode(RELAY_UP_PIN, OUTPUT);
  pinMode(RELAY_DOWN_PIN, OUTPUT);
}
/*******************************************************************
/*********************** Loop **************************************
/*******************************************************************
*/
void loop() {
  Serial.println("Program do testowania podnosnika szyby");
  Serial.println("Wprowadz liczbe cykli:");
  while (Serial.available() == 0) {}     //wait for data available
  String teststr = Serial.readString();  //read until timeout
  teststr.trim();                        // remove any \r \n whitespace at the end of the String
 
  cycles = teststr.toInt();
  Serial.print("Wprowadzona liczba cykli: ");
  Serial.print(cycles);
  Serial.print("\n****************************************************\n");
 
  for(int i = 0; i < cycles; i++)
  {
    digitalWrite(RELAY_UP_PIN, HIGH);
 
    for(int j = 0; j < NO_OF_MEASURE; j++)
    {
      ReadAndPrintValues();
      delay(MEASURE_DELAY);
    }
    digitalWrite(RELAY_UP_PIN, LOW);
 
    delay(100);
 
    digitalWrite(RELAY_DOWN_PIN, HIGH);
 
    for(int j = 0; j < NO_OF_MEASURE; j++)
    {
      ReadAndPrintValues();
    }
 
    digitalWrite(RELAY_DOWN_PIN, LOW);
  }
 
  //ReadAndPrintValues();
 
  delay(2000);
  }
 

/*******************************************************************
/*********************** Functions *********************************
/*******************************************************************
*/
void ReadAndPrintValues (void){
 
  //Ambient temperature sensor
  adc_value = analogRead(TEMP_AMBIENT_SENSOR_PIN);
  delay(10);
  adc_value = analogRead(TEMP_AMBIENT_SENSOR_PIN);
  delay(10);
  ambient_temp = (adc_value * ref_voltage) / ADC_RESOLUTION * 100;
 
  Serial.print("Ambient temperature = ");
  Serial.print(ambient_temp,1);
  Serial.print(" °C\n");
 
  //Motor temperature sensor
  adc_value = analogRead(TEMP_MOTOR_SENSOR_PIN);
  delay(10);
  adc_value = analogRead(TEMP_MOTOR_SENSOR_PIN);
  delay(10);
  motor_temp = (adc_value * ref_voltage) / ADC_RESOLUTION * 100;
 
  Serial.print("Motor temperature = ");
  Serial.print(motor_temp,1);
  Serial.print(" °C\n");
 
 
  //Voltage sensor
  adc_value = analogRead(VOLTAGE_SENSOR_PIN);
  delay(10);
  adc_value = analogRead(VOLTAGE_SENSOR_PIN);
  adc_voltage  = (adc_value * ref_voltage) / ADC_RESOLUTION;
  delay(10);
  voltage = adc_voltage * VOLTAGE_DIVIDER ;
 
  // print the results to the Serial Monitor:
  Serial.print("voltage = ");
  Serial.print(voltage,1);
  Serial.print(" V\n");
 

  //Current sensor
  adc_value = analogRead(CURRENT_SENSOR_PIN);
  delay(10);
  adc_value = analogRead(CURRENT_SENSOR_PIN);
  delay(10);
  current_sensor_voltage = (adc_value / ADC_RESOLUTION) * ADC_VREF_mV; // Gets you mV
  current = ((current_sensor_voltage - ACSoffset) / mVperAmp);
 
  Serial.print("current = ");
  Serial.print(current,1);
  Serial.print(" A\n");
 
  Serial.println("-------------------------------------");
}