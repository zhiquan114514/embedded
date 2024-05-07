关于LCD屏幕的操作，请使用以下代码学习

#include <mbed.h>
#include "drivers/LCD_DISCO_F429ZI.h"

int main() {
  const uint8_t spacing = 50;
  LCD_DISCO_F429ZI lcd;
  lcd.DisplayChar(0 + 0 * spacing,LINE(0),'H');
  lcd.DisplayChar(1 + 1 *spacing,LINE(0),'E');
  lcd.DisplayChar(2 + 2 *spacing,LINE(0),'L');
  lcd.DisplayChar(3 + 3 *spacing,LINE(0),'L');
  lcd.DisplayChar(4 + 4 *spacing,LINE(0),'O');

  lcd.DisplayChar(0 + 0 * spacing,LINE(1),'D');
  lcd.DisplayChar(1 + 1 *spacing,LINE(1),'E');
  lcd.DisplayChar(2 + 2 *spacing,LINE(1),'E');
  lcd.DisplayChar(3 + 3 *spacing,LINE(1),'P');

  lcd.DisplayStringAtLine(2,(uint8_t*)"The Deep Dark Fantasy");
  lcd.DisplayStringAt(0,LINE(3), (uint8_t*)"LT KIDDO",CENTER_MODE);
  char buf[60];
  snprintf(buf, 60, "width: %lu pixels", lcd.GetXSize());
  lcd.DisplayStringAtLine(4,(uint8_t* ) buf);

  snprintf(buf, 60, "height: %lu pixels", lcd.GetYSize());
  lcd.DisplayStringAtLine(5,(uint8_t* ) buf);
  lcd.DrawCircle(120, LINE(8), 25);
  lcd.FillCircle(70 + 0*spacing, LINE(11),25);
  lcd.FillCircle(70 + 2*spacing, LINE(11),25);
  lcd.SetTextColor(LCD_COLOR_WHITE);
  lcd.FillCircle(70 + 0*spacing, LINE(11),5);
  lcd.FillCircle(70 + 2*spacing, LINE(11),5); 
  lcd.SetTextColor(LCD_COLOR_BLACK);
  // put your setup code here, to run once:
  Point smile[] ={{70, LINE(15)}, {120, LINE(18)}, {170, LINE(15)}};
  lcd.FillPolygon(smile,3);
  while(1) {
    // put your main code here, to run repeatedly:
  }
}
