# aug_gtr
Real-time manipulation of guitar signal using an IMU, Arduino and PureData

_This is an old project I did for a university class in 2012 (MUMT 306 @ McGill University). The resouces were originally hosted on my personal webpage. I'm bringing over the code and info as-is to Github._

![The Arduino was mounted to the guitar body and the IMU was mounted to the back of the headstock](http://pietro.io/src/img/gtr734.jpg)

## Introduction

The goal of this project is to achieve basic real-time gestural control of guitar effects using an intertial measurment unit (IMU), Arduino, and Pure Data (Pd). Playing guitar is a dynamic activity and it is of interest to explore how physical movements of the player or guitar could relate to the intrument's sound. The IMU is mounted to the headstock of a guitar and measures its accelerations and angular velocities. Then, the data is transmitted to a laptop serially via the Arduino and is used to control patch parameters in Pd and manipulate the guitar's signal (almost) instantly. 

## Implementation

I2C communication between the IMU and the Arduino was set up using Jeff Rowberg's i2cdevlib. Pitch, defined here as the angle between the guitar neck and horizontal, is calculated using a Complimentary filter. A Complimentary filter combines acceleration and angular velocity measurments from the IMU to compute angular position. It is a good choice because it is simple to implement in code and effectively reduces the measurment error associated with accelerometers and gyroscopes. Alternatively, the more robust Kalman filter would yeild a more accurate result but is conceptually and computationally complex. More information about the Complimentray filter's use with IMUs can be found [here](http://scolton.blogspot.ca/2012/09/fun-with-complementary-filter-multiwii.html). 

Next, the pitch value is offset and scaled to one byte in preparation for serial transmission to Pd. Once received in Pd, the angle's offset is removed and can be used to control patch parameters. Meanwhile, the USB audio interface is used to get the guitar's audio signal into Pd. To use the patch, pictured below, the toggle switches just need to be turned on for one or both of the effects. The dry signal bypasses the effects when neither is selected. At the bottom of the patch, there is a live-updating time-lapse plot of the pitch angle. 

I experimented with two types of simple motion control in Pd - continuous and discrete. The Pd patch includes an example of each. The continuous effect is a stereo tremolo that fades left or right as the guitar is moved upwards or downwards. The effect is always on when the toggle switch in Pd is pressed. The discrete-style effect is a recreation of "star power" from the Guitar Hero video game series. When the guitar is tilted vertically, a distorion effect is activated for a 10 second period to enhance a guitar solo. When the distortion turns off after 10 seconds, the guitar plays normally until tilted again. 

![Screenshot of PureData patch. The patch reads data from the Arduino over via a com port and uses the data to manipulate the audio signal](http://pietro.io/src/img/patch.jpg)

## Hardware and Software Used

* MPU 6000 IMU
* Arduino Mega 2560
* Tascam US-100 USB audio interface
* [PureData](https://puredata.info/)
* Electric guitar + amplifier

## Additional Resources
* [I2C library for MPU6000](https://www.i2cdevlib.com/devices/mpu6050#source)
* [Intro to the complimentary filter](http://www.pieter-jan.com/node/11)
* [More complimentary filter info](http://d1.amobbs.com/bbs_upload782111/files_44/ourdev_665531S2JZG6.pdf)
* [Even more IMU fusion info](http://ca.mouser.com/applications/sensor_solutions_mems/)


