# Implementation review

This review describes the source currently included in the repository. It distinguishes code-level findings from hardware behavior that still needs reproducible measurement.

## Implemented design

Stages 03–08 use the same interrupt-driven acquisition and output structure:

1. Timer1 enters CTC mode with a prescaler of 8 and `OCR1A = 199`, producing a nominal 10 kHz compare rate from the 16 MHz clock.
2. Each Timer1 compare interrupt starts an ADC0 conversion.
3. The ADC completion interrupt processes the 10-bit sample.
4. The result is reduced to eight bits and written to Timer3 channel C for output on D3.

Timer3 runs in 8-bit Fast PWM mode without a prescaler, giving a calculated carrier frequency of 62.5 kHz. The ADC uses AVcc as its reference and a prescaler of 64, giving a calculated ADC clock of 250 kHz.

The processing stages are present in source:

- Comparator: maps samples above 512 to 255 and all other samples to 0.
- Digital gain: recenters around 512, divides the signed displacement by two, and restores the midpoint.
- Low-pass filter: implements a stateful integer recurrence with a nominal coefficient of one quarter.
- Block permutation: alternates two eight-sample buffers and reads the completed buffer in the order `4, 0, 6, 2, 7, 1, 5, 3`.
- LFSR/XOR experiment: advances an 8-bit LFSR initialized to `0xA7` and XORs its value with each reduced sample.

## Design details worth preserving

- Sampling and PWM generation use separate timers, keeping acquisition cadence independent of the output carrier.
- ADC processing occurs on conversion completion rather than assuming a conversion finishes inside the timer ISR.
- Midpoint-centered gain uses signed intermediate arithmetic, avoiding unsigned underflow below midscale.
- Double buffering allows one sample block to be captured while the previous block is emitted.
- The staged sketches make individual peripheral and processing concepts easier to isolate and test.

## Known boundaries

- The 250 kHz ADC clock is derived from register settings, but effective resolution at that clock has not been measured.
- The calculated steady-state conversion time is shorter than the 100 µs trigger interval, but conversion cadence, interrupt latency, and jitter still require measurement.
- Timer3 produces PWM rather than a reconstructed analog signal. No output filter or DAC is included.
- The block scrambler's static buffers start at zero, so stage 07 emits a zero-filled block during startup.
- Stage 08 does not include a descrambler, framing, synchronization, or round-trip recovery test.
- An LFSR/XOR sequence provides reversible obfuscation when both sides are synchronized; it is not a secure encryption scheme.

## Validation priorities

1. Add a reproducible Arduino build or CI check for all eight sketches.
2. Record Timer1 compare cadence, completed ADC cadence, and timing jitter with the exact firmware revision and scope settings.
3. Exercise the filter and block permutation with known sample vectors and document expected versus observed output.
4. Capture input and PWM output traces with generator amplitude, offset, frequency, probe attenuation, and coupling recorded.
5. Implement a synchronized digital descrambler before making any recovered-signal claim.
6. If analog output is required, design and validate a reconstruction stage for the intended signal bandwidth.
