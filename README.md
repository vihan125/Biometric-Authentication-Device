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
  <image src = https://github.com/vihan125/Biometric-Authentication-Device/blob/master/images/images%20by%20ADNS%203080/focus22.png width="100%" height="100%">
</p>

As you can see using this ADNS 3080 module a clear finger print image of thumb cannot be captured because of the low resolution so camera module with  suitable focal lenght
 and resolution was needed.
 
## Image comprssion

The ISO/IEC standard for compressing fingerprint images is the WSQ format which is used by the FBI. WSQ is Wavelet Scalar Quantization in this method image is
represented using wavelets and the co-efficients of those wavelets are quantized and stored. And when the image is needed again it is reconstructed using those co-efficients.
This a lossy commpression technique so the reconstructed image will not be exactly same as the original image but the key features of the fingerprint will be preserved.   


