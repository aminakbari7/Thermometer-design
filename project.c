#include <io.h>
#include <delay.h>
#define LM35_CHAN 0
//////------------------------------define lcd port
#define LCD_RS   PORTD.0
#define LCD_EN   PORTD.1
#define LCD_D4   PORTD.2
#define LCD_D5   PORTD.3
#define LCD_D6   PORTD.4
#define LCD_D7   PORTD.5
////////////////////////////////////////////////----lcd set
void lcd_set(unsigned char data)
{
if(data & (1<<0))
LCD_D4=1;
else
LCD_D4=0;
////////
if(data & (1<<1))
LCD_D5=1;
else
LCD_D5=0;
/////
if(data & (1<<2))
LCD_D6=1;
else
LCD_D6=0;
//////
if(data & (1<<3))
LCD_D7=1;
else
LCD_D7=0;
}
////////////////////////////////////////////////  lcd  com
 void lcd_com(unsigned char com)
 {
 LCD_RS=0;
  lcd_set(com);
  LCD_EN=1;
  delay_ms(1);
  LCD_EN=0;
 }
////////////////////////////////////////////////  lcd init
void lcd_init()
{
  delay_ms(20);
  lcd_com(0x03);
  delay_ms(5);
  lcd_com(0x03);
  delay_ms(1);
  lcd_com(0x03);
  lcd_com(0x02);
  ///////////////
  lcd_com(0x02);
  lcd_com(0x08);

  lcd_com(0x00);
  lcd_com(0x0C);
     
  lcd_com(0x00);
  lcd_com(0x06);
}
//////////////////////////////////////////////// clead lcd
void lcd_clear()
{
lcd_com(0x00);
lcd_com(0x01);
}

//////////////////////////////////////////////// cursor
void lcd_cursor(unsigned char y,unsigned char x)
{

unsigned char temp;
if(y==0)
{
temp=0x80 + x;
}
else if(y==1)
{
temp=0xc0 + x;
}
 lcd_com((temp>>4));
 lcd_com(temp);
}
////////////////////////////////////////////////  write char
void lcd_write_char(unsigned char chr)
{
LCD_RS=1;
 lcd_set((chr>>4));
 LCD_EN=1;
 delay_ms(1);
 LCD_EN=0;
 lcd_set(chr);
 LCD_EN=1;
 delay_ms(1);
 LCD_EN=0;
}
///////////////////////////////////////////////// write string
void lcd_write_string(char *str)
{
unsigned char i;
for(i=0;str[i];i++)
        {
        lcd_write_char(str[i]);
        }
}
//////////////////////////////////////////////// read
unsigned int read(unsigned char ch)
{
  unsigned char adcl_temp,adch_temp;
  ADMUX |=ch;
  delay_us(20);
  ADCSRA |=(1<<ADSC);
  while(ADCSRA & (1<<ADIF)==0)
  ADCSRA |=(1<<ADIF);
  adcl_temp =ADCL;
  adch_temp=ADCH;
  return (((int)adch_temp<<8) | (adcl_temp));
}

 /////////////// ///////////////////////////////
  void convert(int num ,char *str)
 {              
 if(num<10000 && num>=1000)
 {
  str[0]=((num/1000)+48);
  str[1]=(((num/100)%10)+48);
  str[2]=(((num/10)%10)+48);
  str[3]=((num%10)+48);
  str[4]='\0';
 }
 /////
 else if(num<1000 && num>=100)
 {
  str[0]=((num/100)+48);
  str[1]=(((num/10)%10)+48);
  str[2]=((num%10)+48);
  str[3]='\0';
 }
 //////////
  else if(num<100 && num>=10)
 {
  str[0]=((num/10)+48);
  str[1]=((num%10)+48);
  str[2]='\0';
 }
 else
 {
 str[0]=((num%10)+48);
 str[1]='\0';
 }
 }
 /////////////// ///////////////////////////////
void main(void)
{    
     unsigned char  temp_str[5];
     unsigned int temp;  
     DDRB=0XFF;
     DDRD=0XFF;
     ADMUX=(1<<REFS0);
     ADCSRA=(1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

    lcd_init();
    lcd_clear();
    ////---starup
    lcd_write_string("*start sensor *");
    delay_ms(100);
    lcd_clear(); 
    //lcd_cursor(0,2);
    //lcd_write_string("temp : ");   

while (1)
    {    

    temp=0; 
    lcd_cursor(0,0);
    lcd_write_string("temp : ");
        temp=read(LM35_CHAN);
        temp=(int )(temp*4.8828); 
        temp/=10;
        convert(temp,temp_str);
        lcd_cursor(0,9);
        lcd_write_string(temp_str);
        lcd_cursor(0,14);
        lcd_write_char(223);
        lcd_cursor(0,15);
        lcd_write_char('c');
        delay_ms(1000);
        if(temp<=45)
        {    
        PORTB.0=0;
           PORTB.1=0;
           PORTB.2=1; 
        lcd_cursor(1,0);
        lcd_write_string("safe.......temp");
        //delay_ms(2000);
        }
       if(temp>45 & temp<100)
        { 
           PORTB.0=0;
           PORTB.2=0;       
        PORTB.1=1; 
        lcd_cursor(1,0);
        lcd_write_string("midlle.....temp");
        //delay_ms(2000);
        } 
        if(temp>=100)
        {
        PORTB.0=1;
           PORTB.1=0;
           PORTB.2=0;
        lcd_cursor(1,0);
        lcd_write_string("dangerous..temp");
        //delay_ms(2000); 
        }  
    }
    lcd_clear();
}

