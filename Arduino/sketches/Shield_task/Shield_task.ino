int analog_pin =3;
int timer1_counter;
int count =0;

enum {IDLE, TEST, ALERT, ALARM} state; 

void setup() {
  timer1_counter = 34286;
  
  Serial.begin(9600);

  DDRB = B11111101;  // sets 1 output 2 input and all else output

  
  sei();
  
  PORTB = 0x01; //turn idle LED on
  
  timer0_setup(); //setup the timer for the square wave
  timer1_setup(); //setup the timer for the 10 minutes
  
}

void timer0_setup(){
 TCCR0A = 0x23;
 TCCR0B = 0x0C;
 OCR0A = 0x7B;
 OCR0B = 107;
 
}

void timer1_setup(){
  noInterrupts(); 
  TCCR1A = 0;
  TCCR1B = 0;
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();  
}

void loop() {
  
  switch(state){

    case IDLE:
      PORTB = 0x01; //turn idle LED on
      DDRD = (0<<PD5); //turn buzzer off
      
      count=0;

      if(analogRead(analog_pin)>400){
    
        state = ALERT;
        }
      
      if ((PINB&0x02)>>1){
        state = TEST;
         }
         
    break;

    case TEST:
        PORTB = 0x00; //turn idle LED off
        DDRD = (1<<PD5); //turn buzzer on

        if (count ==11){
          state = IDLE;
        }

    break;

    case ALERT:

      if(analogRead(analog_pin)<400){

        state = IDLE;
      }

      if (count >=5){
          state = ALARM;
          count =0;
        }
      
    break;

    case ALARM:
      PORTB = 0x00; //turn idle LED off
      DDRD = (1<<PD5); //turn buzzer on

      if (count == 600){ //10 min timer
        state = IDLE;
      }
    break;
  }
  
  Serial.println(analogRead(analog_pin));
  
}

ISR(TIMER1_OVF_vect) { //test mode timer overflow
  //oscilate between the two frequencies
  if ((count%2) ==0){ //James bond 141, 0xF9, 0x7B

    OCR0A =0xF9;
  }else{
    OCR0A = 0x7B;
  }
  
  count++;
}

