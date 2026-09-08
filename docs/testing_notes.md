# Experimental testing notes

## Recorded hardware observations

Equipment: Mega 2560 R3, function generator, oscilloscope, and an approximately 1 kHz sine input on A0.

| Experiment | Expected behavior | Recorded observation |
| --- | --- | --- |
| Timer3 PWM | 16 MHz / 256 = 62.5 kHz | Approximately 62.5 kHz carrier |
| Timer1 OC1A, D11 | 10 kHz compare events, output toggles at each event | 5.0 kHz square wave |
| Comparator | ADC > 512 produces 255; otherwise 0 | 1.0006 kHz output from approximately 1 kHz input |
| PWM duty encoding | Duty changes with processed samples | High widths approximately 6.8–9.6 µs; calculated duty 42.5–60% |
| Floating input | Timer3 continues running independently | Carrier remained with A0 disconnected; floating samples are not meaningful input data |

The measurements are transcribed from the owner's testing record. No new physical measurements were made during repository preparation. Scope captures are pending upload.

## Completed core and extension status

- Core ADC/PWM pipeline, timer verification, interrupt acquisition, comparator, gain, low-pass filtering, and fixed block reordering: implemented; sketches included.
- Initial LFSR/XOR: source included, but the supplied notes explicitly say it has not received the same experimental validation as the timer, comparator, and PWM stages.
- Descrambler, recovered-signal comparison, FFT analysis, analog reconstruction, and final characterization: future work.

## Repository checks

All eight sketches passed ATmega2560-targeted C++ syntax checks using the installed AVR GCC 7.3.0 toolchain and Arduino AVR core 1.8.8 headers. The Arduino CLI full-build attempt was blocked by local package initialization permissions, so a full Arduino build/link and hardware upload are not claimed. The original code was not changed to manufacture a passing result.

## Next validation record

Record the exact sketch revision, generator frequency/amplitude/offset, wiring, probe attenuation, scope coupling, trigger, timebase, and measured values with each capture. Add input-versus-output traces, completed ADC cadence/jitter, block startup/order behavior, LFSR sequence alignment, and digital round-trip checks. Keep calculated expectations, simulated results, and physical measurements labeled separately.
