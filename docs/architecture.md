# Architecture and implementation details

## Acquisition and output

Stages 03–08 use Timer1 in CTC mode. At 16 MHz with prescaler 8 and OCR1A = 199, the nominal compare event rate is `16,000,000 / (8 * 200) = 10,000 Hz`. The Timer1 ISR starts an ADC conversion by setting ADSC; the ADC completion ISR processes the sample and writes OCR3C.

The ADC uses AVcc and ADC0, with a prescaler of 64. Its 250 kHz clock gives a normal 13-cycle conversion time near 52 µs. The first conversion after enabling is longer; steady-state timing arithmetic alone is not a measurement of startup behavior, interrupt latency, or jitter. The 250 kHz ADC clock also exceeds the usual maximum-resolution recommendation; no measured effective-bit resolution is claimed.

Timer3 uses 8-bit Fast PWM with no prescaler. Its carrier is `16,000,000 / 256 = 62,500 Hz`. Output is D3 / OC3C, not Timer2's Mega output. The PWM register carries an 8-bit duty value after reducing the 10-bit ADC sample.

## Processing stages

Gain subtracts the midpoint using an `int16_t`, divides by two, and restores 512. This avoids unsigned underflow below the midpoint. The filter starts at 512 and applies integer `difference / 4`; integer rounding means it is not an exact floating-point implementation of alpha = 0.25.

The block scrambler fills one eight-element `uint16_t` buffer while outputting the prior buffer through permutation `4, 0, 6, 2, 7, 1, 5, 3`. Both indices advance once per ADC interrupt and reset at the buffer swap. Static buffers initially contain zero, so the first output block is zero-filled. An eight-sample block spans 0.8 ms; individual sample delay varies with its permutation position.

The LFSR stage starts at `0xA7`, computes feedback from bits 7, 5, 4, and 3, shifts left, inserts feedback, and XORs the updated state with `sample10 >> 2`. A descrambler must match the seed, update order, and sample alignment. This sketch contains no descrambler or framing/synchronization protocol.

## Analog boundaries

Keep A0 within the ADC's electrical limits with a suitable bias and common ground. The current sketches output PWM; there is no verified reconstruction filter in this release. At 10 kS/s, the theoretical Nyquist frequency is 5 kHz, with practical usable bandwidth depending on input filtering and implementation. This is a low-bandwidth experiment, not a high-fidelity audio system.

## Source provenance

All eight sketches were extracted from the project owner's supplied versions, with code and comments preserved. Their filenames were aligned with their numbered containing folders for Arduino IDE compatibility. They are successive experiments, not eight simultaneously active modes or a single mode-selecting firmware.
