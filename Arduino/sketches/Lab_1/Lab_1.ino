#include <avr/sleep.h>

int buttons[501];
int counter = 0;
int rec = 0;

void timer0_setup(){
  TCCR0A = 0x02;
  TCCR0B = 0x05;
  OCR0A = 124;
  TIMSK0 = 0x02; //sets timer 0 to generate interrupts
}

void int0_setup() {
  
  EIMSK = 0x00; //initially sets int0 interrupt enable to low
  EICRA = 0x02; //sets interrupt flag to activate on falling edge
  
}


ISR(INT0_vect) {
  
  rec = 2;
    
}

ISR(TIMER0_COMPA_vect) {
    
  //samples the current state of the pins PB2 and PB3
  if (rec==0){
      
    if (counter<500){
      
      Serial.print(counter);
      Serial.print("   ");
      Serial.print(rec);
      Serial.print("   ");
      buttons[counter] = PINB;
      PORTB = ~(PINB <<2);
      Serial.println(buttons[counter]);
      counter++;
        
    }
    
    else {
      
      Serial.println("4 seconds reached");
      EIMSK = 0x01;
      rec = 1;
         
    }
      
  }
    
  else if(rec == 2) {
      
    if (counter<1000){
        
      Serial.print(counter-500);
      Serial.print("   ");
      Serial.print(rec);
      Serial.print("   ");
      PORTB = ~(buttons[counter-500]<<2);
      Serial.println(buttons[counter-500]);
      counter++;
      
    }

    if (counter == 1000){
      counter = 500;
      rec = 3;
    }
      
  }
    
}

void setup() {

  DDRD = 0x00;    // set int0 as input
  DDRB = 0x30;    // set PB2, PB3 as inputs and set PB4 & PB5 as output
  Serial.begin(9600);
  timer0_setup();
  int0_setup();
  sei();
  
}

void loop() {
  
  sleep_cpu();
  
}
