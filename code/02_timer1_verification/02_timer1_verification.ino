void setup()
{
    // Arduino Mega Pin 11 = OC1A
    pinMode(11, OUTPUT);

    TCCR1A = 0;
    TCCR1B = 0;

    // Toggle OC1A on compare match
    TCCR1A = _BV(COM1A0);

    // CTC mode, prescaler = 8
    TCCR1B = _BV(WGM12) | _BV(CS11);

    OCR1A = 199;
}

void loop()
{
}
