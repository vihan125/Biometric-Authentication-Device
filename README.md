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
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/OV7670/pin_description.PNG width="30%" height="30%">
</p>

### Functional Block diagram

<p align="center">
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/OV7670/block%20diagram.PNG width="30%" height="30%">
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











