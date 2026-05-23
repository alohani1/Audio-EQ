# ESP32-S3 Real-Time Audio Equalizer & FFT Visualizer

A dual-core analog and digital signal processing (DSP) application running on the ESP32-S3. This project captures real-time audio, processes it through a custom 5-band biquad equalizer, downsamples the audio with an anti-aliasing decimation filter, and computes a 256-point Fast Fourier Transform (FFT) for live frequency visualization.

## System Architecture

To ensure glitch-free audio processing and responsive UI rendering, the system leverages the dual-core architecture of the ESP32 via FreeRTOS tasks:

* **Core 1 (Audio Task - Priority 10):** Handles high-priority, time-critical I2S audio input/output, executes the 5-band Biquad filter cascade, and runs a 4-point moving average filter for decimation.
* **Core 0 (Control/UI Task - Priority 1):** Polls the 5 physical potentiometers via optimized hardware filtering, updates filter coefficients dynamically, and processes/renders the 256-point FFT data blocks.

---

## Signal Processing Pipeline

1. **Oversampling Input:** Audio is captured via I2S at an oversampled rate of 176.4 kHz to reduce phase distortion in the upper audible bands.
2. **Anti-Aliasing Filter:** A custom 4-point Moving Average Filter (`MovingAverage4`) smooths the high-frequency spectrum.
3. **Decimation:** The audio stream is safely downsampled by a factor of 4 to a standard 44.1 kHz frequency.
4. **5-Band Equalizer:** Audio passes sequentially through a cascade of 5 custom parametric Biquad Filters mapped to fixed audio bands:
   * Sub-Bass / Bass: 100 Hz
   * Low-Mids: 400 Hz
   * Mids: 1 kHz
   * Upper-Mids: 4 kHz
   * Presence / Brilliance: 10 kHz
5. **Spectral Analysis:** A 256-point complex FFT (`arduinoFFT`) converts the time-domain audio buffer into real-world frequency magnitudes using a Hamming window to prevent leakage.

---

## Hardware Configuration

### Pins

**Inputs(Analog)**

* **Bass Pot (100 Hz):** 
* **Low-Mid Pot (400 Hz):** 
* **Mid Pot (1 kHz):** 
* **Upper-Mid Pot (4 kHz):** 
* **Treble Pot (10 kHz):**

**Output(Digital)**

* **External DAC**
* **OLED Display**
  
---

### Software Hardware-Filtering
Potentiometer jitter is completely eliminated in the firmware using the `ResponsiveAnalogRead` library, which puts the ADC pins into a sleep state when physical knobs are stationary to conserve processing overhead.

**Before being passed into the ESP-32, it is essential that you first pass the audio through an analog Anti-Aliasing Filter if using an Audio Jack. Ideal cutoff frequency is ~22 kHz.**

---

## Software Dependencies

The project is built inside the PlatformIO ecosystem using the Visual Studio Code IDE and targets the standard Arduino core framework.

The following libraries are required:
* `dxinteractive/ResponsiveAnalogRead` (Hardware smoothing)
* `kosme/arduinoFFT` (FFT calculations for Arduino)

---
