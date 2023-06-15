// the setup function runs once when you press reset or power the board
#include "TimerOne.h"
#include <IRremote.h>

#define BEEPER 13
#define INTINPUT0 A0
#define INTINPUT1 A1
#include <Servo.h>


// piny dla sonaru (HC-SR04)
#define TRIG A2
#define ECHO A3

// pin kontroli serwo (musi być PWM)
#define SERVO 8

Servo serwo;

#include "PinChangeInterrupt.h"
#include <LiquidCrystal_I2C.h>

byte LCDAddress = 0x27;

LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

uint8_t arrowRight[8] =
{
    0b01000,
    0b01100,
    0b00110,
    0b11111,
    0b11111,
    0b00110,
    0b01100,
    0b01000
};

int argIn = 0;
volatile int cnt0, cnt1, voltage;
bool not_maks;
long int intPeriod = 500000;

int input_pin = 12; // wpisujemy nazwę pinu, po którym nastepuje komunikacja
IRrecv irrecv(input_pin);
decode_results signals;


void setup() {
  pinMode(BEEPER, OUTPUT);
  //Timer1.initialize();
  //TimerUpdate();
  pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
  pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls

  Serial.begin(9600);

  serwo.attach(SERVO);

  delay(100);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  //pinMode(3, OUTPUT);
  //pinMode(5, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  analogWrite(3, 255); //zapisuję prędkość na silniki
  analogWrite(5, 255);

  const_forward();

  cnt0=0;
  cnt1=0;
  voltage = 200;
  not_maks = true;

  Serial.setTimeout(200);

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, arrowRight);

  irrecv.enableIRIn(); // włączenie odbierania danych

 //attachPCINT(digitalPinToPCINT(INTINPUT0), increment, CHANGE);
 //attachPCINT(digitalPinToPCINT(INTINPUT1), increment, CHANGE);

}

void increment() {

  if(digitalRead(INTINPUT0))
    cnt0++;
  if(digitalRead(INTINPUT1))
    cnt1++;

  if(voltage <= 220 && (cnt0 > 100 || cnt1 > 100) && not_maks){
    cnt0 = 0;
    cnt1 = 0;
    voltage += 10;
    intPeriod -= 10000; 
    TimerUpdate();
  }
  else if (voltage >= 100 && (cnt0 > 100 || cnt1 > 100)){
    not_maks = false;
    cnt0 = 0;
    cnt1 = 0;
    voltage -= 10;
    intPeriod += 10000; 
    TimerUpdate();
    if(voltage < 100){
      not_maks = true;
    }
  }
}

// aktualizuje Timer1 aktualną wartością intPeriod
void TimerUpdate() {
  Timer1.detachInterrupt();
  Timer1.attachInterrupt(doBeep, intPeriod);
}

// zmienia wartość pinu BEEPER
void doBeep() {
  digitalWrite(BEEPER, digitalRead(BEEPER) ^ 1);
}


void forward(int time){
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(6, HIGH);  // lewy przód         
  digitalWrite(4, HIGH);   // prawy przód
  delay(time); 
  digitalWrite(6, LOW);  // lewy przód         
  digitalWrite(4, LOW);
  delay(300);
}

void const_forward(){
  analogWrite(3, 255); //zapisuję prędkość na silniki
  analogWrite(5, 255);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(6, HIGH);  // lewy przód         
  digitalWrite(4, HIGH);   // prawy przód
}


void backward(int time, int speed){
  analogWrite(3, speed);
  analogWrite(5, speed);
  digitalWrite(6, LOW); // lewy tył
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(7, HIGH);  // lewy przód         
  digitalWrite(2, HIGH);   // prawy przód
  delay(time); 
  digitalWrite(7, LOW);  // lewy przód         
  digitalWrite(2, LOW);
  delay(30);
}

void turnRight(int degree){
  analogWrite(3, 255); //zapisuję prędkość na silniki
  analogWrite(5, 255);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  int time = degree * 3.6;
  digitalWrite(6, HIGH);
  digitalWrite(2, HIGH);
  delay(time);
  digitalWrite(6, LOW);
  digitalWrite(2, LOW);
  //delay(300);
}

void turnLeft(int degree){
  analogWrite(3, 255); //zapisuję prędkość na silniki
  analogWrite(5, 255);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  int time = degree * 3.6;
  digitalWrite(7, HIGH);
  digitalWrite(4, HIGH);
  delay(time);
  digitalWrite(7, LOW);
  digitalWrite(4, LOW);
  //delay(300);
}

void reverse(int degree){
  analogWrite(3, 255); //zapisuję prędkość na silniki
  analogWrite(5, 255);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(2, HIGH);
  int time = degree * 3.6;
  delay(time);
  digitalWrite(6, LOW);
  digitalWrite(2, LOW);
}

void back_and_reverse(int degree){
  analogWrite(3, 255); //zapisuję prędkość na silniki
  analogWrite(5, 255);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);

  digitalWrite(7, HIGH);  // lewy przód         
  digitalWrite(2, HIGH);   // prawy przód
  delay(300); 
  digitalWrite(7, LOW);  // lewy przód         
  digitalWrite(2, LOW);

  digitalWrite(6, HIGH);
  digitalWrite(2, HIGH);
  int time = degree * 3.6;
  delay(time);
  digitalWrite(6, LOW);
  digitalWrite(2, LOW);
}

void printDick(int dlugosc, int grubosc){

  int czasD = dlugosc / (0.074);
  int czasG = grubosc / (0.074);
  int jaja = czasD / 3.0;
  forward(jaja);
  turnRight(200);
  forward(jaja);
  turnRight(200);
  forward(jaja);
  reverse(640);
  forward(czasD);
  turnRight(200);
  forward(czasG);
  turnRight(200);
  forward(czasD);
  reverse(640);
  forward(jaja);
  turnRight(200);
  forward(jaja);
  turnRight(200);
  forward(jaja);
  

  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  delay(6000);
}

void lcdDist(int val){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(0);

    lcd.setCursor(0,0);
    lcd.print(val);
    //barPos = map(val, 0, argIn, 0, 16);
    //lcd.setCursor(barPos, 1);
    //lcd.print('=');
    lcd.write(0);
    //delay(20);
  

}

unsigned int lookAndTellDistance(byte angle) {
  
  unsigned long tot;      // czas powrotu (time-of-travel)
  unsigned int distance;

  Serial.print("Patrzę w kącie ");
  Serial.print(angle);
  serwo.write(angle);
  
/* uruchamia sonar (puls 10 ms na `TRIGGER')
 * oczekuje na powrotny sygnał i aktualizuje
 */
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  tot = pulseIn(ECHO, HIGH);

/* prędkość dźwięku = 340m/s => 1 cm w 29 mikrosekund
 * droga tam i z powrotem, zatem:
 */
  distance = tot/58;

  Serial.print(": widzę coś w odległości ");
  Serial.println(distance);
  return distance;
}
void slowdown(int velo){
  analogWrite(3, velo); //zapisuję prędkość na silniki
  analogWrite(5, velo);

}

void check_dir(){
  int left;
  int center;
  int right;

  center = lookAndTellDistance(90);
  if (center > 30 && center <= 50){
    slowdown(130);
  }
  else{ 
  if (center <= 30){
    lcdDist(center);
    //slowdown(10);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    left = lookAndTellDistance(10);
    right = lookAndTellDistance(170);

    if(left < 30 && right < 30){
      back_and_reverse(180);
    }
    else{
    if(left<right){
      turnRight(90);
      //print right arrow
    }
    else{
      turnLeft(90);
      // print left arrow 
    }
    }
    serwo.write(90);
    const_forward();
  }
  }
}

void listen(){ // nasłuchuj sygnałów z pilota


}

int t = 0;
int oldt = 0;
bool juz_stoi = true;

// the loop function runs over and over again forever
void loop() {


  if (irrecv.decode(&signals)) {
        Serial.println(signals.decode_type); // typ nadajnika
        Serial.print(F("wynik = 0x"));
        Serial.println(signals.value, HEX); // wynik w systemie szesnatkowym
        Serial.println(signals.bits); // ilość przesłanych danych
        irrecv.blink13(true); // mrugamy diodami w arduino
        
        // Przykładowe porównanie:
        
        if (signals.value == 0x9716BE3F)
        {
          Serial.println("Wcisnieto klawisz 1");
        }
        
        // ... i tak dalej
        
        irrecv.resume(); // nasłuchujemy na następne nadanie     
    }
  

  // t = millis();




  // if(otrzymano sygnal){

  //   t = 0;
  //   oldt = 0;
  //   juz_stoi = false;
  //   last_signal = signal;

  //   if(tab[last_signal] == 0){
  //     tab[last_signal] = 1;
  //     action(sygnal);
  //   }
    


  // }
  // else{

  //     if(t >= oldt + 100 && !juz_stoi){
  //       oldt = t;
  //       stand();
  //       juz_stoi = true;
  //       tab[last_signal] = 0;
  //     }

  // }
  
  
}




