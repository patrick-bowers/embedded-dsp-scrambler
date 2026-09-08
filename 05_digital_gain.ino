#include <avr/interrupt.h>

void setup()
{
    pinMode(3, OUTPUT);

    TCCR3A = _BV(COM3C1) | _BV(WGM30);
    TCCR3B = _BV(WGM32) | _BV(CS30);

    TCCR1A = 0;
    TCCR1B = 0;

    TCCR1B = _BV(WGM12) | _BV(CS11);
    OCR1A = 199;

    ADMUX = _BV(REFS0);

    ADCSRA = _BV(ADEN) |
             _BV(ADIE) |
             _BV(ADPS2) |
             _BV(ADPS1);

    TIMSK1 = _BV(OCIE1A);

    sei();
}

void loop()
{
}

ISR(TIMER1_COMPA_vect)
{
    ADCSRA |= _BV(ADSC);
}

ISR(ADC_vect)
{
    uint16_t sample = ADC;

    int16_t centered =
        (int16_t)sample - 512;

    int16_t processedSample =
        512 + centered / 2;

    OCR3C = processedSample >> 2;
}
