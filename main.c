/*
 * EMBEDDED_EXAM.c
 *
 * Created: 10/19/2021 14:58:49 PM
 * Author : Saad El Jadouri
 
 * Using B ports and PWM
 *
 */ 

// blink 3 leds one after another in case of button do the blinking twice and in case of potentiometer light up from dark to bright or reverse4


// In our tmega328 we will use b ports and we will try to set pwm

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void IniLeds(void)
{ //leds as output
  DDRB |= (1<<DDB1) | (1<<DDB2)| (1<<DDB3); 
}

void Lightdarktobright(void)
{
  
  for(int i = 0; i < 255; i++)
  {
    OCR1A += 1;
    _delay_ms(30);
  }
  OCR1A = 0;
  PORTB &= ~(1<<PORTB1);
  
  
  for(int i = 0; i < 255; i++)
  {
    OCR1B += 1;
    _delay_ms(30);
  }
  OCR1B = 0;
  PORTB &= ~(1<<PORTB2);
  
  
  for(int i = 0; i < 255; i++)
  {
    OCR2A += 1;
    _delay_ms(30);
  }
  OCR2A = 0;
  PORTB &= ~(1<<PORTB3);
  
}


void lightbrighttodark(void)
{
	
  for(int i = 0; i < 255; i++)
  {
    OCR2A += 1;
    _delay_ms(30);
  }
  OCR2A = 0;
  PORTB &= ~(1<<PORTB3);
  
  
  for(int i = 0; i < 255; i++)
  {
    OCR1B += 1;
    _delay_ms(30);
  }
  OCR1B = 0;
  PORTB &= ~(1<<PORTB2);
      
  for(int i = 0; i < 255; i++)
  {
    OCR1A += 1;
    _delay_ms(30);
  }
  OCR1A = 0;
  PORTB &= ~(1<<PORTB1);
}

//BUTTON

void InitButton(void)
{
  //digital button 2 as input
  DDRD &= (1 << DDD2);
  PORTD |= ( 1 << PORTD2);
}

//POTENTIOMETER
void InitPotentiometer(void)
{
  // potentiometer at Analog 1
  ADMUX |= ( 1 << REFS0);
  ADCSRA |= ( 1 << ADEN) | (1 << ADPS0) | ( 1 << ADPS1);
}


//We are going to initialize a fast pwm mode


void FastPwm(void)
{
  // PIN9
  TCCR1A |= (1 << WGM10) | (1 << COM1A1);
  TCCR1B |= (1 << CS10) | (1 << WGM12);

  // PIN10
  TCCR1A |= (1<<COM1B1)| (1<<WGM10);      
  TCCR1B |= (1<<WGM12)|(1<<CS10);       
  
  // PIN11
  TCCR2A |=  (1 << COM2A1) | (1<<WGM20) ;   // Normal mode OC2A & OC2B
  TCCR2B |=  (1 << CS22) | (1 << CS20); //setting bits
  //Prescaling 1024
}


uint16_t ConversionADC(void)
{
  ADCSRA |= (1<<ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  uint16_t pt = ADC;
  return pt;
}

void Blinkleds(void)
{
  
  OCR1A = 0;
  OCR1B = 0;
  OCR2A =0;
  
  // Blinking Twice
  for (int i = 0; i < 2; i++)
  {
     OCR1A = 0xFF;
     OCR1B = 0xFF;
     OCR2A = 0xFF;
    _delay_ms(500);
	OCR1A == 0x00;
     OCR1B == 0x00;
     OCR2A == 0x00;
  }
}

void TInterrupt(void)
{
  EICRA |= (1 << ISC00);  
  EIMSK |= (1 << INT0);  
}


int main()
{
  // Button initialization
  InitButton();
  IniLeds();
  InitPotentiometer();
  FastPwm();
  sei();
  TInterrupt();
  
  while(1)
  {
    Lightdarktobright();
	
    uint16_t val =  ConversionADC(); 
	
    //potentiometer as 512 half way
	if (val > 512)
    {
      lightbrighttodark();
    }
    else
    {
      lightbrighttodark();
    }

  }
  
}
//routine
ISR (INT0_vect)
{
  Blinkleds();
}