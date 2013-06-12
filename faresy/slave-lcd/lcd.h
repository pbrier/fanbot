/*#define RS	15
#define CS	16
#define WR	1
#define RST	4

#define D0	8
#define D1	9
#define D2	7
#define D3	0
#define D4	2
#define D5	3
#define D6	12
#define D7	13

#define T_DOUT	5 
#define T_IRQ	6
#define T_DIN	10
#define T_CLK	11
*/
#define LCD_18	18
#define LCD_22	22
#define LCD_22SPI	23
#define LCD_24	24
#define LCD_25	25
#define LCD_28	28

#define swap(i,j)	{int t;t=i;i=j;j=t;}
#define CAL_X 				0x00378F66UL
#define CAL_Y 				0x03C34155UL
#define CAL_S 				0x000EF13FUL

#define RIGHT 			9999
#define CENTER 			9998
void delayP(int a);
void DspSignalColor(unsigned char h,unsigned char l);
void WriteCommandI2c(unsigned char c);
void LCDInit(void);
void SetPinNU(short P0,short P1,short P2,short P3,short P4,short P5,short P6,short P7,
			  short Prs,short Pcs,short Pwr,short Prst,short Pdout,short Pirq,short Pdin,short Pclk);
void SetPinI2C(int cs,int scl,int sda,int rs,int rst,int gnd,int vin);
void SetLCDSize(int a);
//void swap(unsigned int i,unsigned int j);
void WriteCommand(unsigned int c);
void WriteData(unsigned int c);
void WriteCommandData(unsigned int cmd,unsigned int dat);
void SetXY(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1);
void Pant(unsigned int color);
void fillScr(unsigned int color);
void fillScrRGB(int r, int g, int b);
void clrScr();
void LcdOff();
void LcdOn();
void clrXY();
void setColor(long int color);
void setColorRGB(int r, int g, int b);
void setBackColor(int color);
void setBackColorRGB(int r, int g, int b);
void drawRect(int x1, int y1, int x2, int y2);
void drawHLine(int x, int y, int l);
void drawVLine(int x, int y, int l);
void drawLine(int x1, int y1, int x2, int y2);
void drawRoundRect(int x1, int y1, int x2, int y2);
void drawPixel(int x, int y);
void setPixel(int color);
void fillRect(int x1, int y1, int x2, int y2);
void fillRoundRect(int x1, int y1, int x2, int y2);
void drawCircle(int x, int y, int radius);
void fillCircle(int x, int y, int radius);
unsigned short int PgmRead(unsigned short int *p);
void printChar(unsigned char c, int x, int y);
void setFont(unsigned short int mxsize,unsigned short int mysize,unsigned short int moffset);
void Touch_Init(void);
void Touch_WriteData(unsigned char data);
unsigned int Touch_ReadData(void);
void Touch_Read(void);
char autorelease(void);
int Touch_GetX(void);
int Touch_GetY(void);
void rotateChar(unsigned char c, int x, int y, int pos, int deg);
void print(char *st, int x, int y, int deg);
//void print(String st, int x, int y, int deg);
unsigned char* getFont();
unsigned int getFontXsize();
unsigned int getFontYsize();
int getDisplayXSize();
int getDisplayYSize();
void printNumI(long num, int x, int y, int length, char filler);
void printNumF(double num, unsigned char dec, int x, int y, char divider, int length, char filler);
void drawBitmap(int x, int y, int sx, int sy, unsigned int* data, int deg, int rox, int roy);
void drawBitmapP(int x, int y, int sx, int sy, unsigned int* data, int scale);
void setPrecision(unsigned char precision);
