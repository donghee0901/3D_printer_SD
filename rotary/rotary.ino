#define Encoder_A 0x04
#define Encoder_B 0x08
#define Encoder_Button 0x01

#define digitalRead_C(PIN) (PINC & PIN) != 0 ? 1 : 0
#define digitalRead_D(PIN) (PIND & PIN) != 0 ? 1 : 0



void setup() {
  Serial.begin(9600);
  DDRC &= ~Encoder_Button;
  DDRD &= ~(Encoder_A | Encoder_B);
  PORTD |= Encoder_A | Encoder_B;
  PORTC |= Encoder_Button;
}

void loop() {
  rotary(digitalRead_D(Encoder_A), digitalRead_D(Encoder_B));
  button(digitalRead_C(Encoder_Button));
}

int button_toggle = 0;

void button(int Read_button)
{
  if (Read_button == 0 && button_toggle == 0) {
    button_toggle = 1;
    Serial.println("Push");
  }
  else if (Read_button == 1 && button_toggle == 1) {
    button_toggle = 0;
    Serial.println("Pop");
  }
}

int ing = 0;
int LR_count = 0;
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
    Serial.print("right : ");
    Serial.println(LR_count);
    ing = 0;
    return;
  }
  if (ing == -4) {
    LR_count--;
    Serial.print("left : ");
    Serial.println(LR_count);
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
