#include <avr/io.h>

int main()
{
    DDRB |= (1 << PB0);

    TCCR0 |= (1 << WGM01);
    TCCR0 |= (1 << CS01) | (1 << CS00);

    OCR0 = 124;

    while (1)
    {
        for (int i = 0; i < 500; i++)
        {
            while (!(TIFR & (1 << OCF0)));

            TIFR |= (1 << OCF0);
        }

        PORTB ^= (1 << PB0);
    }
}