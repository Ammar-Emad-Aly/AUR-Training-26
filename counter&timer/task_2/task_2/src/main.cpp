#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

int main()
{
    DDRB |= (1 << PB3);

    TCCR0 |= (1 << WGM00) | (1 << WGM01);
    TCCR0 |= (1 << COM01);
    TCCR0 |= (1 << CS01);

    // PWM frequency = 8000000 / (8 * 256) = 3906.25 Hz

    OCR0 = 64;

    while (1)
    {
        for (int i = 64; i <= 255; i += 13)
        {
            OCR0 = i;
            _delay_ms(100);
        }

        for (int i = 255; i >= 64; i -= 13)
        {
            OCR0 = i;
            _delay_ms(100);
        }
    }
}