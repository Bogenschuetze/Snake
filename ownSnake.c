/*
 * ownSnake.c
 *
 * Created: 23.05.2014 08:27:26
 *  Author: Meyerr
 */ 

//LED-Matrix mit Beschleunigungssensor
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Transistoren Spalten[20] PB0,PB1,PB3,PB4,PB5,PB6,PB7,PC2,PC3,PC4,PC5,PC6,PC7,PD0,PD1,PD2,PD4,PD5,PD6,PD7
//Transistoren Zeilen [10] Pin15=QA,Pin1=QB,Pin2=QC,Pin3=QD,Pin4=QE,Pin5=QF,Pin6=QG,Pin7=QH,PA3,PA4
//Schieberegister: 14-PA0,11-PA1,12-PA2
//Sensor: PA5,PB2,PC0,PC1,PD3
//Taster: PA6,PA7

const int spalten_anzahl = 10;
const int zeilen_anzahl = 10;
volatile uint8_t spalte[21] = {0,PB0,PB1,PB3,PB4,PB5,PB6,PB7,PD4,PD5,PD3,PD2,PD1,PD0,PD7,PC2,PC3,PC4,PC5,PC6,PC7};//Displayspeicher
volatile uint8_t zeile[11]  = {0,PA3,PA4,7,6,5,4,3,2,1,15};
void init();
void zeile_an(uint8_t x);
void zeile_an2(uint8_t x);
void spalte_an2(uint8_t x);
void set_pixel(uint8_t x,uint8_t y);
void schiebereset();
void clear_pixel(uint8_t x,uint8_t y);
int main (void)
{
	init();
	uint8_t z = 1;
	uint8_t s = 1;
	uint32_t geschwindigkeit = 45; 
while (1)
{			
	for(z=1;z<11;z++)
	{	
		for(s=1;s<21;s=s+2)
		{
			set_pixel(s,z);
			_delay_ms(geschwindigkeit);
			clear_pixel(s,z);
	}}
}
}

void init()
{

	DDRD |= 0b11111111;
	DDRB |= 0b00001011;
	DDRA |= 0b00000111;
	DDRC |= 0b11111100;
}
void set_pixel(uint8_t x,uint8_t y)
{
	switch (x) {
		case 1 : PORTB |= (1<<PB0); break;
		case 2 : PORTB |= (1<<PB1); break;
		case 3 : PORTB |= (1<<PB3); break;
		case 4 : PORTB |= (1<<PB4); break;
		case 5 : PORTB |= (1<<PB5); break;
		case 6 : PORTB |= (1<<PB6); break;
		case 7 : PORTB |= (1<<PB7); break;
		case 8 : PORTD |= (1<<PD4); break;
		case 9 : PORTD |= (1<<PD5); break;
		case 10 : PORTD |= (1<<PD3); break;
		case 11 : PORTD |= (1<<PD2); break;
		case 12 : PORTD |= (1<<PD1); break;
		case 13 : PORTD |= (1<<PD0); break;
		case 14 : PORTD |= (1<<PD7); break;
		case 15 : PORTC |= (1<<PC2); break;
		case 16 : PORTC |= (1<<PC3); break;
		case 17 : PORTC |= (1<<PC4); break;
		case 18 : PORTC |= (1<<PC5); break;
		case 19 : PORTC |= (1<<PC6); break;
		case 20 : PORTC |= (1<<PC7); break;
	}
	switch (y) {
		case 1 : zeile_an(1); break;
		case 2	: zeile_an(2); break;
		case 3	: zeile_an(3); break;
		case 4	: zeile_an(4); break;
		case 5  : zeile_an(5); break;
		case 6  : zeile_an(6); break;
		case 7  : zeile_an(7); break;
		case 8  : zeile_an(8); break;
		case 9	: PORTA |= (1<<PA4); break;
		case 10	: PORTA |= (1<<PA3); break;
	}
}
void clear_pixel(uint8_t x,uint8_t y) //Beta ! mit Vorsicht zu genießen :D
{
	switch (x) {
		case 1 : PORTB &=~ (1<<PB0); break;
		case 2 : PORTB &=~ (1<<PB1); break;
		case 3 : PORTB &=~ (1<<PB3); break;
		case 4 : PORTB &=~ (1<<PB4); break;
		case 5 : PORTB &=~ (1<<PB5); break;
		case 6 : PORTB &=~ (1<<PB6); break;
		case 7 : PORTB &=~ (1<<PB7); break;
		case 8 : PORTD &=~ (1<<PD4); break;
		case 9 : PORTD &=~ (1<<PD5); break;
		case 10 : PORTD &=~ (1<<PD3); break;
		case 11 : PORTD &=~ (1<<PD2); break;
		case 12 : PORTD &=~ (1<<PD1); break;
		case 13 : PORTD &=~ (1<<PD0); break;
		case 14 : PORTD &=~ (1<<PD7); break;
		case 15 : PORTC &=~ (1<<PC2); break;
		case 16 : PORTC &=~ (1<<PC3); break;
		case 17 : PORTC &=~ (1<<PC4); break;
		case 18 : PORTC &=~ (1<<PC5); break;
		case 19 : PORTC &=~ (1<<PC6); break;
		case 20 : PORTC &=~ (1<<PC7); break;
	}
	switch (y) {
		case 1 : schiebereset(); break;
		case 2	: schiebereset(); break;
		case 3	: schiebereset(); break;
		case 4	: schiebereset(); break;
		case 5  : schiebereset(); break;
		case 6  : schiebereset(); break;
		case 7  : schiebereset(); break;
		case 8  : schiebereset(); break;
		case 9	: PORTA &=~ (1<<PA4); break;
		case 10	: PORTA &=~ (1<<PA3); break;
	}
}
void zeile_an(uint8_t x)
{
	uint8_t i = 0;
		PORTA |= (1<<PA0);
		
		PORTA &=~ (1<<PA1);								//0-1 Flanke um Shift hervorzurufen
		PORTA |= (1<<PA1);
		PORTA &=~ (1<<PA1);
		for (i=1;i<x;i++)
		{		//0-1 Flanke um Shift hervorzurufen
				PORTA &=~ (1<<PA0);
				PORTA |= (1<<PA1);
				PORTA &=~ (1<<PA1);
		}
		PORTA &=~ (1<<PA2);								//0-1 Flanke um Schieberegister in Ausgangsregister
		PORTA |= (1<<PA2);
		PORTA &=~ (1<<PA2);
}
void zeile_an2(uint8_t x)
{
		switch (x) {
			case 1 : zeile_an(1); break;
			case 2	: zeile_an(2); break;
			case 3	: zeile_an(3); break;
			case 4	: zeile_an(4); break;
			case 5  : zeile_an(5); break;
			case 6  : zeile_an(6); break;
			case 7  : zeile_an(7); break;
			case 8  : zeile_an(8); break;
			case 9	: PORTA |= (1<<PA4); break;
			case 10	: PORTA |= (1<<PA3); break;
		}
}
void spalte_an2(uint8_t x)
{
		switch (x) {
			case 1 : PORTB |= (1<<PB0); break;
			case 2 : PORTB |= (1<<PB1); break;
			case 3 : PORTB |= (1<<PB3); break;
			case 4 : PORTB |= (1<<PB4); break;
			case 5 : PORTB |= (1<<PB5); break;
			case 6 : PORTB |= (1<<PB6); break;
			case 7 : PORTB |= (1<<PB7); break;
			case 8 : PORTD |= (1<<PD4); break;
			case 9 : PORTD |= (1<<PD5); break;
			case 10 : PORTD |= (1<<PD3); break;
			case 11 : PORTD |= (1<<PD2); break;
			case 12 : PORTD |= (1<<PD1); break;
			case 13 : PORTD |= (1<<PD0); break;
			case 14 : PORTD |= (1<<PD7); break;
			case 15 : PORTC |= (1<<PC2); break;
			case 16 : PORTC |= (1<<PC3); break;
			case 17 : PORTC |= (1<<PC4); break;
			case 18 : PORTC |= (1<<PC5); break;
			case 19 : PORTC |= (1<<PC6); break;
			case 20 : PORTC |= (1<<PC7); break;
		}
}
void schiebereset()
{
	uint8_t i = 0;
	PORTA &=~ (1<<PA0);
	for (i=1;i<10;i++)
	{		//0-1-0 Flanke um Shift hervorzurufen
		//PORTA &=~ (1<<PA0);
		PORTA &=~ (1<<PA1);
		PORTA |= (1<<PA1);
		PORTA &=~ (1<<PA1);
	}
		PORTA &=~ (1<<PA2);								//0-1 Flanke um Schieberegister in Ausgangsregister
		PORTA |= (1<<PA2);
		PORTA &=~ (1<<PA2);	
}




