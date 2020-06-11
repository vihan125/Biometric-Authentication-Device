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

###Capturing fingerprint image

As the first attempt optical flow sensor ADNS 3080 was used to capture the fingerprint image because it is very sensitive to movemnts and has a high resolution with small images.
ADNS 3080 module takes a image with 30x30 resolution. 

####Implementation using ADNS 3080 optical flow sensor

ADNS 3080 optical flow sensor is normally used in mouse devices because it is sensitive to movement.
This sensor has some in build registers with can be accessed by the user and get neccessary data.
This device uses SPI communication and clock is driven by the master.  
some of the inbuild registers are,

<p align="center">
  <image src = https://github.com/vihan125/Micro-bank-App-/blob/master/images/ER.jpeg width="90%" height="90%">
</p>
  
  
  


## App interfaces
### Home interface
First interface that will be displayed to the user. There are two types of users as customer and agent. Agent is the owner of the embedded device and user is the bank customer. User can login to their account using  customer login and if agent wants to do changes in the balance of the account they can use the agent updates option.
<p align="center">
  <image src = https://github.com/vihan125/Micro-bank-App-/blob/master/images/Screenshot_1577949022.png width="30%" height="30%">
</p>

### User login interface
This is the interface that customers use to login to their accounts. If the user the username or password is incorrect it will generate a error message.
<p align="center">
  <image src = https://github.com/vihan125/Micro-bank-App-/blob/master/images/Screenshot_1577949030.png width="30%" height="30%">
</p>

### Options interface
After loging into their accounts customers have two options as deposit money or to withdraw money. the balance of their account will be shown at the bottom this balance is taken from the main server.
<p align="center">
  <image src = https://github.com/vihan125/Micro-bank-App-/blob/master/images/Screenshot_1577949044.png width="30%" height="30%">
</p>
  
  ### User deposits interface
  This interface is used to do deposit transactions. After a transaction is made that details will be saved in the transactions table and the makes table.
<p align="center">
  <image src = https://github.com/vihan125/Micro-bank-App-/blob/master/images/Screenshot_1577949050.png width="30%" height="30%">
</p>
  
### User withdrawl interface
This interface is used to do withdrawl transactions. After a transaction is made that details will be saved in the transactions table and the makes table.
<p align="center">
  <image src = https://github.com/vihan125/Micro-bank-App-/blob/master/images/Screenshot_1577949055.png width="30%" height="30%">
</p>
  
### Agent login interface
This is the interface used by the agent to login to their account to do updates in the device.
<p align="center">
  <image src = https://github.com/vihan125/Micro-bank-App-/blob/master/images/Screenshot_1577949063.png width="30%" height="30%">
</p>
  
### Agent update interface
This interface is used to do updates in the device and the balance table will get update according to the value entered. this is the available balance in the device. customers cannot withdraw more than this balance.
<p align="center">
  <image src = https://github.com/vihan125/Micro-bank-App-/blob/master/images/Screenshot_1577949082.png width="30%" height="30%">
</p>
  
### Transaction successful interface
This indicates that a transaction has successfully completed.
<p align="center">
  <image src = https://github.com/vihan125/Micro-bank-App-/blob/master/images/Screenshot_1577948982.png width="30%" height="30%">
</p>


