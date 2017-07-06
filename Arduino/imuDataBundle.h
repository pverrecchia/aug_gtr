#ifndef imuDataBundle
#define imuDataBundle

typedef struct {
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
  int fail;
} imuDataBundle_t;

#endif 

