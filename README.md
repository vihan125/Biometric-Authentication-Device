# BIOMETRIC AUTHENTICATION DEVICE

## Project description

Personal identity refers to a set of attributes like name, social security number , blood type etc that are associated with a person.
Identity management is the process of creating or linking the attributes to a physical person, maintaining and destroying identities of individuals in a population. 
One of the critical tasks in identity management is person authentication, where the goal is to either determine the previously established identity of an individual or verify an individual's identity claim. 
This can be accomplished by three methods. The two conventional methods of authentication are based on a person’s exclusive possession of a token (e.g., ID card or key) or knowledge of a secret (e.g., password). 
The third method, called biometric recognition, authenticates a person based on his biological and behavioral (biometric) traits.
Here the basic idea is to build a biometric authentication device, using the fingerprint image of a thumb of a person.  
There are such devices already available in the market which keeps a templates of fingerprint images within the device and do the authentication.
In this device there is no storing of template images in the device instead the captured image will be transfered to the back end or a central server and identification is done and the results will be transfered back to the device.
According to the result it get access will granted or denied. In this project scope is to build only the first part of the device which is capturing the fingerprint image and transfering it to the back end.  
Another key point in this project is that image will be captured, comprssed and encrypted according to the ISO/IEC standards.
So the image transfered to the backend can be used for any other perpose.
 


## Implementation

### Capturing fingerprint image

As the first attempt optical flow sensor ADNS 3080 was used to capture the fingerprint image because it is very sensitive to movemnts and has a high resolution with small images.
ADNS 3080 module takes a image with 30x30 resolution. 

#### Implementation using ADNS 3080 optical flow sensor

ADNS 3080 optical flow sensor is normally used in mouse devices because it is sensitive to movement.
This sensor has some in build registers with can be accessed by the user and get neccessary data.
This device uses SPI communication and clock is driven by the master.  
some of the inbuild registers are,

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/read%20me%20images/REGISTERS.PNG width="50%" height="50%">
</p>
  
 Here the registers like motion, pixel burst can be used to detect motion and capture an image.  

Message in the SPI communication consist of two Bytes. First bit of the first byte indicate weather it is a read or write operation.
'1' for write operation and '0' for read operation. Other 7 bits in the first byte indicates the address of the register and the secong byte contains data.
In an write operation  bus is owned by master and in a read operation control of the bus is handed over to the cmaera module. 

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/read%20me%20images/READ%20WRITE.PNG width="50%" height="50%">
</p>

 
Implemetation was done using python lanuage using GPIO library and as the development board a raspberry pi model B was used.
In this implementation clock given in the raspberry pi was not used and clock was driven manually using coding.
As a example go through following piece of code which shows how the write operation works.
 
<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/read%20me%20images/code.PNG width="50%" height="50%">
</p>
 
As you can see in the above image whole SPI communication was done manually without using any libraries outside and any person with brief knowladge about SPI communication can understand the implementation.
Here python lists have used to indicate the address of a register and data that has to be transfered.
Addresses of the registers and data that has to be transfered indicated in binary format using '1's and '0's.
According to these '1' and '0' MOSI is raised to high or low.
And when reading data depending on the voltage level of the MISO pin data is recorded in to a python list and converted to decimal format later.
Clock was driven using a gpio pin at the whole time.  
whole implementation can be found here : https://github.com/vihan125/Biometric-Authentication-Device/blob/master/ADNS%203080%20cmaera%20spi%20communication/camera.py  

To capture an image burst mode in the module called as frame capture mode can be used. To trigger the capture, write to the Frame_Capture
register. The next available complete 1 2/3 frames (1536
values) will be stored to memory. The data are is retrieved
by reading the Pixel_Burst register. Output image pixel order will be as in the following diagram

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/read%20me%20images/Capture.PNG width="50%" height="50%">
</p>

Image which is captured using above method is shown below.

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/images%20by%20ADNS%203080/focus22.png width="20%" height="20%">
</p>

As you can see using this ADNS 3080 module a clear finger print image of thumb cannot be captured because of the low resolution so camera module with  suitable focal lenght
 and resolution was needed.
 
## Image comprssion

The ISO/IEC standard for compressing fingerprint images is the WSQ format which is used by the FBI. WSQ is Wavelet Scalar Quantization in this method image is
represented using wavelets and the co-efficients of those wavelets are quantized and stored. And when the image is needed again it is reconstructed using those co-efficients.
This a lossy commpression technique so the reconstructed image will not be exactly same as the original image but the key features of the fingerprint will be preserved.   

implementation of this whole software is available in: https://github.com/lessandro/nbis  

Iwent through the source code and extract neccessary files and did neccesarry changes to build code that do only encoding and decoding process.
New implementation is available here : https://github.com/vihan125/Biometric-Authentication-Device/tree/master/WSQ/new_encoder 

to run the code run the make file in and execute the executable file, pixels data in hard coded at the present sitiuation, and the decoded pixel values are written to the 'results.txt' file.

### WSQ compression of a 600x339 image

Original image

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/compressed_examples/2.PNG width="80%" height="80%">
</p>

Reconstructed image

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/compressed_examples/3.PNG width="80%" height="80%">
</p>

Note that the size of the reconstructed image is less because the WSQ image compression is a lossy image compression method.

### It is important to note that WSQ image compression technique will not work with images with low resolution

For an example take a look at this compression of lena image with resolution 256x256
<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/compressed_examples/1.PNG width="80%" height="80%">
</p>


## OV7670 Camera module

The OV7670 is a low cost image sensor + DSP that can operate at a maximum of 30 fps and 640 x 480 ("VGA") resolutions, equivalent to 0.3 Megapixels. The captured image can be pre-processed by the DSP before sending it out. 
This preprocessing can be configured via the Serial Camera Control Bus (SCCB).  
You can see the full datasheet here: https://github.com/vihan125/Biometric-Authentication-Device/tree/master/OV7670%20camera 

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/OV7670/camera.jpg width="50%" height="50%">
</p>

### Pin layout and description

OV7670 camera module consists of 18 pins,

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/OV7670/pinout.PNG width="20%" height="20%">
</p>

Meaning and the functions of the pins 

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/OV7670/pin_description.PNG width="50%" height="50%">
</p>

### Functional Block diagram

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/OV7670/block%20diagram.PNG width="100%" height="100%">
</p>

As you can see there are two clock inputs as XCLK and SIO_C. 
XCLK is the clock for the system operation and the SIO_c is the clock for the SCCB to communicate with registers. 


* You can safely supply 3.3V (3.0V - 3.6V) to the OV7670 VDD. (I used this configuration)
* You can safely use a maximum of 3.0V for the I/O pins. However the module I/O pins will work at 2.8V.
* A 5V supply for the OV7670 VDD might work (try at your own risk), it depends on the maximum input voltage of the LDO regulators your module has.
* You can use 3.3V on the I/O pins, the internal I/O protection diodes will clamp the I/O voltage to 2.8V. However, this may degrade the OV7670 faster and/or cause more power loss. (I used this configuration)

### Capturing frames

By default OV7670 camera module outfputs 640x480 frames at a rate of 30 fps, You can change these seetibgs by making neccessary changes in DSP through Serial Camera Control Bus (SCCB) which operates in a similar manner to I2c communication.
However this SCCB is used only to make chnages in the settings of the module. 
When capturing a frame pins that will be used are PCLK (pixel clock), XCLK(system clock), HREF(horizontal refernce), VSYNC(vertical synchronization) and d0-d7 pins which will give out a byte.

* Lets see how to capture a Data related to a single row with 640 pixels.
<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/OV7670/clock_1.PNG width="100%" height="100%">
</p>

First thing to notice, the D0-D7 must be sampled at the rising edge of the PCLK signal. Number two, D0-D7 must be sampled only when HREF is high.
Also, the rising edge of HREF signals the start of a line, and the falling edge of HREF signals the end of the line.

All these bytes sampled when HREF was high, correspond to the pixels in one line.
Note that one byte is not a pixel, it depends on the format chosen. By default, the format is YCbCr422, this means that in average two bytes correspond to a pixel.

* Capturing a single frame with 640x480 pixels (307200)

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/OV7670/clock_2.PNG width="100%" height="100%">
</p>

The image above shows the signals for a "VGA" (640 x 480) frame.
During HSYNC high state, we must capture 640 pixels, equivalent to a line.
The 480 lines, equivalent to a frame, are captured during the low state of VSYNC. This means that the falling edge of VSYNC signals the start of a frame, and its rising edge signals the end of a frame.

That covers all the process of obtaining one frame, the remaining question is how fast are frames sent.
By default, the PCLK will have the same frequency of XCLK, however prescalers and PPLs can be configured using the SCCB, to produce a PCLK of different frequency.

A PCLK of 24 MHz will produce 30 fps, a PCLK of 12 MHz will produce 15 fps and so on.
All this is independent of the format of the image (VGA, CIF, QCIF, etc).

However OV7670 clock speed is not campatible with raspberry pi GPIO pins. When we try to read pixel values what we get is only some pixel values of a frame here an there.
Clock frequency is too much for GPIO pins.

Then I try using OV7670 with an arduino board, and I was able to find two libraries as well,
1. arducam library : Arducam library : https://github.com/ArduCAM/Arduino
2. LiveOV7670 library : https://github.com/indrekluuk/LiveOV7670 

But this solution causes some issues with the device
1. Using a separate micro controller to just to take an image makes project more complex.
2. Power consumption also high when using two MCUs for one device.
3. Most of the libraries are focused on generating a video frame rather than capturing an image.
4. OV7670 camera module has 201 registers for different purposes.
5. OV7670 camera module has couple versions as one with a FIFO buffer another one without a FIFO buffer.
6. Because of the high frame rate (30fps) most of the implementations are done with low resolution of 340x280.

By considering all these factors I decided to move to another camera module which is compatible with raspberry pi.

## Raspberry pi camera module

This is the camera module that is build for using with raspberry pi. You just have to plug the camera in and can easily capture images with various resolutions and you can take video clips as well.
Here is a link to get an idea about how to operate raspberry pi camera : https://projects.raspberrypi.org/en/projects/getting-started-with-picamera 

This camera has a fixed focal length and you can capture clear images at a distance about 15cm, if you need to capture nearby objects you might have to use lens.
here is a link shown how a microscope has built with use of raspberry pi camera and some lenses : https://www.raspberrypi.org/forums/viewtopic.php?f=43&t=210605&start=25#p1308543

with the help of this camera i was able to capture very detailed image of my palm and after WSQ compression the rebuild image showed even more features.
You can see the results in the images below,

* Image taken from raspberry pi camera module

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/rasp%20camera/images/3.PNG width="80%" height="80%">
</p>

* Image that was reconstructed after WSQ image compression

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/rasp%20camera/images/4.PNG width="80%" height="80%">
</p>


# Final product

With help of new raspberry pi camera module I was able to finalize my product.
Biometric data was chnaged from fingerprint data to palm data because of the focusing issue of the raspberry pi camera module.

## Reasones for changing from fingerprint data to palm data 

* palm data is more secure than fingerprint and face data because palm data is hard to fake as its covered in most occasions
* Hardware limitations due to COVID-19 pandamic situation.
<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/read%20me%20images/01.PNG width="50%" height="50%">
</p>

## Color of light used to capture images

When caprturing biometric data Green and Blue colour lights were used. As Blue and Green colour lights have a high frequency than other colours of light.
Because of the high frequency light rays bounce back from the surface from the object where else other low frequency lights are absorbed into the object. That is the 
reason why most natural object in the environment looks blue or green.
<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/read%20me%20images/color%20spectrum.png width="80%" height="80%">
</p>

## Design 

Because of these changes device was design in a way where device can be change according to the requirements. Device was considered as three sub components as 
Camera, encoder and the transmitter. Flow chart of the Device is as follows.

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/read%20me%20images/flow%202.png width="80%" height="80%">
</p>

As shown in the above flow char there two common sources for the three sub components. The image taken by the camera is fed into the encoder and ecoder takes image as an
input and write decoded data into a text file. This text file is taken into the transmitter and transmit the data into the backend server. These subcomponents can be chnaged 
according to the requirements and it should take inputs according to the above flow chat and output the desired output. Lets say we want to change this device to a fingerprint detecting device what we have to do is
replace the camera module with a suitable camera and feed the out image to the encoder.

When consider about the class diagram, there are three interfaces that are given to the three sub components and if we replace a device it should implement the repective methods stated in the interface which will be used in the main class.
Class Diagram is shown below,

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/read%20me%20images/class%20diagram.png width="80%" height="80%">
</p>

## Output

With the limited resorces available I was able to build a small device, Using a cardboard box and some LEDs.
There are three LEDs in the device as green, blue and yellow,
* Green - will glow to indicate the initialization of the device
* Blue - will glow to indicate when a image of the biometric data is captured
* Yellow - will glow when encoded data is transmitted to the backend

<p align="left">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/read%20me%20images/device%201.jpeg width="30%" height="30%">
</p>

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/read%20me%20images/device%202.jpeg width="30%" height="30%">
</p>

<p align="right">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/read%20me%20images/light.jpeg width="30%" height="30%">
</p>













