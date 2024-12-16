#define outputPin1 3
#define outputPin2 4
#define outputPin3 5
#define outputPin4 6

volatile int chanel = 0;
volatile int chanelPin[10] = {0};
volatile int chanelDutycycle[10] = {0};

String serialLoad;
int dutycycle = 0;

void setup() {
  // put your setup code here, to run once:
  sei(); // turn on interrupts
  Timer1Init(); // turn on timer1 for output
  AttachChanel(0,outputPin1);//chanel 0 for outputPin1
  AttachChanel(1,outputPin2);//chanel 1 for outputPin2
  AttachChanel(2,outputPin3);//chanel 2 for outputPin3
  AttachChanel(3,outputPin4);//chanel 3 for outputPin4
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    serialLoad = Serial.readStringUntil('\n');
    dutycycle = serialLoad.toInt();
    chanelDutycycle[0] = dutycycle;
    chanelDutycycle[1] = dutycycle;
    chanelDutycycle[2] = dutycycle;
    chanelDutycycle[3] = dutycycle;
  }
}

void Timer1Init(){
  // Reset timer1;
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= 1<<CS11; // prescaler = clk/8
  TIMSK1 |= (1<<TOIE1)||(1<<OCIE1A); // enable timer1 interrupt, OCR1A compare match
  TCNT1 = 65534; // timer1 interrupt
  OCR1A = 1;
}

void AttachChanel(int chanel,int pin){
  //PORTD only
  chanelPin[chanel] = pin;
  PORTD |= (1<<pin); // set pin is output;
  DDRD &= ~(1<<pin); // write pin: LOW;
}

ISR(TIMER1_OVF_vect){
  TCNT1 = 61535; //interrupt 500Hz

  //reset or next to chanel 
  if(chanel == 9){
    chanel = 0; 
  }
  else{
    chanel++; 
  }

  if(chanelPin[chanel]!=0){
    DDRD |= (1<<chanelPin[chanel]); // chanel pin is HIGH
    OCR1A = TCNT1 + 2000 + chanelDutycycle[chanel]*20; // time to chanel pin LOW
  }
  else{
    OCR1A = 1; // no compare
  }

}

ISR(TIMER1_COMPA_vect){
  DDRD &= ~(1<<chanelPin[chanel]); //chanel pin is LOW
}
