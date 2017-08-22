# TransparentAudioAmplifier

Introduction: The customer has requested of Team Two to build a transparent audio system that amplifies signals provided by means of USB digital audio to a vast array of headphones. The customer will be able to plug a USB cable from the system into a PC running windows. Per the customer’s demands, the system will be able to support a wide variety of headphones. For example, once the system is connected to the PC and playing music, the customer will be able to connect headphones with impedances ranging from 16Ω – 600Ω. This audio system will be integrated into one final package with the tone controls being manipulated by a touchscreen interface. With all of the controls being easily navigable through a simple touchscreen, the customer can finely tune the audio sound to his/her liking. Ultimately, the customer desires a low noise, low distortion audio amplifier suitable for frequencies ranging from 20 Hz to 20,000 Hz. The system will include a touchscreen used as a three band tone control unit. These frequency bands include bass, mid, and treble frequencies. From using the touch screen, the customer will have over +/- 6 dB of adjustment over the bass, mid, and treble controls.   The touch screen will also include controls allowing manipulation of the volume and balance of the audio signals. Included with the touchscreen interface will be a VU meter that presents the signal amplitude of the left and right channels. This total package allows the customer to fully immerse himself/herself in the music listening experience. 


Operation:   	
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

