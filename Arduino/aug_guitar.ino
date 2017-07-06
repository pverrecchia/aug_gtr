//Pietro Verrecchia
//MUMT 306 Augmented Guitar Project
//Fall 2012

#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "imuDataBundle.h"
#include <math.h>

//IMU setup

MPU6050 accelgyro;

int gx, gy, gz;
int ax, ay, az;
float roll,pitch, dRoll, dPitch, roll_old, pitch_old = 0;
int biasX, biasY, biasZ;
long prevMicros = 0;

void setup()
{
 
  Wire.begin();
  Serial.begin(9600);

  //Serial.println("Initializing I2C devices...");
  initialize(accelgyro);
  
  //get gyroscope biases when imu is resting. 
  biasX = getBias('x');
  biasY = getBias('y');
  biasZ = getBias('z');

  prevMicros=micros();
	
}

void loop()
{
  
  unsigned long currentMicros = micros();
  unsigned long deltaT = currentMicros - prevMicros;
  
  if (deltaT > 10000){ //10000 us --> 100 Hz
    prevMicros = currentMicros;
  
    imuDataBundle_t imuData;
    
    //acquire data
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    
    //convert velocities to deg/sec and subtract bias taken at beginning of program
    imuData.gx= gx/131.0 - biasX;
    imuData.gy= gy/131.0 - biasY;
    imuData.gz= gz/131.0 - biasZ;
    
    //scale accelerations according to datasheet
    imuData.ax = ax / 16384.0;
    imuData.ay = ay / 16384.0;
    imuData.az = az / 16384.0;
    
    //integrate with deltaT scaled to seconds
    dRoll = imuData.gx * deltaT * 0.000001;
    dPitch = imuData.gz * deltaT * 0.000001;
    
    //complimentary filter
    roll = 0.97*(roll_old + dRoll) + 0.03*atan2(imuData.ay, imuData.az)*57.295; 
    pitch = 0.95*(pitch_old + dPitch) + 0.05*atan2(imuData.ax, imuData.ay)*57.295; 
    
    //scale data
    byte roll_out = byte((roll + 45.0) + 0.5);
    byte pitch_out = byte((pitch + 45.0) + 0.5);
    
    if(pitch_out > 175) pitch_out = 0;
    
    //send to Pd
    
    //Serial.print('R');
    //delay(70);
    //Serial.write(roll_out); 
    //delay(70);
    Serial.print('P');
    delay(70);
    Serial.write(pitch_out);
    
    roll_old = roll;
    pitch_old = pitch;
    
    }
  }



//*********IMU FUNCTIONS**********


void initialize(MPU6050 IMU)
{
  IMU.setClockSource(MPU6050_CLOCK_INTERNAL);
  IMU.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  IMU.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
  IMU.setDLPFMode(MPU6050_DLPF_BW_256);
  IMU.setSleepEnabled(false);
}

float getBias(char channel){

	float N = 30.0;

	float biasBuffer[30];
	float sum = 0;

	for(int i = 0; i < N; i++){
		accelgyro.getRotation(&gx, &gy, &gz);

                //Serial.println("test");

		if(channel == 'x'){
			biasBuffer[i] = (gx/ 131.0);
		}
		if(channel == 'y'){
			biasBuffer[i] = (gy/ 131.0);
		}
		if(channel == 'z'){
			biasBuffer[i] = (gz/ 131.0);
		}

		sum += biasBuffer[i];
	}

	sum = sum / N;  
	return sum;
}



