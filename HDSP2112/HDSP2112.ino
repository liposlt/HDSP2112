//By liposlt
//SIEMENS HDSP2112S测试程序

#define HC595_DS  6
#define HC595_RCK 4
#define HC595_SCK 3
#define FLASH     7
#define RESET     5
#define CE        9
#define WR        8

#define HC595_DS_RESET    digitalWrite(HC595_DS, LOW)
#define HC595_DS_SET      digitalWrite(HC595_DS, HIGH)

#define HC595_ST_CP_RESET digitalWrite(HC595_RCK, LOW)    //RCK
#define HC595_ST_CP_SET   digitalWrite(HC595_RCK, HIGH)

#define HC595_SH_CP_RESET digitalWrite(HC595_SCK, LOW)    //SCK
#define HC595_SH_CP_SET   digitalWrite(HC595_SCK, HIGH)

/*-------------------------data format-------------------------------
  data0 == D7    D6    D5    D4    D3    D2    D1    D0
  data1 == X     X     X     A4    A3    A2    A1    A0
  -------------------------------------------------------------------*/

void setup() {
  // put your setup code here, to run once:
  pinMode(HC595_DS, OUTPUT);
  pinMode(HC595_RCK, OUTPUT);
  pinMode(HC595_SCK, OUTPUT);
  pinMode(FLASH, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(CE, OUTPUT);
  pinMode(WR, OUTPUT);

  digitalWrite(HC595_DS, HIGH);
  digitalWrite(HC595_RCK, HIGH);
  digitalWrite(HC595_SCK, HIGH);
  digitalWrite(FLASH, HIGH);
  digitalWrite(RESET, HIGH);
  digitalWrite(CE, HIGH);
  digitalWrite(WR, HIGH);

  digitalWrite(RESET, LOW);           //复位
  delay(10);
  digitalWrite(RESET, HIGH);          //正常工作

  HDSP_2112_WriteChar(1, 0x30);

  for (int i = 0; i < 8; i++)
    HDSP_2112_WriteChar(i, 0x30 + i);
  delay(1000);

  HDSP_2112_WriteChar(0, 'L');
  HDSP_2112_WriteChar(1, 'i');
  HDSP_2112_WriteChar(2, 'p');
  HDSP_2112_WriteChar(3, 'o');
  HDSP_2112_WriteChar(4, 's');
  HDSP_2112_WriteChar(5, 'l');
  HDSP_2112_WriteChar(6, 't');
  HDSP_2112_WriteChar(7, ' ');
  delay(1000);

  HDSP_2112_WriteUDC();
  HDSP_2112_displayfull();
}

void loop() {
  // put your main code here, to run repeatedly:
    for (int i = 0; i < 127; i++)
    {
      for (int j = 0; j < 8; j++)
        HDSP_2112_WriteChar(j, i);
      delay(400);
    }
}

void HDSP_2112_displayfull(void)              //全部显示，亮度逐级递减
{
  for (int i = 0; i < 7 ; i++)
  {
    HDSP_2112_WriteControlWord(i, 1);
    for (int  j = 0; j < 8; j++)
    {
      HDSP_2112_WriteChar(j, 128 + 0x00);
    }
    delay(1000);
  }
}
//brightness control levels
//D2  D1  D0
//0   0   0   100%
//0   0   1   80%
//0   1   0   53%
//0   1   1   40%
//1   0   0   27%
//1   0   1   20%
//1   1   0   13%
//1   1   1   0%

//blinkk为1使能blink功能
void HDSP_2112_WriteControlWord(int brightness, int blinkk)
{
  digitalWrite(FLASH, HIGH);           //FLASH = 1
  HC595SendData_two((0x07 & brightness) | (0x10 & blinkk) | 0x08, 0x10);

  digitalWrite(CE, LOW); delay(1);
  digitalWrite(WR, LOW); delay(1);
  digitalWrite(WR, HIGH); delay(1);
  digitalWrite(CE, HIGH); delay(1);
}

void HDSP_2112_WriteChar(int x, int chr)
{
  digitalWrite(FLASH, HIGH);           //FLASH = 1
  HC595SendData_two(chr, 0x18 + x);

  digitalWrite(CE, LOW); delay(1);
  digitalWrite(WR, LOW); delay(1);
  digitalWrite(WR, HIGH); delay(1);
  digitalWrite(CE, HIGH); delay(1);
}

void HDSP_2112_WriteUDC(void)
{
  int table[7] = {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};
  digitalWrite(FLASH, HIGH);           //FLASH = 1
  HC595SendData_two(0x00, 0x00);       //16个自定义字符，存储第0b0000个。
  digitalWrite(CE, LOW); delay(1);
  digitalWrite(WR, LOW); delay(1);
  digitalWrite(WR, HIGH); delay(1);
  digitalWrite(CE, HIGH); delay(1);

  for (int i = 0; i < 7; i++)
  {
    digitalWrite(FLASH, HIGH);           //FLASH = 1
    HC595SendData_two(table[i], 0x08 + i);
    digitalWrite(CE, LOW); delay(1);
    digitalWrite(WR, LOW); delay(1);
    digitalWrite(WR, HIGH); delay(1);
    digitalWrite(CE, HIGH); delay(1);
  }
}

void HC595SendData_two(int data0, int data1)
{
  int i = 0;
  for (i = 0; i < 8; i++)
  {
    if (data0 & 0x80) {
      HC595_DS_SET;
    }
    else              {
      HC595_DS_RESET;
    }
    data0 <<= 1;
    HC595_SH_CP_SET;  HC595_SH_CP_RESET;
  }
  for (i = 0; i < 8; i++)
  {
    if (data1 & 0x80) {
      HC595_DS_SET;
    }
    else              {
      HC595_DS_RESET;
    }
    data1 <<= 1;
    HC595_SH_CP_SET;  HC595_SH_CP_RESET;
  }
  HC595_ST_CP_SET;
  HC595_ST_CP_RESET;
}
