/*
 * Oblig1.c
 *
 * Created: 16.02.2017 14:10:04
 * Author : fsa011
 */ 

#include <avr/io.h>
#define F_CPU 3686400
#include <avr/interrupt.h>
#include <math.h>

volatile unsigned char running;
volatile unsigned int ov_counter,ending_edge, speed;	
volatile unsigned long clocks, time_finish;
volatile unsigned int distance = 100; 
unsigned char displayScreen(unsigned int speed);

ISR(INT0_vect)
{
	running = 1;
	speed = 0;
}

ISR(TIMER1_OVF_vect)
{
	ov_counter++;
}

ISR(TIMER1_CAPT_vect)
{
	if(running == 1){
		ov_counter = 0;
		TCNT1 = 0;
		running = 2;
	
	}
	else if(running == 2)
	{

		ending_edge = ICR1L;
		ending_edge += (ICR1H * 256);
		clocks = (unsigned long)ending_edge
				+((unsigned long)ov_counter*65536);
        // <-- (Klokkehastighet / prescaler) 3686400 / 8 (s*1000)
		time_finish = clocks/460; 
		//Gir hastighet * 10, for å slippe desimaler. //Får dermed m/s.
        speed =((unsigned long)distance*10/((unsigned long)time_finish));
		running = 0;

	}
}

int main(void)
{
	DDRC = 0xff;
	DDRB = 0xff;
	GICR = 1<<INT0;
	MCUCR = (1<<ISC01)|(1<<ISC00);
	TCCR1B = (1<<ICES1)|(1<<CS11);
	TIMSK = (1<<TICIE1)|(1<<TOIE1);
	sei();

    while (1) 
    {
        //Venstre skjerm
		PORTB = displayScreen(speed/10)|1;
        //Høyre skjerm 
		PORTC = displayScreen(speed%10);   
	}
}

unsigned char displayScreen(unsigned int speed)
{
	unsigned char output;
	switch(speed)
	{
		case 0:
			output = 0b11101110;
			break;
		case 1:
			output = 0b00100100;
			break;
		case 2:
			output = 0b10111010;
			break;
		case 3: 
			output = 0b10110110;
			break;
		case 4:
			output = 0b01110100;
			break;
		case 5:
			output = 0b11010110;
			break;
		case 6:
			output = 0b01011110;
			break;
		case 7:
			output = 0b11100100;
			break;
		case 8:
			output = 0xfe;
			break;
		case 9:
			output = 0b11101110;
			break;
		default:
			output = 0xff;
			break;
	}
	return output; 
}

