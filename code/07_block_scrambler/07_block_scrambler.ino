#include <avr/interrupt.h>

#define BLOCK_SIZE 8

volatile uint16_t bufferA[BLOCK_SIZE];
volatile uint16_t bufferB[BLOCK_SIZE];

volatile uint16_t* fillBuffer = bufferA;
volatile uint16_t* outputBuffer = bufferB;

volatile uint8_t fillIndex = 0;
volatile uint8_t outputIndex = 0;

const uint8_t scrambleOrder[BLOCK_SIZE] =
{
    4, 0, 6, 2, 7, 1, 5, 3
};

void setup()
{
    pinMode(3, OUTPUT);

    // Timer3 PWM
    TCCR3A = _BV(COM3C1) | _BV(WGM30);
    TCCR3B = _BV(WGM32) | _BV(CS30);

    // Timer1: 10 kHz
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

    // Store newest sample
    fillBuffer[fillIndex] = sample;

    // Output a sample from the previous block
    // using the scrambling permutation
    uint16_t outputSample =
        outputBuffer[scrambleOrder[outputIndex]];

    OCR3C = outputSample >> 2;

    fillIndex++;
    outputIndex++;

    if (fillIndex >= BLOCK_SIZE)
    {
        // Exchange capture and output buffers
        volatile uint16_t* temp = fillBuffer;

        fillBuffer = outputBuffer;
        outputBuffer = temp;

        fillIndex = 0;
        outputIndex = 0;
    }
}
