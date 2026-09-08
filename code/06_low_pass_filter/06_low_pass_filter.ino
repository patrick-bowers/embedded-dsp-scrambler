#include <avr/interrupt.h>

volatile uint16_t filteredSample = 512;

void setup()
{
    pinMode(3, OUTPUT);

    // Timer3 PWM
    TCCR3A = _BV(COM3C1) | _BV(WGM30);
    TCCR3B = _BV(WGM32) | _BV(CS30);

    // Timer1 sampling clock
    TCCR1A = 0;
    TCCR1B = 0;

    TCCR1B = _BV(WGM12) | _BV(CS11);
    OCR1A = 199;

    // ADC
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

    // y[n] = y[n-1] + 0.25(x[n] - y[n-1])

    int16_t difference =
        (int16_t)sample -
        (int16_t)filteredSample;

    filteredSample += difference / 4;

    OCR3C = filteredSample >> 2;
}
