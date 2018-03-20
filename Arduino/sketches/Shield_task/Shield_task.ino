int analog_pin =3;
int timer1_counter;
int count =0;

//250khz tone
int frequency = 250;
//bool sqState =false;

void setup() {
  int timer1_counter;
  timer1_counter = 34286;
  
  Serial.begin(9600);

  DDRB = B11111101;  // sets 1 output 2 input and all else output

  
  sei();
  
  PORTB = 0x01; //turn idle LED on
  
  timer1_setup(); //setup the timer for the test
  timer2_setup(); //setup the timer for the sq wave
  
  //timer 2 setup
  pinMode(11, OUTPUT);
  pinMode(3,OUTPUT);
  TCCR2A=0;//reset the register
  TCCR2B=0;//reset the register
  TCCR2A=0b01010011;// fast pwm mode
  TCCR2B=0b00001001;// no prescaler and WGM22 is 1
  OCR2A=199;//control value from the formula
  
}

void timer1_setup(){
  noInterrupts(); 
  TCCR1A = 0;
  TCCR1B = 0;
  //OCR0A = 1;
  //TIMSK0 = 0x02; //sets timer 0 to generate interrupts
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();  
}

void timer2_setup(){
 TCCR0A = 0x23;
 TCCR0B = 0x0C;
 OCR0A = 141;
 OCR0B = 107;
 
}

void loop() {
  
  //sleep_cpu();
  //PORTB = 00000001;//(1 << PB0); //LED on
  //Serial.println((PINB&0x02)>>1);
  
  if ((PINB&0x02)>>1){
    PORTB = 0x00; //turn idle LED off
    DDRD = (1<<PD5); //turn buzzer on
    timer1_setup();
    timer2_setup();
    count=0;
  }
  
  Serial.println(analogRead(analog_pin));
  
}

ISR(TIMER1_OVF_vect) { //test mode timer overflow
  
  if (count==1){ //was 9
    PORTB = 0x01; //turn idle LED on  
    DDRD = 0; //turn buzzer off
  }
  count++;
}
/*
ISR(TIMER2_OVF_vect) { //square wave timer overflow

  Serial.println("sq");
  
  if((PORTB&0x03)>>2){
    PORTB = (PORTB&0x03); //hi
  }else{
    PORTB = (PORTB&0x03); //lo
  }
  */


