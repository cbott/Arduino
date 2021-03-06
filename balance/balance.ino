/* MPU9250 Basic Example Code
 Hardware setup:
 MPU9250 Breakout --------- Arduino
 VDD ---------------------- 3.3V
 VDDI --------------------- 3.3V
 SDA ----------------------- A4
 SCL ----------------------- A5
 GND ---------------------- GND
 */

#include "quaternionFilters.h"
#include "MPU9250.h"

#include "sn754410.h"

#define SerialDebug true  // Set to true to get Serial output for debugging

// Pin definitions
int intPin = 12;  // These can be changed, 2 and 3 are the Arduinos ext int pins
int myLed  = 13;  // Set up pin 13 led for toggling

const int LEFT_A = 5;
const int LEFT_B = 6;
const int RIGHT_A = 9;
const int RIGHT_B = 10;

sn754410 drive(LEFT_A, LEFT_B, RIGHT_A, RIGHT_B);

MPU9250 myIMU;

float get_temperature(){
  myIMU.tempCount = myIMU.readTempData();  // Read the adc values
  // Temperature in degrees Centigrade
  myIMU.temperature = ((float) myIMU.tempCount) / 333.87 + 21.0;
  // Return temperature in degrees Centigrade
  return myIMU.temperature;
}

void update_heading(){
  myIMU.yaw   = atan2(2.0f * (*(getQ()+1) * *(getQ()+2) + *getQ() *
                *(getQ()+3)), *getQ() * *getQ() + *(getQ()+1) * *(getQ()+1)
                - *(getQ()+2) * *(getQ()+2) - *(getQ()+3) * *(getQ()+3));
  myIMU.pitch = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ() *
                *(getQ()+2)));
  myIMU.roll  = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2) *
                *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1) * *(getQ()+1)
                - *(getQ()+2) * *(getQ()+2) + *(getQ()+3) * *(getQ()+3));
  myIMU.pitch *= RAD_TO_DEG;
  myIMU.yaw   *= RAD_TO_DEG;

  myIMU.yaw   += 7.0;
  myIMU.roll  *= RAD_TO_DEG;
}

void setup()
{
  Wire.begin();
  // TWBR = 12;  // 400 kbit/sec I2C speed
  if(SerialDebug) Serial.begin(38400);

  // Set up the interrupt pin, its set as active high, push-pull
  pinMode(intPin, INPUT);
  digitalWrite(intPin, LOW);
  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH);

  // Read the WHO_AM_I register, this is a good test of communication

  byte c;
  do {
    c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    if(SerialDebug){
      Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
      Serial.print(" I should be "); Serial.println(0x71, HEX);
    }
    if(c != 0x71){
      if(SerialDebug){
        Serial.print("Could not connect to MPU9250: 0x");
        Serial.println(c, HEX);
      }
      delay(1000);  
    }
  } while(c != 0x71);

  if(SerialDebug){ Serial.println("MPU9250 is online..."); }

  // Start by performing self test and reporting values
  myIMU.MPU9250SelfTest(myIMU.SelfTest);
  if(SerialDebug){
    Serial.print("x-axis self test: acceleration trim within : ");
    Serial.print(myIMU.SelfTest[0],1); Serial.println("% of factory value");
    Serial.print("y-axis self test: acceleration trim within : ");
    Serial.print(myIMU.SelfTest[1],1); Serial.println("% of factory value");
    Serial.print("z-axis self test: acceleration trim within : ");
    Serial.print(myIMU.SelfTest[2],1); Serial.println("% of factory value");
    Serial.print("x-axis self test: gyration trim within : ");
    Serial.print(myIMU.SelfTest[3],1); Serial.println("% of factory value");
    Serial.print("y-axis self test: gyration trim within : ");
    Serial.print(myIMU.SelfTest[4],1); Serial.println("% of factory value");
    Serial.print("z-axis self test: gyration trim within : ");
    Serial.print(myIMU.SelfTest[5],1); Serial.println("% of factory value");
  }

  // Calibrate gyro and accelerometers, load biases in bias registers
  myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);

  myIMU.initMPU9250();
  // Initialize device for active mode read of acclerometer, gyroscope, and
  // temperature
  if(SerialDebug){ Serial.println("MPU9250 initialized for active data mode...."); }

  // Read the WHO_AM_I register of the magnetometer, this is a good test of
  // communication
  byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);

  if(SerialDebug){
    Serial.print("AK8963 "); Serial.print("I AM "); Serial.print(d, HEX);
    Serial.print(" I should be "); Serial.println(0x48, HEX);
  }
  // Get magnetometer calibration from AK8963 ROM
  myIMU.initAK8963(myIMU.magCalibration);
  // Initialize device for active mode read of magnetometer
  if (SerialDebug)
  {
    Serial.println("AK8963 initialized for active data mode....");
    //  Serial.println("Calibration values: ");
    Serial.print("X-Axis sensitivity adjustment value ");
    Serial.println(myIMU.magCalibration[0], 2);
    Serial.print("Y-Axis sensitivity adjustment value ");
    Serial.println(myIMU.magCalibration[1], 2);
    Serial.print("Z-Axis sensitivity adjustment value ");
    Serial.println(myIMU.magCalibration[2], 2);
  }

  drive.set(0.0,0.0);
}

float prev_speed = 0;

float k_angle = -0.2; //-
float k_w = -0.25; //- angular velocity
float k_speed = 0.05;//+

void loop()
{
  // If intPin goes high, all data registers have new data
  // On interrupt, check if data ready interrupt
  if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {  
    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values
    myIMU.getAres();

    // Now we'll calculate the accleration value into actual g's
    // This depends on scale being set
    myIMU.ax = (float)myIMU.accelCount[0]*myIMU.aRes; // - accelBias[0];
    myIMU.ay = (float)myIMU.accelCount[1]*myIMU.aRes; // - accelBias[1];
    myIMU.az = (float)myIMU.accelCount[2]*myIMU.aRes; // - accelBias[2];

    myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values
    myIMU.getGres();

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    myIMU.gx = (float)myIMU.gyroCount[0]*myIMU.gRes;
    myIMU.gy = (float)myIMU.gyroCount[1]*myIMU.gRes;
    myIMU.gz = (float)myIMU.gyroCount[2]*myIMU.gRes;

    myIMU.readMagData(myIMU.magCount);  // Read the x/y/z adc values
    myIMU.getMres();
    // User environmental x-axis correction in milliGauss, should be
    // automatically calculated
    myIMU.magbias[0] = +470.;
    // User environmental x-axis correction in milliGauss TODO axis??
    myIMU.magbias[1] = +120.;
    // User environmental x-axis correction in milliGauss
    myIMU.magbias[2] = +125.;

    // Calculate the magnetometer values in milliGauss
    // Include factory calibration per data sheet and user environmental
    // corrections
    // Get actual magnetometer value, this depends on scale being set
    myIMU.mx = (float)myIMU.magCount[0]*myIMU.mRes*myIMU.magCalibration[0] -
               myIMU.magbias[0];
    myIMU.my = (float)myIMU.magCount[1]*myIMU.mRes*myIMU.magCalibration[1] -
               myIMU.magbias[1];
    myIMU.mz = (float)myIMU.magCount[2]*myIMU.mRes*myIMU.magCalibration[2] -
               myIMU.magbias[2];
  } // if (readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)

  // Must be called before updating quaternions!
  myIMU.updateTime();

  // Sensors x (y)-axis of the accelerometer is aligned with the y (x)-axis of
  // the magnetometer; the magnetometer z-axis (+ down) is opposite to z-axis
  // (+ up) of accelerometer and gyro! We have to make some allowance for this
  // orientationmismatch in feeding the output to the quaternion filter. For the
  // MPU-9250, we have chosen a magnetic rotation that keeps the sensor forward
  // along the x-axis just like in the LSM9DS0 sensor. This rotation can be
  // modified to allow any convenient orientation convention. This is ok by
  // aircraft orientation standards! Pass gyro rate as rad/s
//  MadgwickQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f,  my,  mx, mz);
  MahonyQuaternionUpdate(myIMU.ax, myIMU.ay, myIMU.az, myIMU.gx*DEG_TO_RAD,
                         myIMU.gy*DEG_TO_RAD, myIMU.gz*DEG_TO_RAD, myIMU.my,
                         myIMU.mx, myIMU.mz, myIMU.deltat);

  update_heading();

  float ang_vel = myIMU.gy;
  float angle = myIMU.roll;

  float speed = (k_w * ang_vel) + (k_angle * angle) + (k_speed * prev_speed);

  drive.set(speed, speed);
  prev_speed = constrain(speed, -1.0, 1.0);


  myIMU.delt_t = millis() - myIMU.count;
  if (SerialDebug && myIMU.delt_t > 1000){
    Serial.print("X-acceleration: "); Serial.print(1000*myIMU.ax);
    Serial.print(" mg ");
    Serial.print("Y-acceleration: "); Serial.print(1000*myIMU.ay);
    Serial.print(" mg ");
    Serial.print("Z-acceleration: "); Serial.print(1000*myIMU.az);
    Serial.println(" mg ");

    // Print gyro values in degree/sec
    Serial.print("X-gyro rate: "); Serial.print(myIMU.gx, 3);
    Serial.print(" degrees/sec ");
    Serial.print("Y-gyro rate: "); Serial.print(myIMU.gy, 3);
    Serial.print(" degrees/sec ");
    Serial.print("Z-gyro rate: "); Serial.print(myIMU.gz, 3);
    Serial.println(" deg/sec");

    // Print mag values in degree/sec
    Serial.print("X-mag field: "); Serial.print(myIMU.mx);
    Serial.print(" mG ");
    Serial.print("Y-mag field: "); Serial.print(myIMU.my);
    Serial.print(" mG ");
    Serial.print("Z-mag field: "); Serial.print(myIMU.mz);
    Serial.println(" mG");

    Serial.print("q0 = "); Serial.print(*getQ());
    Serial.print(" qx = "); Serial.print(*(getQ() + 1));
    Serial.print(" qy = "); Serial.print(*(getQ() + 2));
    Serial.print(" qz = "); Serial.println(*(getQ() + 3));

    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(myIMU.yaw, 2);
    Serial.print(", ");
    Serial.print(myIMU.pitch, 2);
    Serial.print(", ");
    Serial.println(myIMU.roll, 2);

    Serial.print("Speed: "); Serial.println(speed);
    Serial.println();

    myIMU.count = millis();

  } // every X ms
}
