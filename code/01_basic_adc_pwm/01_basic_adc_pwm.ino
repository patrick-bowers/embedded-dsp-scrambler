void setup()
{
    pinMode(3, OUTPUT);

    // Timer3: Fast PWM, 8-bit
    TCCR3A = _BV(COM3C1) | _BV(WGM30);

    // Prescaler = 1
    // PWM frequency = 16 MHz / 256 = 62.5 kHz
    TCCR3B = _BV(WGM32) | _BV(CS30);
}

void loop()
{
    uint16_t sample = analogRead(A0);

    // Convert 10-bit ADC value to 8-bit PWM value
    OCR3C = sample >> 2;
}
