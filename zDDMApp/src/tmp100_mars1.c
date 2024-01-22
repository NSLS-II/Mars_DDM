// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TMP100
// This code is designed to work with the TMP100_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Temperature?sku=TMP100_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>


float read_ltc2451(int dev, int addr)
{

   char data[2] = {0};
   float cTemp, fTemp;

	if (ioctl(dev, I2C_SLAVE, addr) < 0) {
	   printf("Device %i not selected\n",addr);
	}

	// Select configuration register(0x01)
	// Continuous conversion, comparator mode, 12-bit resolution(0x60)
	
	//config[0] = 0x01;
	//config[1] = 0x60;
	//bytes=write(file, config, 2);
	//printf("Bytes written to device %i: %i\n",i,bytes);
	//sleep(1);

	// Read 2 bytes of data from register(0x00)
	// temp msb, temp lsb
	
	if(read(dev, data, 2) != 2)
	    printf("ADC Read Error : Input/Output error \n");
	else {
            // Convert the data to 16-bits
   	    int temp = (data[0] * 65536) + data[1];
   	    cTemp = temp * 1.0;

    	    // Output data to screen
 	    //printf("Temperature # %i in Celsius is : %.2f C \n", addr, cTemp);
 	}
   return cTemp;

}


float read_tmp100(int dev, int addr)
{

   char data[2] = {0};
   float cTemp, fTemp;

	if (ioctl(dev, I2C_SLAVE, addr) < 0) {
	   printf("Device %i not selected\n",addr);
	}

	// Select configuration register(0x01)
	// Continuous conversion, comparator mode, 12-bit resolution(0x60)
	
	//config[0] = 0x01;
	//config[1] = 0x60;
	//bytes=write(file, config, 2);
	//printf("Bytes written to device %i: %i\n",i,bytes);
	//sleep(1);

	// Read 2 bytes of data from register(0x00)
	// temp msb, temp lsb
	
	if(read(dev, data, 2) != 2)
	    printf("Error : Input/Output error \n");
	else {
            // Convert the data to 12-bits
   	    int temp = (data[0] * 256 + (data[1] & 0xF0)) / 16;
	    if (temp > 2047) 
		temp -= 4096;
   	    cTemp = temp * 0.0625;
 	    fTemp = cTemp * 1.8 + 32;

    	    // Output data to screen
 	    //printf("Temperature # %i in Celsius is : %.2f C \n", addr, cTemp);
 	}
   return cTemp;

}



void main() 
{
	// Create I2C bus
	int file, i, bytes;
	char *bus = "/dev/i2c-0";
	char config[2] = {0};
	char data[2] = {0};
	char reg[1] = {0x00};
        float tmp[2] = {0};
	
	if ((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, TMP100 I2C address is 0x4F(79)
	int addr[4];
	addr[0] = 0b1001000;        // 1st TMP100 address 
	addr[1] = 0b1001001;        // 2nd TMP100 address
	addr[2] = 0b1001010;        // 3rd TMP100 address                         
        addr[3] = 0b0010100;        // LTC2451 ADC
	
        while (1) {
            for(i=0;i<3;i++)
               tmp[i] = read_tmp100(file,addr[i]);
            
            //tmp[3] = read_ltc2451(file,addr[3]);
            tmp[3] = 0;

            sleep(1); 
            printf("%f\t%f\t%f\t%f\n",tmp[0],tmp[1],tmp[2],tmp[3]);
       


 }

}
