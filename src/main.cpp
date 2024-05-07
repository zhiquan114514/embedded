#include <mbed.h>
#include "drivers/LCD_DISCO_F429ZI.h"
#define CTRL_REG1 0x20
#define CTRL_REG1_CONFIG 0b01'10'1'1'1'1
#define CTRL_REG4 0x23
#define CTRL_REG4_CONFIG 0b0'0'01'0'00'0
#define SPI_FLAG 1
#define OUT_X_L 0x28

EventFlags flags;

void spi_cb(int event)
{
    flags.set(SPI_FLAG);
}

#define SCALING_FACTOR (17.5f * 0.0174532925199432957692236907684886f / 1000.0f)

int main() {
  LCD_DISCO_F429ZI lcd;
  
  SPI spi(PF_9, PF_8, PF_7, PC_1, use_gpio_ssel);
  uint8_t write_buf[32], read_buf[32];
  spi.format(8, 3);
  spi.frequency(1'000'000);
  write_buf[0] = CTRL_REG1;
  write_buf[1] = CTRL_REG1_CONFIG;
  spi.transfer(write_buf, 2, read_buf, 2, spi_cb);
  flags.wait_all(SPI_FLAG);
  write_buf[0] = CTRL_REG4;
  write_buf[1] = CTRL_REG4_CONFIG;
  spi.transfer(write_buf, 2, read_buf, 2, spi_cb);
  flags.wait_all(SPI_FLAG);
  int counter = 0;
  bool tremor_detected = 0;
  float x_total = 0;
  float y_total = 0;
  float z_total = 0;
  while(1) {
        uint16_t raw_gx, raw_gy, raw_gz;
        float gx, gy, gz;

        // Prepare to read the gyroscope values starting from OUT_X_L
        write_buf[0] = OUT_X_L | 0x80 | 0x40;

        // Perform the SPI transfer to read 6 bytes of data (for x, y, and z axes)
        spi.transfer(write_buf, 7, read_buf, 7, spi_cb);
        flags.wait_all(SPI_FLAG);

        // Convert the received data into 16-bit integers for each axis
        raw_gx = (((uint16_t)read_buf[2]) << 8) | ((uint16_t) read_buf[1]);
        raw_gy = (((uint16_t)read_buf[4]) << 8) | ((uint16_t) read_buf[3]);
        raw_gz = (((uint16_t)read_buf[6]) << 8) | ((uint16_t) read_buf[5]);

        gx = ((float) raw_gx) * SCALING_FACTOR;
        gy = ((float) raw_gy) * SCALING_FACTOR;
        gz = ((float) raw_gz) * SCALING_FACTOR;

        if(gx > 20)
          gx = 0;
        if(gy > 20)
          gy = 0;
        if(gz > 20)
          gz = 0;
        //printf("RAW -> \t\tgx: %d \t gy: %d \t gz: %d \t\n", raw_gx, raw_gy, raw_gz);
        //printf("\n------------------------------------\n");
        //printf("Gyro -> \t\tgx: %4.5f \t gy: %4.5f \t gz: %4.5f \t\n", gx, gy, gz);
        //printf("\n------------------------------------\n");        
        if(counter < 10){
          x_total += gx;
          y_total += gy;
          z_total += gz;
          counter++;
        } else{
        float x_avg = x_total /10;
        float y_avg = y_total /10;
        float z_avg = z_total /10;
        char textBuffer[30];
        char xValBuffer[15];
        char yValBuffer[15];
        char zValBuffer[15];
        snprintf(xValBuffer, sizeof(xValBuffer), "gx: %4.5f", x_avg);
        snprintf(yValBuffer, sizeof(yValBuffer), "gy: %4.5f", y_avg);
        snprintf(zValBuffer, sizeof(zValBuffer), "gz: %4.5f", z_avg);

        lcd.DisplayStringAtLine(16, (uint8_t*)"Gyro Values-> ");
        lcd.DisplayStringAtLine(17, (uint8_t*) xValBuffer);
        lcd.DisplayStringAtLine(18, (uint8_t*) yValBuffer);
        lcd.DisplayStringAtLine(19, (uint8_t*) zValBuffer);
        counter = 0;
        x_total = 0;
        y_total = 0;
        z_total = 0;
        }

      //TODO tremor detection

      
        if(tremor_detected){
          lcd.SetTextColor(LCD_COLOR_RED);
          lcd.DisplayStringAtLine(10, (uint8_t*)"TREMOR DETECTED ");
        } else{
          lcd.SetTextColor(LCD_COLOR_BLACK);
          lcd.DisplayStringAtLine(10, (uint8_t*)"TREMOR NOT DETECTED");          
        }


      thread_sleep_for(50);

      } 
  }
