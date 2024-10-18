# Serial Commands

All communications with the VibeCheck are UTF8 (plain text) encoded, with spaces or commas as token separators and newline character as delimiter.  

## Commands

This is a list of serial commands for the VibeCheck separated into categories by subsystem. Arguments for the commands and types are indicated with brackets: [argument name:type]. The VibeCheck will respond first with "ack" after successfully processing a command, and then with the requested value(s) if calling a getter command. Remember to terminate the command with a newline (LF) character when sending to the VibeCheck or else it will not respond.

### Sensors

- `sensor fakedata start`
  
  Starts sending fake accelerometer data (3 sine waves out of phase) over the serial port for testing the plotting UI and data logging.

- `sensor fakedata stop`
  
  Stops sending fake accelerometer data.

- `sensor set packetsize [size:int]`

  Sets how many data points are included in each data transmission. This value can be adjusted to balance latency in receiving samples (smaller packet size might be better at low data rates) versus avoiding overloading the data link by calling the USB transmission function too often (large packet size might be better at high data rates). The minimum value is 1 and the maximum value is defined in the firmware (*currently 512 but could change*).

- `sensor get packetsize`

  Gets how many data points are included in each data transmission.

- `sensor [channel:int] start accel`
  
  Starts measuring acceleration on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2). If that channel is not currently connected but the acceleration measurement is started, it will start measuring data immediately upon connection.

- `sensor [channel:int] stop accel`
  
  Stops measuring acceleration on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2).

- `sensor [channel:int] start gyro`
  
  Starts measuring angular rate on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2). If that channel is not currently connected but the angular rate measurement is started, it will start measuring data immediately upon connection.

- `sensor [channel:int] stop gyro`
  
  Stops measuring angular rate on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2).

- `sensor [channel:int] set accel odr [odr:int]`
  
  Sets the accelerometer output data rate (ODR) on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2). The argument is in Hz, and the allowed values are:

  13, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660

  The VibeCheck will select the closest allowed value in the event of an argument that is not permitted.

- `sensor [channel:int] get accel odr`
  
  Gets the accelerometer output data rate (ODR) in Hz on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2).

- `sensor [channel:int] set accel range [range:int]`
  
  Sets the accelerometer range in g on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2). The allowed values are:

  2, 4, 8, 16

  The VibeCheck will select the closest allowed value in the event of an argument that is not permitted. A setting of 2, for instance, means that we can measure accelerations between -2 g and +2 g.

- `sensor [channel:int] get accel range`
  
  Gets the accelerometer range in g on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2).

- `sensor [channel:int] set gyro odr [odr:int]`
  
  Sets the gyroscope output data rate (ODR) on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2). The argument is in degrees per second, and the allowed values are:

  13, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660

  The VibeCheck will select the closest allowed value in the event of an argument that is not permitted.

- `sensor [channel:int] get gyro odr`
  
  Gets the gyroscope output data rate (ODR) in Hz on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2).

- `sensor [channel:int] set gyro range [range:int]`
  
  Sets the gyroscope range in degrees per second on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2). The allowed values are:

  125, 245, 500, 1000, 2000

  The VibeCheck will select the closest allowed value in the event of an argument that is not permitted. A setting of 125, for instance, means that we can measure angular rates between -125 deg/s and +125 deg/s.

- `sensor [channel:int] get gyro range`
  
  Gets the gyroscope range in degrees per second on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2).

- `sensor [channel:int] set offsets [x:float] [y:float] [z:float]`
  
  Sets the on-chip accelerometer DC offset corrections in g on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2). The offsets are the loaded into the accelerometer registers to be subtracted from the raw sensor readings. Each chip potentially acquires some DC offset error during the manufacturing/board soldering process so the offsets are daughterboard-specific.

- `sensor [channel:int] get offsets`
  
  Gets the on-chip accelerometer DC offset corrections in g on the specified channel. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2). 

- `sensor [channel:int] get connected`
  
  Returns 1 if that sensor channel is connected or 0 if it is not connected. The channel refers to the physical port on the board, zero indexed (so the allowed values are 0, 1 and 2). 

### Strobe

- `strobe start`
  
  Starts the strobe blinking.

- `strobe stop`
  
  Stops the strobe.

- `strobe set frequency [frequency:float]`
  
  Sets the strobe frequency in Hz. The argument will be clamped to the minimum and maximum values set in firmware.

- `strobe get frequency`
  
  Returns the strobe frequency in Hz.

- `strobe set phase [phase:float]`
  
  Sets the phase in degrees of the strobe flash relative to an arbitrary origin point. The argument will be between -180.0 degrees and 180.0 degrees.

- `strobe get phase`
  
  Returns the phase of the strobe flash in degrees.

- `strobe set exposure [exposure:float]`
  
  Sets the on time of the strobe flash in milliseconds.

- `strobe get exposure`
  
  Returns the on time of the strobe flash in milliseconds.

### Waveform Generator

- `wavegen start`
  
  Starts generating a waveform with the specified parameters and outputting it through the audio jack.

- `wavegen stop`
  
  Stops the waveform generation. To avoid noise, the DAC is set to the mid-range value rather than tri-stated.

- `wavegen demo start`
  
  Starts a fun song

- `wavegen demo stop`
  
  Stops the song

- `wavegen set frequency [frequency:float]`
  
  Sets the frequency of the waveform in Hz. The argument will be clamped to the minimum and maximum values set in the firmware. There is a tradeoff between resolution of the waveform, timer capabilities and intonation.

- `wavegen get frequency`
  
  Returns the frequency of the waveform in Hz.

- `wavegen set amplitude [amplitude:float]`
  
  Sets the amplitude of the waveform. The argument should be between 0 and 1 — this range is mapped to the full scale of the DAC.

- `wavegen get amplitude`
  
  Returns the amplitude of the waveform between 0 and 1.

- `wavegen set waveform [waveform:str]`
  
  Sets the shape of the generated waveform. Options are "sine", "square", "saw" or "triangle"

- `wavegen get waveform`
  
  Returns the shape of the generated waveform. Options are "sine", "square", "saw" or "triangle"

### RGB LEDs

- `rgb start`
  
  Starts a demo of the RGB LEDs with different colors and sequencing

- `rgb stop`
  
  Stops the demo and turns all the lights off

- `rgb set [index:int] [r:int] [g:int] [b:int]`
  
  Set a single pixel to a certain color. The pixels are zero-indexed starting near the USB port. The colors are 0-255.

- `rgb get [index:int]`
  
  Gets the RGB color value of the specified LED.

## Responses

The responses from the board are organized into 3 types. Data is the packetized sensor measurements which will continuously stream once the measurement is enabled. Events are external user inputs such as pressing a button on the VibeCheck or plugging in a daughterboard. Acknowledgements are responses to commands, sometimes containing data if it was requested. The form of the specific acknowledgement messages are found in the commands description.

### Data

A data transmission consists of a packet of sensor readings arranged into data points. The data transmission will look as follows:

```
"data [numpoints:int] [channel0:int] [timestamp0:int] [xdata0:float]  [ydata0:float]  [zdata0:float] [channel1:int] [timestamp1:int] [xdata1:float]  [ydata1:float]  [zdata1:float] … " 
```

It begins with the data keyword and then lists the number of data points that will follow in the packet. The first element of each data point is the channel that generated the data. The accelerometers and gyroscopes are treated as separate data channels, so sensor 0 would contain data channel 0 (accelerometer) and data channel 1 (gyroscope). Sensor 1 would contain data channel 2 (accelerometer) and data channel 3 (gyroscope), and so on. The next element of the data packet is the timestamp in microseconds. *This is subject to change because a 32 bit unsigned integer timestamp in microseconds will overflow in about 70 minutes.* Finally the data point contains the three axes of measured data whose unit depends on the sensor that produces it. Acceleration readings are in g and the gyroscope measurements have units of degrees per second.

### Events

Events correspond to user interactions directly with the VibeCheck board (not through the web UI). Currently the only sources of events are the user pressing a button (mute or record) or the connection/disconnection of a sensor. 

*The currently implemented events are as follows. This is all likely to change as we refine the UX.*

```
"event wavegen muted"
"event wavegen unmuted"
"event sensor [channel:int] connected"
"event sensor [channel:int] disconnected"
```
