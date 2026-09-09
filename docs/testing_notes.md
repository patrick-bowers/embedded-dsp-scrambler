# Testing status and measurement plan

## Evidence boundary

The values below were transcribed from development notes. The original oscilloscope captures, complete instrument settings, and a revision-linked test log are not included in this repository, so these values should be treated as preliminary bench observations rather than independently reproducible results.

The noted setup used an Arduino Mega 2560 R3, a function generator, an oscilloscope, and an approximately 1 kHz sine-wave input on A0.

## Preliminary bench observations

| Experiment | Expected from implementation | Observation recorded in development notes |
| --- | --- | --- |
| Timer3 PWM carrier | `16 MHz / 256 = 62.5 kHz` | Approximately 62.5 kHz |
| Timer1 diagnostic on D11 | 10 kHz compare events; one output toggle per event produces a 5 kHz waveform | 5.0 kHz square wave |
| Comparator | Samples above 512 produce full-scale PWM duty; lower samples produce zero duty | 1.0006 kHz output for an approximately 1 kHz input |
| PWM duty encoding | High time changes with the processed sample | Approximately 6.8–9.6 µs high time, equivalent to 42.5–60% of a 16 µs period |
| Floating A0 | Timer3 continues to generate its carrier | Carrier remained present; floating ADC samples are not meaningful signal data |

No new physical measurements were made while organizing the repository.

## What source inspection confirms

- All eight staged sketches are present.
- Stages 03–08 configure Timer1-triggered ADC acquisition and Timer3 PWM output.
- Comparator, midpoint-centered gain, integer low-pass filtering, and fixed block permutation are implemented in their respective sketches.
- Stage 08 contains an LFSR/XOR transform, but no inverse path or synchronization mechanism.

Source inspection alone does not establish successful compilation, upload, electrical behavior, timing accuracy, signal fidelity, or round-trip recovery. No build log or automated test result is committed here.

## Current validation status

| Area | Status |
| --- | --- |
| Register configuration and algorithm structure | Reviewable in source |
| Timer, comparator, and PWM observations | Preliminary values recorded; captures unavailable |
| Reproducible builds for every sketch | Not documented in the repository |
| ADC latency, cadence, and jitter | Not measured here |
| Filter response and block-order vectors | Not documented here |
| LFSR sequence characterization | Not documented here |
| Descrambling and recovered-sample comparison | Not implemented |
| Analog reconstruction | Not implemented |

## Recommended test record

For each future measurement, record:

- sketch path and commit revision;
- board and supply voltage;
- generator waveform, frequency, amplitude, offset, and output impedance;
- probe attenuation, coupling, trigger, and timebase;
- measurement points and shared-ground arrangement;
- calculated expectation, observed value, and acceptance criterion.

Priority captures are the Timer1 diagnostic output, ADC completion cadence, input-versus-PWM timing, filter response, and block-permutation behavior with a known repeating input. Keep calculated values, simulations, and physical measurements clearly labeled.
