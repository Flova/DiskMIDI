#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int gyro_scale = 131;
int acc_scale = 16384;
int buffersize = 64;
int buffer[64];
int buffer_pointer;
bool booted = false;
float sensitivity = 6;

void setup() {
   Serial.begin(9600);
   Wire.begin();
   Wire.beginTransmission(MPU_addr);
   Wire.write(0x6B);  // PWR_MGMT_1 register
   Wire.write(0);     // set to zero (wakes up the MPU-6050)
   Wire.endTransmission(true);
   for(int count = 0; count < buffersize; count++) {
      get_sensor_data();
      buffer_sensor();
   }
}

void loop() {
  get_sensor_data();
  buffer_sensor();
  int avg = 0;
  for(int count = 1; count < buffersize; count++) {
    avg += abs(get_buffer(-count - 1) - get_buffer(-count))/buffersize;
  }
  int diff = abs(get_buffer(-1) - get_buffer(0));
  if(diff > avg * sensitivity) {
    sendMIDI(0);
  }
}

void buffer_sensor() {
  buffering(sqrt(pow(AcX, 2) + pow(AcY, 2) + pow(AcZ, 2)));
}

void buffering(int value) {
  buffer_pointer++;
  buffer_pointer = buffer_pointer % buffersize;
  buffer[buffer_pointer] = value;
}

int get_buffer(int relative_pos) {
  return buffer[(buffer_pointer + relative_pos + buffersize) % buffersize];
}

void get_sensor_data() {  
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    delay(10);
}

void sendMIDI(int mode) {
  Serial.println("Triggered!");
}

