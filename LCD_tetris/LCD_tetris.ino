#define LCD_SCK 0x02 //PA1
#define LCD_CS 0x08 //PA3
#define LCD_MOSI 0x02 //PC1

#define Encoder_A 0x04
#define Encoder_B 0x08
#define Encoder_Button 0x01

#define digitalRead_C(PIN) (PINC & PIN) != 0 ? 1 : 0
#define digitalRead_D(PIN) (PIND & PIN) != 0 ? 1 : 0

int button_toggle = 0;
int ing = 0;
int LR_count = 0;
int count = 0;

void setup() {
  //shiftout(data, clk, MSBFIRST, 0xff);
  DDRA = LCD_SCK | LCD_CS;
  DDRC = LCD_MOSI;
  PORTA &= ~(LCD_SCK | LCD_CS);
  PORTC &= ~LCD_MOSI;

  DDRC &= ~Encoder_Button;
  DDRD &= ~(Encoder_A | Encoder_B);
  PORTD |= Encoder_A | Encoder_B;
  PORTC |= Encoder_Button;

  delayMicroseconds(50);

  lcd_set_inst(0x30);
  lcd_set_inst(0x30);
  lcd_set_inst(0x30);

  //clecr
  lcd_set_inst(0x30);
  lcd_set_inst(0x06);
  lcd_set_inst(0x0C);

  lcd_set_inst(0x01);
  delay(10);

  lcd_cls();
  delay(1000);
}

unsigned char numbers[10][8] = {
  {0x00, 0x38, 0x44, 0x4C, 0x54, 0x64, 0x44, 0x38},
  {0x00, 0x10, 0x30, 0x50, 0x10, 0x10, 0x10, 0x7c},
  {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c},
  {0x00, 0x38, 0x44, 0x04, 0x18, 0x04, 0x44, 0x38},
  {0x00, 0x08, 0x18, 0x28, 0x48, 0x7C, 0x08, 0x08},
  {0x00, 0x7C, 0x40, 0x78, 0x04, 0x04, 0x44, 0x38 },
  {0x00, 0x38, 0x40, 0x40, 0x78, 0x44, 0x44, 0x38},
  {0x00, 0x7C, 0x04, 0x08, 0x10, 0x20, 0x20, 0x20},
  {0x00, 0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38},
  {0x00, 0x38, 0x44, 0x44, 0x3C, 0x04, 0x44, 0x38},
};

char block_L[8][8] = {
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,1,1,0,0,0,0},
  {0,0,1,1,0,0,0,0},
  {0,0,1,1,1,1,1,1},
  {0,0,1,1,1,1,1,1},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
};

char block_LL[8] = {0x00,0x00,0x30,0x30,0x3F,0x3F,0x00,0x00};


void loop() {
  // set_data(3, 7, 0x5555);
  /*
  for(int i=0;i<8;i++)
  {
    set_data(1,8+i,numbers[LR_count / 10][i]);
    set_data(2,8+i,numbers[LR_count % 10][i]);
    rotary(digitalRead_D(Encoder_A), digitalRead_D(Encoder_B));
    button(digitalRead_C(Encoder_Button));
  }*/

  for(int i=0;i<8;i++){
    set_data(0, i+count, block_LL[i]);
  }
  delay(500);
  for(int i=0;i<8;i++){
    set_data(0, i+count, 0x0000);
  }
  count+=2;
  if(count == 20)count = 0;
}
void lcd_cls()
{
  for(int j=0;j<32;j++){
    for(int i=0;i<16;i++){
      set_data(i,j,0x0000);
    }
  }
}

void set_data(char x,char y, short data)
{
  char addr_x = 0x80 | (x & 0x0f);
  char addr_y = 0x80 | (y & 0x7f);
  char data_high = (0xff00 & data) >> 8;
  char data_low = 0x00ff & data;
  
  lcd_set_inst(0x36);
  lcd_set_inst(addr_y); //y위치
  lcd_set_inst(addr_x); //x위치

  lcd_set_inst(0x30);
  lcd_set_data(data_high);
  lcd_set_data(data_low);

}

void lcd_set_inst(char inst)
{
  PORTA |= LCD_CS; //CS high
  //----------------------------

  shift_out(0xF8);
  shift_out(inst & 0xF0); //상위 4비트
  shift_out((inst<<4) & 0xF0); //하위 4비트를 상위 4비트로
  
  //----------------------------
  PORTA &= ~LCD_CS; //CS low

  delayMicroseconds(100);
}
void lcd_set_data(char data)
{
  PORTA |= LCD_CS; //CS high
  //----------------------------

  shift_out(0xFA);
  shift_out(data & 0xF0); //상위 4비트
  shift_out((data<<4) & 0xF0); //하위 4비트를 상위 4비트로
  
  //----------------------------
  PORTA &= ~LCD_CS; //CS low

  delayMicroseconds(100);
}


void shift_out(char data)
{
  PORTA |= LCD_CS; //CS high
  //----------------------------

  for (int i = 0; i < 8; i++)
  {
    if (data & (0x80 >> i)) {
      PORTC |= LCD_MOSI; //data high
    }
    else {
      PORTC &= ~LCD_MOSI; //data low
    }
    PORTA |= LCD_SCK;
    PORTA &= ~LCD_SCK;
  }

  //----------------------------
  PORTA &= ~LCD_CS; //CS low
}

void button(int Read_button)
{
  if (Read_button == 0 && button_toggle == 0) {
    button_toggle = 1;
  }
  else if (Read_button == 1 && button_toggle == 1) {
    button_toggle = 0;
  }
}
void rotary(int Read_A, int Read_B)
{
  switch (ing)
  {
    case 0:
      if (switch0(Read_A, Read_B) == 1) {
        return;
      }
      else if (switch1(Read_A, Read_B) == 1) {
        ing = 1;
        return;
      }
      else if (switch3(Read_A, Read_B) == 1) {
        ing = -1;
        return;
      }
      return;
    case 1:
      if (switch1(Read_A, Read_B) == 1) {
        return;
      }
      else if (switch2(Read_A, Read_B) == 1) {
        ing = 2;
        return;
      }
      else if (switch0(Read_A, Read_B) == 1) {
        ing = 0;
        return;
      }
      return;
    case 2:
      if (switch2(Read_A, Read_B) == 1) {
        return;
      }
      else if (switch3(Read_A, Read_B) == 1) {
        ing = 3;
        return;
      }
      else if (switch1(Read_A, Read_B) == 1) {
        ing = 1;
        return;
      }
      return;
    case 3:
      if (switch3(Read_A, Read_B) == 1) {
        return;
      }
      else if (switch0(Read_A, Read_B) == 1) {
        ing = 4;
        printing();
        return;
      }
      else if (switch2(Read_A, Read_B) == 1) {
        ing = 2;
        return;
      }
      return;
    case -1:
      if (switch3(Read_A, Read_B) == 1) {
        return;
      }
      else if (switch2(Read_A, Read_B) == 1) {
        ing = -2;
        return;
      }
      else if (switch0(Read_A, Read_B) == 1) {
        ing = 0;
        return;
      }
      return;
    case -2:
      if (switch2(Read_A, Read_B) == 1) {
        return;
      }
      else if (switch1(Read_A, Read_B) == 1) {
        ing = -3;
        return;
      }
      else if (switch3(Read_A, Read_B) == 1) {
        ing = -1;
        return;
      }
      return;
    case -3:
      if (switch1(Read_A, Read_B) == 1) {
        return;
      }
      else if (switch0(Read_A, Read_B) == 1) {
        ing = -4;
        printing();
        return;
      }
      else if (switch2(Read_A, Read_B) == 1) {
        ing = -2;
        return;
      }
      return;

  }
}
void printing()
{
  if (ing == 4) {
    LR_count++;
    ing = 0;
    return;
  }
  if (ing == -4) {
    LR_count--;
    ing = 0;
    return;
  }
}
int switch0(int Read_A, int Read_B)
{
  if (Read_A == 1 && Read_B == 1) return 1;
  return 0;
}
int switch1(int Read_A, int Read_B)
{
  if (Read_A == 0 && Read_B == 1) return 1;
  return 0;
}
int switch2(int Read_A, int Read_B)
{
  if (Read_A == 0 && Read_B == 0) return 1;
  return 0;
}
int switch3(int Read_A, int Read_B)
{
  if (Read_A == 1 && Read_B == 0) return 1;
  return 0;
}
