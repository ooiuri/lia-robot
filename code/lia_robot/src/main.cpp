#include <Arduino.h>

// declaration of motors pins
#define PWMA 1
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define PWMB 6

#define button1 2
#define button2 22
#define button3 23
#define buzzer 34


#define numberOfSamples 10

int velA, velB, vel;

// declaration of ir sensor pins
#define IR_SENSORS_NUMBER 8
int ir_sensors_pins[IR_SENSORS_NUMBER] = {32, 33,25, 26, 27, 14, 12, 13};
int ir_values[IR_SENSORS_NUMBER];
int ir_min_value[IR_SENSORS_NUMBER] = {0, 0, 0, 0, 0, 0, 0, 0};
int ir_max_value[IR_SENSORS_NUMBER] = {4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096};

void readSensor(int * destination){
  for(int i = 0; i < IR_SENSORS_NUMBER; i++){
    destination[i] = map(analogRead(ir_sensors_pins[i]), ir_min_value[i], ir_max_value[i], 0, 4095);
  }
}

void printSensor(int * array){
  for(int i = 0; i < IR_SENSORS_NUMBER; i++){
    Serial.print("|\t");
    Serial.print(array[i]);
    Serial.print("\t");

  }
  Serial.println(' ');
}

void keepMins(int * array1, int * array2, int lenght){
  for(int i = 0; i < lenght; i++){
    if(array2[i] < array1[i]){
      array1[i] = array2[i];
    }
  }
}

void keepMax(int * array1, int * array2, int lenght){
  for(int i = 0; i < lenght; i++){
    if(array2[i]> array1[i]){
      array1[i] = array2[i];
    }
  }
}

void calibrateIR(){
  Serial.println("Aguardando o botão de calibrar...");
  //wait for the button1 to be pressed
  while(!digitalRead(button1)){} 
  Serial.println("Apertou o botão");

  //read dark values
  int candidate[IR_SENSORS_NUMBER];
  readSensor(ir_min_value); 
  for(int j = 0; j < numberOfSamples; j++){
    readSensor(candidate);
    keepMins(ir_min_value, candidate, IR_SENSORS_NUMBER);
  }

  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);

  Serial.println("Aguardando o botão de calibrar...");
  while(!digitalRead(button1)){} //wait for the button1 to be pressed
  //read white values

  readSensor(ir_min_value); 
  for(int j = 0; j < numberOfSamples; j++){
    readSensor(candidate);
    keepMax(ir_min_value, candidate, IR_SENSORS_NUMBER);
  }
  Serial.println("Apertou o botão dnv");

  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);

}


void setup() {
  
  
    // pinMode(PWMA, OUTPUT);
    // pinMode(IN1, OUTPUT);
    // pinMode(IN2, OUTPUT);
    // pinMode(IN3, OUTPUT);
    // pinMode(IN4, OUTPUT);
    // pinMode(PWMB, OUTPUT);
    pinMode(button1, INPUT);
    pinMode(button2, INPUT);
    pinMode(button3, INPUT);
    pinMode(buzzer, OUTPUT);
    Serial.begin(115200);


    for(int i = 0; i < IR_SENSORS_NUMBER; i++){
      pinMode(ir_sensors_pins[i], INPUT);
      ir_max_value[i] = 4095;
      ir_min_value[i] = 0;
    }
    
  


  Serial.println("Hello World");
  calibrateIR();
}

void loop() {
  delay(100);
  
  readSensor(ir_values);
  //mapSensor(ir_values, ir_min_value, ir_max_value);
  printSensor(ir_values);
  delay(100);
  // put your main code here, to run repeatedly:
}



