# TransparentAudioAmplifier

Operation   	
Once the Transparent USB Audio Headphone Amplifier is set up, a USB cord from a PC to D-to-A subsystem can be connected to receive sound from the computer and headphones, of user’s choice, can be attached to the audio jack.
       	The D-to-A system will pass a signal from the PC to the reconstruction filter. 
       	The signal is then passed through a reconstruction filter. This smooths out high frequency sampling produced by the DAC. The filter has a cut off frequency of 42 kHz as the DAC provided to us over-samples two times.
       	The filter is connected to the amplifier controls subsystem. The signal tones and volume can be adjusted by the user through the GUI interface. 
The GUI adjusts Volume, Balance, Treble, Mid and Bass by sending out SPI signals to digital potentiometers. The customer actively alters the SPI signals by using the sliders on the GUI touch screen to change the audio output to his/her liking. Moving the slider to the right will induce greater filter boost and/or an increase in volume, depending on the attribute being changed. Moving a slider to the left will result in greater cut or a reduction in volume of the audio output, the exact opposite affect of what happens when moving the slider to the right.  The VU meter analyzes the input signals and displays a scaled version of the wave on the touchscreen
	Finally, the amplifier controls send the signal to the power amplifier. Here, the signal is amplified in voltage by the preamplifier, then amplified in current by the power amplifier. In total, this amplifies the power, providing the signal just enough to drive a varity of headphones, ranging from 16Ω to 600Ω



Sub-Systems

Reconstruction Filter

The reconstruction filter is used to produce a smooth analogue signal from the DAC’s digital input. It removes the high sampling frequency of the D to A to prevent damage to sensitive parts of the circuit. This could include preventing mechanical damage of the speaker. Thus, a very high quality filter will be required. The Reconstruction filter will incorporate a 3rd order Butterworth filter. Filters approach ideal filter behavior as the order approaches infinity. At the same time filters tend to get expensive and difficult to implement as the order increases. We are assuming that the DAC will oversample data 4 times. Butterworth filters have no pass or stop band ripple and it has a flat frequency response. Other filters that were considered were Chebyshev and Bessel.

VU + GUI Controls

The GUI accepts user input to control audio signals output via a touchscreen. This consists of a TIVA microcontroller & a compatible touchscreen. The touchscreen provides a slider interface for the user to adjust audio controls. The microcontroller interfaces to 5 digital potentiometers which are in the Amplifier controls. It does this via SPI. The VU meter removes negative values in the input waveform. It uses a full wave rectifier that converts the whole of the input waveform to one of constant polarity, in this case positive.

