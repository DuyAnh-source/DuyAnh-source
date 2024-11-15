// **Author**  **Nguyen Duy Anh***

const uint8_t A = 2; // A encoder
const uint8_t B = 3; //B encoder
const uint8_t PWM_out1 = 5; //  PWM+
const uint8_t PWM_out2 = 6; //  PWM-
const unsigned long resolution = 334; // resolution

const unsigned long x = 120000000/resolution;

bool current_A;
bool current_B;
bool past_A;
bool past_B;

unsigned long timer1 = micros();
unsigned long timer2 = micros();
uint8_t f_PWM = 255;
unsigned long pulse_width; // us

bool Direction;//1 is clockwise; 0 is counter-clockwise; default = 1;
String Dir;
unsigned long Motor_speed; // (rpm)
long Position;
long Last_Position;
long Pos_req;


void setup() {
  pinMode(A,INPUT_PULLUP);
  pinMode(B,INPUT_PULLUP);
  pinMode(PWM_out1,OUTPUT);
  pinMode(PWM_out2,OUTPUT);
  past_A = digitalRead(A);
  past_B = digitalRead(B);
  Position = 0;
  Last_Position = Position;
  Direction = 1;
  Serial.begin(9600);
  Pos_req = 1000;
}


void print_speed(){

 // if(Direction) Dir = "clockwise";
 // else Dir = "counter-clockwise";
 // Serial.println("Speed: " + (String)Motor_speed+ " Direction:  " + Dir + " Position: " + (String)Position);
 // Serial.println("Speed: " + (String)Motor_speed);
 Serial.println((String)Position);
}
void cycle(){
  current_A = digitalRead(A);
  current_B = digitalRead(B);
  if((current_A != past_A)){
    //pulse_width =(unsigned long)(micros()- timer1);
    //timer1 = micros();
   // Motor_speed = x/pulse_width; // fomula
    if(current_B != current_A ){ 
      Position++;     //clockwise
      //Direction = 1;
    }
    else{
      Position--;    //counter-clockwise
      //Direction = 0;
    }
    past_A = current_A;
  }
  if((current_B != past_B)){
    //pulse_width =(unsigned long)(micros()- timer1);
    //timer1 = micros();
    //Motor_speed = x/pulse_width; // fomula
    if(current_A == current_B ){ 
      Position++;     //clockwise
      //Direction = 1;
    }
    else{
      Position--;    //counter-clockwise
      //Direction = 0;
    }
    past_B = current_B;
  }
}
void cycle1(){
  current_A = digitalRead(A);
  if((current_A != past_A)){
    if(current_A == 1){
      pulse_width =(unsigned long)(micros()- timer1);
      timer1 = micros();
      Motor_speed = x/pulse_width; // fomula
      //Serial.println("Speed: " + (String)Motor_speed);
    }
    past_A = current_A;
  }
  if((unsigned long)(micros()- timer1)>x){
    Motor_speed = 0;
  }
}

void PWM(){
  if(Pos_req < (Position-10)){
    analogWrite(PWM_out2,f_PWM);
    analogWrite(PWM_out1,0);
  }
  else if(Pos_req>(Position+10)){
    analogWrite(PWM_out1,f_PWM);
    analogWrite(PWM_out2,0);
  }
  else{
    analogWrite(PWM_out2,0);
    analogWrite(PWM_out1,0);  }
}

void loop() {
  cycle();
  if(Last_Position != Position){
     Serial.println((String)Position);
     Last_Position = Position;
  }
}
