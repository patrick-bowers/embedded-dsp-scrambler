#include <avr/interrupt.h>

void setup()
{
    pinMode(3, OUTPUT);

    // Timer3 PWM
    TCCR3A = _BV(COM3C1) | _BV(WGM30);
    TCCR3B = _BV(WGM32) | _BV(CS30);

    // Timer1: 10 kHz sampling clock
    TCCR1A = 0;
    TCCR1B = 0;

    TCCR1B = _BV(WGM12) | _BV(CS11);
    OCR1A = 199;

    // ADC
    // AVcc reference, ADC0 / A0 input
    ADMUX = _BV(REFS0);

    // ADC enabled
    // ADC interrupt enabled
    // Prescaler = 64
    ADCSRA = _BV(ADEN) |
             _BV(ADIE) |
             _BV(ADPS2) |
             _BV(ADPS1);

    // Timer1 Compare A interrupt
    TIMSK1 = _BV(OCIE1A);

    sei();
}

void loop()
{
}

ISR(TIMER1_COMPA_vect)
{
    // Begin ADC conversion every 100 us
    ADCSRA |= _BV(ADSC);
}

ISR(ADC_vect)
{
    uint16_t sample = ADC;

    OCR3C = sample >> 2;
}
