#include <avr/interrupt.h>

// LFSR must never be initialized to zero.
volatile uint8_t lfsr = 0xA7;

uint8_t nextLFSR()
{
    // Polynomial:
    // x^8 + x^6 + x^5 + x^4 + 1

    uint8_t feedback =
        ((lfsr >> 7) ^
         (lfsr >> 5) ^
         (lfsr >> 4) ^
         (lfsr >> 3)) & 1;

    lfsr = (lfsr << 1) | feedback;

    return lfsr;
}

void setup()
{
    pinMode(3, OUTPUT);

    // Timer3: 62.5 kHz PWM
    TCCR3A = _BV(COM3C1) | _BV(WGM30);
    TCCR3B = _BV(WGM32) | _BV(CS30);

    // Timer1: 10 kHz sampling
    TCCR1A = 0;
    TCCR1B = 0;

    TCCR1B = _BV(WGM12) | _BV(CS11);
    OCR1A = 199;

    // ADC A0
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
    uint16_t sample10 = ADC;

    // Convert 10-bit ADC sample to 8 bit
    uint8_t sample8 = sample10 >> 2;

    // Generate next pseudo-random value
    uint8_t scrambleValue = nextLFSR();

    // Scramble sample
    uint8_t scrambledSample =
        sample8 ^ scrambleValue;

    // Send scrambled value to PWM
    OCR3C = scrambledSample;
}
