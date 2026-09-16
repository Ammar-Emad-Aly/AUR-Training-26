#include <avr/io.h>
#include <avr/interrupt.h>

ISR(INT0_vect)
{
    PORTB ^= (1 << PB0);
}

int main()
{
    DDRB |= (1 << PB0);
    PORTB &= ~(1 << PB0);

    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);

    MCUCR |= (1 << ISC01);
    MCUCR &= ~(1 << ISC00);

    GICR |= (1 << INT0);

    sei();

    while(1)
    {
    }
}