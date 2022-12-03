#include <Arduino.h>

// declaration of motors pins
#define PWMA 16
#define IN1 17
#define IN2 5
#define IN3 18
#define IN4 19
#define PWMB 21

const int freq = 30000;
const int pwmChannelA = 0;
const int pwmChannelB = 1;
// Setting the resolution to 8Bits, gives you a duty cycle range [0 – 255]. While setting it to 10Bits, gives you a range of [ 0 – 1023 ]. And so on!
const int resolution = 8;
int dutyCycle = 200;


#define button1 2
#define button2 22
#define button3 23
#define buzzer 27


#define numberOfSamples 100

int velA, velB, vel;

// declaration of ir sensor pins
#define IR_SENSORS_NUMBER 8
#define limSup 1000
int ir_sensors_pins[IR_SENSORS_NUMBER] = {36,39,34,35,32,33,25,26};
unsigned long ir_values[IR_SENSORS_NUMBER];
unsigned long ir_min_value[IR_SENSORS_NUMBER] = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095};
unsigned long ir_max_value[IR_SENSORS_NUMBER] = {0, 0, 0, 0, 0, 0, 0, 0};

unsigned long positio, lastPos;
int alvo = 3600;
#define IR_THRESHOLD 850

int error, lastError = 0;
float corrigir;

int  velocidadeBase = 200;
int velocidadeInicial = velocidadeBase;
int vMax = 220;
int vMin = -120;
int velocidadeE, velocidadeD;

double Kp = 0.060;
float Ki = 0;
float Kd = 0.030;

int timer1 = 0;
int timer2 = 0;
int Ta = 1;

void buzz(int time){
  digitalWrite(buzzer, HIGH);
  delay(time);
  digitalWrite(buzzer, LOW);
}

void readSensor(unsigned long * destination){
  // Serial.print("ReadSensor: ");
  for(int i = 0; i < IR_SENSORS_NUMBER; i++){
    // Serial.print(analogRead(ir_sensors_pins[i]));
    // Serial.print("\t");
    if(ir_min_value[i] < ir_max_value[i]){
      destination[i] = map(analogRead(ir_sensors_pins[i]), ir_min_value[i], ir_max_value[i], 0, limSup);
    }
    if(ir_min_value[i] > ir_max_value[i]) {
      destination[i] = map(analogRead(ir_sensors_pins[i]), ir_max_value[i], ir_min_value[i], 0, limSup);
    }
    destination[i] = constrain(destination[i], 0, limSup);
  }
  // Serial.println("");
}

void printSensor(unsigned long * array){
  for(int i = 0; i < IR_SENSORS_NUMBER; i++){
    Serial.print("|  ");
    Serial.print(array[i]);
    Serial.print("\t");

  }
  Serial.println(' ');
}

void keepMins(unsigned long * array1, unsigned long * array2, int length){
  for(int i = 0; i < length; i++){
    if(array2[i] < array1[i]){
      array1[i] = array2[i];
    }
  }
}

void keepMax(unsigned long * array1, unsigned long * array2, int length){
  for(int i = 0; i < length; i++){
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
  unsigned long candidate[IR_SENSORS_NUMBER];
  // readSensor(ir_min_value); 
  delay(1000);

  for(int j = 0; j < numberOfSamples; j++){
    for(int i = 0; i < IR_SENSORS_NUMBER; i++){
      ir_min_value[i] += analogRead(ir_sensors_pins[i]);
      candidate[i] = analogRead(ir_sensors_pins[i]);

    }
    printSensor(candidate);
    // readSensor(candidate);
    //keepMins(ir_min_value, candidate, IR_SENSORS_NUMBER);
    delay(1);
  }
  for(int i = 0; i < IR_SENSORS_NUMBER; i++){
      ir_min_value[i] /= numberOfSamples;

  }

  

  buzz(200);

  Serial.println("Aguardando o botão de calibrar...");
  while(!digitalRead(button1)){} //wait for the button1 to be pressed
  //read white values
  // readSensor(ir_max_value); 
  delay(1000);
  
  for(int j = 0; j < numberOfSamples; j++){
    for(int i = 0; i < IR_SENSORS_NUMBER; i++){
      candidate[i] = analogRead(ir_sensors_pins[i]);
      ir_max_value[i] += analogRead(ir_sensors_pins[i]);

    }
    printSensor(candidate);
    // readSensor(candidate);
    // keepMax(ir_max_value, candidate, IR_SENSORS_NUMBER);
    delay(1);
  }

  for(int i = 0; i < IR_SENSORS_NUMBER; i++){
    ir_max_value[i] /= numberOfSamples;
  }
  Serial.println("Max value: ");
  printSensor(ir_max_value);

  Serial.println("Apertou o botão dnv");

  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);

}

unsigned long calculatePosition(unsigned long * array, unsigned long length){
  unsigned long num = 0, den = 0, calculatedPos = 0;
  if (array[0] > IR_THRESHOLD && array[1] > IR_THRESHOLD && array[2] > IR_THRESHOLD && array[3] > IR_THRESHOLD && array[4] > IR_THRESHOLD && array[5] > IR_THRESHOLD && array[6] > IR_THRESHOLD && array[7] > IR_THRESHOLD) {
    if (lastPos > alvo) {
      calculatedPos = 7000;
      velocidadeInicial = velocidadeBase*0.9;
    } else {
      calculatedPos = 0;
      velocidadeInicial = velocidadeBase*0.9;

    }
  }
  else {
    velocidadeInicial = velocidadeBase;

    for(int i = 0; i < length; i++){
      num += 1000 * i * array[i];
      den += array[i];
    }
    calculatedPos = num * 1.0 / den;
  }
  lastPos = calculatedPos;
  return calculatedPos;
}

void move(int velA, int velB){
  if(velA == 0 && velB == 0){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    ledcWrite(pwmChannelA, velA);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
    ledcWrite(pwmChannelB, velB);
  }
  if(velA > 0 && velB > 0){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    ledcWrite(pwmChannelA, velA);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    ledcWrite(pwmChannelB, velB);
  }
  else if(velA >= 0 && velB < 0){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    ledcWrite(pwmChannelA, velA);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    ledcWrite(pwmChannelB, velB);
  }
  else if(velA < 0 && velB >= 0){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    ledcWrite(pwmChannelA, velA);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    ledcWrite(pwmChannelB, velB);
  }
  else if(velA < 0 && velB < 0){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    ledcWrite(pwmChannelA, velA);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    ledcWrite(pwmChannelB, velB);
  }
}

void testeMotor(){
  
  int vTeste = 0;
  while(!digitalRead(button2)){
    while(!digitalRead(button1)){}
    if(vTeste > 512){
      vTeste = 0;
    }
    vTeste += 32;
    move(vTeste, vTeste);
    delay(1000);
    move(0, 0);
    for(int i = 0; i < vTeste / 32; i ++){
      buzz(100);
      delay(50);
    }
  }
}

void meCorrige(){
  error = positio - alvo;
  /*
  Serial.print("\t");
  Serial.print(error);
*/
  corrigir = (error * Kp) + ((error - lastError) * Kd);
  corrigir = corrigir / 1.0;
  /*
  Serial.print("\t");
  Serial.print(corrigir);
*/
  velocidadeE = velocidadeInicial + corrigir;
  velocidadeD = velocidadeInicial - corrigir;

  if (velocidadeE > vMax)
    velocidadeE = vMax;
  if (velocidadeE < vMin)
    velocidadeE = vMin;

  if (velocidadeD > vMax)
    velocidadeD = vMax;
  if (velocidadeD < vMin)
    velocidadeD = vMin;
  /*
  Serial.print("\t");
  Serial.print(velocidadeD);
  Serial.print("\t");
  Serial.println(velocidadeE);
*/
  move(velocidadeE, velocidadeD);

  lastError = error;
}

void setup() {
  
    pinMode(PWMA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(PWMB, OUTPUT);

    // PWM Setup
    ledcSetup(pwmChannelA, freq, resolution);
    ledcSetup(pwmChannelB, freq, resolution);
    ledcAttachPin(PWMA, pwmChannelA);
    ledcAttachPin(PWMB, pwmChannelB);

    pinMode(button1, INPUT);
    pinMode(button2, INPUT);
    pinMode(button3, INPUT);
    pinMode(buzzer, OUTPUT);
    buzz(100);
    Serial.begin(115200);


    for(int i = 0; i < IR_SENSORS_NUMBER; i++){
      pinMode(ir_sensors_pins[i], INPUT);
    }
    
  


  Serial.println("Hello World");
  
  calibrateIR();
  

  Serial.print("ir_min");
  printSensor(ir_min_value);
  Serial.println("\nir_max");
  printSensor(ir_max_value);
  readSensor(ir_values);
  Serial.println("\nir_values");
  printSensor(ir_values);

  while(!digitalRead(button1)){
    readSensor(ir_values);
    //printSensor(ir_values);
    if(digitalRead(button3)){
      velocidadeBase = 245;
      velocidadeInicial = velocidadeBase;
      vMax = 255;
      Kp = 0.070;
      Ki = 0;
      Kd = 0.035;
      buzz(150);
    }
    if(digitalRead(button2)){
      velocidadeBase = 225;
      velocidadeInicial = velocidadeBase;
      vMax = 255;
      Kp = 0.062;
      Ki = 0;
      Kd = 0.035;
      buzz(250);
    }
    delay(50);
  } //wait for the button1 to be pressed
  buzz(1000);
  delay(1000);
 
}

void loop() {
  
  readSensor(ir_values);
  // Serial.print("Real");
  // for(int i = 0; i < IR_SENSORS_NUMBER; i++){
  //   Serial.print("|  ");
  //   Serial.print(analogRead(ir_sensors_pins[i]));
  //   Serial.print("\t");
  // }
  // Serial.print("Norma");
  // printSensor(ir_values);
  timer1 = millis();
  
  positio = calculatePosition(ir_values, IR_SENSORS_NUMBER);
  meCorrige();

  Serial.print("Positio: ");
  Serial.print(positio);
  Serial.print("\t Error: ");
  Serial.print(error);
  Serial.print("\tCorrigir: ");
  Serial.print(corrigir);
  Serial.print("\tvD: ");
  Serial.print(velocidadeD);
  Serial.print("\tvE: ");
  Serial.print(velocidadeE);
  Serial.println("\t");
}