# Design review and validation plan

Prepared from the June 2026 progress notes, the original blueprint, and the author's September 2026 update. The update supersedes the June implementation status: Timer1 controls 10 kHz sampling events; Timer3/OCR3C drives D3; gain, low-pass filtering, and double-buffered eight-sample reordering are implemented according to the author. Current source and scope captures remain pending. The corrections below concern historical documents unless explicitly noted.

## Historical document corrections and remaining checks

1. **Board and timer mapping.** The blueprint names an Uno; the progress notes name a Mega 2560. On a Mega, D3 is OC3C and Timer2 output OC2B is D9. A Timer2/OCR2B example for Uno D3 cannot simply be reused on Mega D3. The author now reports the correct Timer3/OCR3C configuration for D3; confirm it in the source when uploaded. Check the actual sketch against the [official Mega pinout](https://docs.arduino.cc/resources/pinouts/A000067-full-pinout.pdf).

2. **Sampling arithmetic.** For a 16 MHz AVR and normal 13-cycle conversions, prescaler 16 implies a 1 MHz ADC clock and a theoretical conversion rate near 76.9 kS/s. A 500 kHz ADC clock corresponds to prescaler 32 and about 38.5 kS/s. Prescaler 64 gives about 19.2 kS/s, not 32 kS/s. These historical free-running calculations do not describe the updated Timer1-driven 10 kS/s design. Completed conversion throughput and jitter still need measurement. Higher ADC clocks trade resolution for speed; do not claim a specific effective resolution without measurements. See [Microchip ADC timing guidance](https://www.microchip.com/content/dam/mchp/documents/OTH/ApplicationNotes/ApplicationNotes/AN2538-ADC-of-megaAVR-in-SingleEnded-Mode-00002538A.pdf).

3. **Reconstruction filter.** A 1 kΩ / 10 µF RC stage has `fc = 1/(2*pi*R*C) ≈ 15.9 Hz`. For a single unloaded stage, its amplitude response at 1 kHz is approximately 0.0159 (about -36 dB), so it heavily suppresses the intended kHz signal. Two passive stages also load each other. Choose a filter based on the desired signal bandwidth and measured PWM carrier, rather than treating 10 µF as a validated audio reconstruction value. The progress note's text diagram also appears to connect the output junction directly to ground; clarify the actual wiring.

4. **Spectral inversion target.** Alternating sample sign maps a real tone below Nyquist to `f_out = fs/2 - f_in`. At 10 kS/s, 500 Hz maps to 4.5 kHz; 1 kHz maps to 4 kHz. A 1 kHz-to-4.5 kHz claim requires a different sample rate. The updated nominal 10 kS/s design would map a 1 kHz tone to 4 kHz if alternating-sign spectral inversion is added. The current reported scrambling is sample reordering, not that spectral-inversion mode.

5. **Sample scaling.** The blueprint subtracts 512 from a 10-bit sample, then adds 128 and writes an 8-bit PWM value. That can clip or wrap; define the 10-to-8-bit scaling, signed intermediate range, and saturation explicitly before implementing it.

6. **Sampling and PWM update rates.** A nominal 76.9 kS/s ADC rate exceeds a 62.5 kHz PWM carrier. Register writes are not automatically equivalent to independently reconstructed samples; the updated nominal 10 kS/s design avoids this specific rate mismatch, but must still account for output register buffering.

7. **Security and recovery.** A constant XOR byte is a reversible digital transform, not secure encryption. Exact digital XOR recovery does not establish exact recovery after PWM filtering and re-sampling, which lose information. Remove claims that these experiments reproduce the security of named tactical radios.

## Validation record to build

| Test | Record | Current evidence |
| --- | --- | --- |
| Input bias | Scope capture, min/max, offset, probe setting | Numerical observations in June notes only |
| Sample timing | Completed conversion interval, jitter, timer settings | Author reports 5.0 kHz OC1A toggle output for 10 kHz timer events; capture pending |
| PWM carrier | Pin, timer mode, frequency, duty cycle range | Timer3/D3, 62.5 kHz and 6.8–9.6 µs widths reported; capture pending |
| Bypass | Paired input/output traces, gain and phase versus frequency | Pending |
| Gain, low-pass, sample reordering | Source, numeric vectors, paired traces, buffer timing | Implementation reported September 2026; files pending |
| Spectral mapping | Actual fs, input frequency, predicted and measured output peak | Pending |
| LFSR/XOR | Digital round-trip check, sequence alignment, separate analog distortion assessment | Planned next stage |

## Simulation scope

[Wokwi](https://docs.wokwi.com/getting-started/supported-hardware) supports the Mega 2560 and digital logic analysis. Its [analog simulation is limited](https://docs.wokwi.com/chips-api/analog), so it is not evidence that this analog reconstruction circuit works. The listed built-in hardware also does not include the FM project's Si4703 tuner.

A later simulation should target the actual saved firmware, label its inputs and assumptions, and publish its source alongside screenshots. Numerical filter plots or synthetic FFTs should be labeled as calculated/simulated, never as oscilloscope measurements. No simulation results are claimed for this repository release.
