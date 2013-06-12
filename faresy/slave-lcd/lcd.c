#include <wiringPi.h>
//#include <linux/delay>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "lcd.h"
#include "SFont.h"

#define PORTRAIT 		6
#define LANDSCAPE 		9

extern unsigned char	 	PSmallFont[1144];
extern unsigned char	 	PBigFont[3044];
extern unsigned char     	PSevenSegNumFont[2004];

int 					TP_X,TP_Y;
int 					fch,fcl,bch,bcl;
int 					orient;
unsigned short int 		x_Size,y_Size,offset;
unsigned char* 			font;
unsigned long int 		disp_x_size,disp_y_size;
unsigned long int		touch_x_left, touch_x_right, touch_y_top, touch_y_bottom;
unsigned long int		_default_orientation;
unsigned char			prec;
short					D0,D1,D2,D3,D4,D5,D6,D7,RS,CS,WR,RST,T_DOUT,T_IRQ,T_DIN,T_CLK;
int 					I2cCS,I2cRS,I2cRST,I2cGND,I2cVIN;
int 					gLCDSize;
int  					SDA1,SCL1;
unsigned int 			gTime;

void delayP(int a)
{
	int i;
	for(i=0;i<a;i++)
	{
	}
}
void DspSingleColor(unsigned char h,unsigned char l)
{
	unsigned char i,j;
	for(i=0;i<160;i++)
	{
		for(j=0;j<128;j++)
			{
			WriteData(h);
			WriteData(l);
			}
	}
}
void WriteCommandI2c(unsigned char c)
{

	digitalWrite(I2cCS,LOW);
	digitalWrite(I2cRS,LOW);
	digitalWrite(SDA1,(c&0x80));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
	digitalWrite(SDA1,(c&0x40));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
	digitalWrite(SDA1,(c&0x20));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
	digitalWrite(SDA1,(c&0x10));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
	digitalWrite(SDA1,(c&0x08));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
	digitalWrite(SDA1,(c&0x04));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
	digitalWrite(SDA1,(c&0x02));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
	digitalWrite(SDA1,(c&0x01));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
	digitalWrite(I2cCS,HIGH);

}
void SetPinNU(short P0,short P1,short P2,short P3,short P4,short P5,short P6,short P7,
			  short Prs,short Pcs,short Pwr,short Prst,short Pdout,short Pirq,short Pdin,short Pclk)
{
	D0 = P0;
	D1 = P1;
	D2 = P2;
	D3 = P3;
	D4 = P4;
	D5 = P5;
	D6 = P6;
	D7 = P7;

	RS = Prs;
	CS = Pcs;
	WR = Pwr;
	RST = Prst;

	T_DOUT = Pdout;
	T_IRQ  = Pirq;
	T_DIN  = Pdin;
	T_CLK  = Pclk;

}
void SetPinI2C(int cs,int scl,int sda,int rs,int rst,int gnd,int vin)
{
	I2cCS = cs;
	SCL1 = scl;
	SDA1 = sda;
	I2cRS = rs;
	I2cRST = rst;
	I2cGND = gnd;
	I2cVIN = vin;

}
void SetLCDSize(int a)
{
	gLCDSize = a;
}

void LCDInit(void)
{
	int i,R,G,B;
	gTime = 30;

	switch(gLCDSize)
	{
		case LCD_18:
		disp_x_size=127;
		disp_y_size=159;
		break;
		case LCD_22SPI:
		disp_x_size=175;
		disp_y_size=219;
		break;
		case LCD_22:
		disp_x_size=175;
		disp_y_size=219;
		break;
		case LCD_24:
		disp_x_size=239;
		disp_y_size=319;
		break;
		case LCD_28:
		disp_x_size=239;
		disp_y_size=319;
		break;
		default:
		break;
	}
	orient=LANDSCAPE;
	if(gLCDSize == LCD_18 || gLCDSize == LCD_22SPI)
	{
		pinMode(I2cCS,OUTPUT);
		pinMode(SCL1,OUTPUT);
		pinMode(SDA1,OUTPUT);
		pinMode(I2cRS,OUTPUT);
		pinMode(I2cRST,OUTPUT);
		pinMode(I2cGND,OUTPUT);
		pinMode(I2cVIN,OUTPUT);

		digitalWrite(I2cVIN,HIGH);
		digitalWrite(I2cGND,LOW);
		digitalWrite(I2cRST,LOW);
		delay(10);
		digitalWrite(I2cRST,HIGH);
		delay(10);
	}
	else
	{
		pinMode(D0, OUTPUT);
		pinMode(D1, OUTPUT);
		pinMode(D2, OUTPUT);
		pinMode(D3, OUTPUT);
		pinMode(D4, OUTPUT);
		pinMode(D5, OUTPUT);
		pinMode(D6, OUTPUT);
		pinMode(D7, OUTPUT);

		pinMode(CS, OUTPUT);
		pinMode(RS, OUTPUT);
		pinMode(WR, OUTPUT);
		pinMode(RST, OUTPUT);
		
		digitalWrite(RST, 1);
		delay(1);
		digitalWrite(RST, 0);
		delay(1);
		digitalWrite(RST, 1);
		digitalWrite(CS, 1);
		digitalWrite(WR, 1);
		delay(20);
	}


switch(gLCDSize)
{

	case LCD_18:
		WriteCommand(0x11); //Sleep out
		delay(120); //Delay 120ms
		//------------------------------------ST7735S Frame Rate-----------------------------------------//
		WriteCommand(0xB1);
		WriteData(0x05);
		WriteData(0x3C);
		WriteData(0x3C);
		WriteCommand(0xB2);
		WriteData(0x05);
		WriteData(0x3C);
		WriteData(0x3C);
		WriteCommand(0xB3);
		WriteData(0x05);
		WriteData(0x3C);
		WriteData(0x3C);
		WriteData(0x05);
		WriteData(0x3C);
		WriteData(0x3C);
		//------------------------------------End ST7735S Frame Rate-----------------------------------------//
		WriteCommand(0xB4); //Dot inversion
		WriteData(0x03);
		WriteCommand(0xC0);
		WriteData(0x28);
		WriteData(0x08);
		WriteData(0x04);
		WriteCommand(0xC1);
		WriteData(0XC0);
		WriteCommand(0xC2);
		WriteData(0x0D);
		WriteData(0x00);
		WriteCommand(0xC3);
		WriteData(0x8D);
		WriteData(0x2A);
		WriteCommand(0xC4);
		WriteData(0x8D);
		WriteData(0xEE);
		//---------------------------------End ST7735S Power Sequence-------------------------------------//
		WriteCommand(0xC5); //VCOM
		WriteData(0x1A);
		WriteCommand(0x36); //MX, MY, RGB mode
		WriteData(0x08);
		//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
		WriteCommand(0xE0);
		WriteData(0x04);
		WriteData(0x22);
		WriteData(0x07);
		WriteData(0x0A);
		WriteData(0x2E);
		WriteData(0x30);
		WriteData(0x25);
		WriteData(0x2A);
		WriteData(0x28);
		WriteData(0x26);
		WriteData(0x2E);
		WriteData(0x3A);
		WriteData(0x00);
		WriteData(0x01);
		WriteData(0x03);
		WriteData(0x13);
		WriteCommand(0xE1);
		WriteData(0x04);
		WriteData(0x16);
		WriteData(0x06);
		WriteData(0x0D);
		WriteData(0x2D);
		WriteData(0x26);
		WriteData(0x23);
		WriteData(0x27);
		WriteData(0x27);
		WriteData(0x25);
		WriteData(0x2D);
		WriteData(0x3B);
		WriteData(0x00);
		WriteData(0x01);
		WriteData(0x04);
		WriteData(0x13);
		//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
		WriteCommand(0x3A); //65k mode
		WriteData(0x05);
		WriteCommand(0x29); //Display on
	break;

	case LCD_22:
		WriteCommandData(0x0026,0x0084); //PT=10,GON=0, DTE=0, D=0100
		delay(40);
		WriteCommandData(0x0026,0x00B8); //PT=10,GON=1, DTE=1, D=1000
		delay(40);
		WriteCommandData(0x0026,0x00BC); //PT=10,GON=1, DTE=1, D=1100
		delay(20);
		WriteCommandData(0x0060,0x0000); //PTBA[15:8]
		WriteCommandData(0x0061,0x0006); //PTBA[7:0]
		WriteCommandData(0x0062,0x0000); //STBA[15:8]
		WriteCommandData(0x0063,0x00C8); //STBA[7:0]
		delay(20);
		WriteCommandData(0x0073,0x0070); //
		WriteCommandData(0x0040,0x0000); //
		WriteCommandData(0x0041,0x0040); //
		WriteCommandData(0x0042,0x0045); //
		WriteCommandData(0x0043,0x0001); //
		WriteCommandData(0x0044,0x0060); //
		WriteCommandData(0x0045,0x0005); //
		WriteCommandData(0x0046,0x000C); //
		WriteCommandData(0x0047,0x00D1); //
		WriteCommandData(0x0048,0x0005); //
		WriteCommandData(0x0050,0x0075); //
		WriteCommandData(0x0051,0x0001); //
		WriteCommandData(0x0052,0x0067); //
		WriteCommandData(0x0053,0x0014); //
		WriteCommandData(0x0054,0x00F2); //
		WriteCommandData(0x0055,0x0007); //
		WriteCommandData(0x0056,0x0003); //
		WriteCommandData(0x0057,0x0049); //
		delay(20);       
		WriteCommandData(0x001F,0x0003); //VRH=4.65V     VREG1??GAMMA?? 00~1E  080421    
		WriteCommandData(0x0020,0x0000); //BT (VGH~15V,VGL~-12V,DDVDH~5V)
		WriteCommandData(0x0024,0x0024); //VCOMH(VCOM High voltage3.2V)     0024/12    080421    11~40
		WriteCommandData(0x0025,0x0034); //VCOML(VCOM Low voltage -1.2V)    0034/4A    080421    29~3F 
		WriteCommandData(0x0023,0x002F); //VMF(no offset)                            
		delay(20);                        
		// Power Supply Setting
		WriteCommandData(0x0018,0x0044); //I/P_RADJ,N/P_RADJ Noraml mode 60Hz
		WriteCommandData(0x0021,0x0001); //OSC_EN='1' start osc
		WriteCommandData(0x0001,0x0000); //SLP='0' out sleep
		WriteCommandData(0x001C,0x0003); //AP=011
		WriteCommandData(0x0019,0x0006); // VOMG=1,PON=1, DK=0,
		delay(20);                            
		// Display ON Setting
		WriteCommandData(0x0026,0x0084); //PT=10,GON=0, DTE=0, D=0100
		delay(40);
		WriteCommandData(0x0026,0x00B8); //PT=10,GON=1, DTE=1, D=1000
		delay(40);
		WriteCommandData(0x0026,0x00BC); //PT=10,GON=1, DTE=1, D=1100
		delay(20);                    
		//SET GRAM AREA
		WriteCommandData(0x0002,0x0000); 
		WriteCommandData(0x0003,0x0000); 
		WriteCommandData(0x0004,0x0000);
		WriteCommandData(0x0005,0x00AF);
		WriteCommandData(0x0006,0x0000);
		WriteCommandData(0x0007,0x0000);
		WriteCommandData(0x0008,0x0000);
		WriteCommandData(0x0009,0x00DB);
		delay(20);               
		WriteCommandData(0x0016,0x0008);  
		WriteCommandData(0x0005,0x00DB);  
		WriteCommandData(0x0009,0x00AF);
		WriteCommandData(0x0017,0x0005);//COLMOD Control Register (R17h)
		WriteCommand (0x0021);//LCD_WriteCMD(GRAMWR)
		WriteCommand(0x0022);		
	break;
	case LCD_22SPI:
			
	break;
	case LCD_24:
		WriteCommandData(0x0011,0x2004);		
		WriteCommandData(0x0013,0xCC00);		
		WriteCommandData(0x0015,0x2600);	
		WriteCommandData(0x0014,0x252A);	
		//	WriteCommandData(0x14,0x002A);		
		WriteCommandData(0x0012,0x0033);		
		WriteCommandData(0x0013,0xCC04);		
		//delayms(1); 
		WriteCommandData(0x0013,0xCC06);		
		//delayms(1); 
		WriteCommandData(0x0013,0xCC4F);		
		//delayms(1); 
		WriteCommandData(0x0013,0x674F);
		WriteCommandData(0x0011,0x2003);
		//delayms(1); 	
		WriteCommandData(0x0030,0x2609);		
		WriteCommandData(0x0031,0x242C);		
		WriteCommandData(0x0032,0x1F23);		
		WriteCommandData(0x0033,0x2425);		
		WriteCommandData(0x0034,0x2226);		
		WriteCommandData(0x0035,0x2523);		
		WriteCommandData(0x0036,0x1C1A);		
		WriteCommandData(0x0037,0x131D);		
		WriteCommandData(0x0038,0x0B11);		
		WriteCommandData(0x0039,0x1210);		
		WriteCommandData(0x003A,0x1315);		
		WriteCommandData(0x003B,0x3619);		
		WriteCommandData(0x003C,0x0D00);		
		WriteCommandData(0x003D,0x000D);		
		WriteCommandData(0x0016,0x0007);		
		WriteCommandData(0x0002,0x0013);		
		WriteCommandData(0x0003,0x0003);		
		WriteCommandData(0x0001,0x0127);		
		//delayms(1); 
		WriteCommandData(0x0008,0x0303);		
		WriteCommandData(0x000A,0x000B);		
		WriteCommandData(0x000B,0x0003);   
		WriteCommandData(0x000C,0x0000);   
		WriteCommandData(0x0041,0x0000);    
		WriteCommandData(0x0050,0x0000);   
		WriteCommandData(0x0060,0x0005);    
		WriteCommandData(0x0070,0x000B);    
		WriteCommandData(0x0071,0x0000);    
		WriteCommandData(0x0078,0x0000);    
		WriteCommandData(0x007A,0x0000);   
		WriteCommandData(0x0079,0x0007);		
		WriteCommandData(0x0007,0x0051);   
		//delayms(1); 	
		WriteCommandData(0x0007,0x0053);		
		WriteCommandData(0x0079,0x0000);

		WriteCommand(0x0022);
	break;
	
	case LCD_25:
		
	break;
	case LCD_28:
		WriteCommandData(0x00E5, 0x78F0); // set SRAM internal timing
		WriteCommandData(0x0001, 0x0100); // set Driver Output Control  
		WriteCommandData(0x0002, 0x0200); // set 1 line inversion  
		WriteCommandData(0x0003, 0x1030); // set GRAM write direction and BGR=1.  
		WriteCommandData(0x0004, 0x0000); // Resize register  
		WriteCommandData(0x0008, 0x0207); // set the back porch and front porch  
		WriteCommandData(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]  
		WriteCommandData(0x000A, 0x0000); // FMARK function  
		WriteCommandData(0x000C, 0x0000); // RGB interface setting  
		WriteCommandData(0x000D, 0x0000); // Frame marker Position  
		WriteCommandData(0x000F, 0x0000); // RGB interface polarity  
		//*************Power 00On sequence ****************//  
		WriteCommandData(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB  
		WriteCommandData(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]  
		WriteCommandData(0x0012, 0x0000); // VREG1OUT voltage  
		WriteCommandData(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude  
		WriteCommandData(0x0007, 0x0001);  
		delay(200); // Dis-ch00arge capacitor power voltage  
		WriteCommandData(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB  
		WriteCommandData(0x0011, 0x0227); // Set DC1[2:0], DC0[2:0], VC[2:0]  
		delay(50); // Delay 5000ms  
		WriteCommandData(0x0012, 0x000D); // 0012  
		delay(50); // Delay 5000ms  
		WriteCommandData(0x0013, 0x1200); // VDV[4:0] for VCOM amplitude  
		WriteCommandData(0x0029, 0x000A); // 04  VCM[5:0] for VCOMH  
		WriteCommandData(0x002B, 0x000D); // Set Frame Rate  
		delay(50); // Delay 5000ms  
		WriteCommandData(0x0020, 0x0000); // GRAM horizontal Address  
		WriteCommandData(0x0021, 0x0000); // GRAM Vertical Address  
		// ----------- Adjust00 the Gamma Curve ----------//  
		WriteCommandData(0x0030, 0x0000);  
		WriteCommandData(0x0031, 0x0404);  
		WriteCommandData(0x0032, 0x0003);  
		WriteCommandData(0x0035, 0x0405);  
		WriteCommandData(0x0036, 0x0808);  
		WriteCommandData(0x0037, 0x0407);  
		WriteCommandData(0x0038, 0x0303);  
		WriteCommandData(0x0039, 0x0707);  
		WriteCommandData(0x003C, 0x0504);  
		WriteCommandData(0x003D, 0x0808);  
		//------------------ 00Set GRAM area ---------------//  
		WriteCommandData(0x0050, 0x0000); // Horizontal GRAM Start Address  
		WriteCommandData(0x0051, 0x00EF); // Horizontal GRAM End Address  
		WriteCommandData(0x0052, 0x0000); // Vertical GRAM Start Address  
		WriteCommandData(0x0053, 0x013F); // Vertical GRAM Start Address  
		WriteCommandData(0x0060, 0xA700); // Gate Scan Line  
		WriteCommandData(0x0061, 0x0001); // NDL,VLE, REV   
		WriteCommandData(0x006A, 0x0000); // set scrolling line  
		//-------------- Part00ial Display Control ---------//  
		WriteCommandData(0x0080, 0x0000);  
		WriteCommandData(0x0081, 0x0000);  
		WriteCommandData(0x0082, 0x0000);  
		WriteCommandData(0x0083, 0x0000);  
		WriteCommandData(0x0084, 0x0000);  
		WriteCommandData(0x0085, 0x0000);  
		//-------------- Pane00l Control -------------------//  
		WriteCommandData(0x0090, 0x0010);  
		WriteCommandData(0x0092, 0x0000);  
		WriteCommandData(0x0007, 0x0133);
	break;
	default:
	break;
	}
}
void WriteCommand(unsigned int c)
{
	if(gLCDSize == LCD_18)
	{
		digitalWrite(I2cCS,LOW);
		delayP(gTime);
		digitalWrite(I2cRS,LOW);
		delayP(gTime);
		digitalWrite(SDA1,((unsigned char)c&0x80));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x40));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x20));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x10));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x08));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x04));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x02));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x01));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		delayP(gTime);
		digitalWrite(I2cCS,HIGH);
		delayP(gTime);
	}
	else
	{

		digitalWrite(RS,0);
		delayP(gTime);
		digitalWrite(CS,0);
		delayP(gTime);
		if(c>>8&0x01)
		{
			digitalWrite(D0,1);
		}
		else
		{
			digitalWrite(D0,0);	
		}
		if(c>>8&0x02)
		{
			digitalWrite(D1,1);	
		}
		else
		{
			digitalWrite(D1,0);	
		}
		if(c>>8&0x04)
		{
			digitalWrite(D2,1);	
		}
		else
		{
			digitalWrite(D2,0);	
		}
		if(c>>8&0x08)
		{
			digitalWrite(D3,1);	
		}
		else
		{
			digitalWrite(D3,0);	
		}
		if(c>>8&0x10)
		{
			digitalWrite(D4,1);	
		}
		else
		{
			digitalWrite(D4,0);	
		}
		if(c>>8&0x20)
		{
			digitalWrite(D5,1);	
		}
		else
		{
			digitalWrite(D5,0);	
		}
		if(c>>8&0x40)
		{
			digitalWrite(D6,1);	
		}
		else
		{
			digitalWrite(D6,0);	
		}
		if(c>>8&0x80)
		{
			digitalWrite(D7,1);	
		}
		else
		{
			digitalWrite(D7,0);	
		}
		delayP(gTime);
		digitalWrite(WR,0);
		delayP(gTime);
		digitalWrite(WR,1);
		delayP(gTime);
		if(c&0x01)
		{
			digitalWrite(D0,1);	
		}
		else
		{
			digitalWrite(D0,0);	
		}
		if(c&0x02)
		{
			digitalWrite(D1,1);	
		}
		else
		{
			digitalWrite(D1,0);	
		}
		if(c&0x04)
		{
			digitalWrite(D2,1);	
		}
		else
		{
			digitalWrite(D2,0);	
		}
		if(c&0x08)
		{
			digitalWrite(D3,1);	
		}
		else
		{
			digitalWrite(D3,0);	
		}
		if(c&0x10)
		{
			digitalWrite(D4,1);	
		}
		else
		{
			digitalWrite(D4,0);	
		}
		if(c&0x20)
		{
			digitalWrite(D5,1);	
		}
		else
		{
			digitalWrite(D5,0);	
		}
		if(c&0x40)
		{
			digitalWrite(D6,1);	
		}
		else
		{
			digitalWrite(D6,0);	
		}
		if(c&0x80)
		{
			digitalWrite(D7,1);	
		}
		else
		{
			digitalWrite(D7,0);	
		}
		delayP(gTime);
		digitalWrite(WR,0);
		delayP(gTime);
		digitalWrite(WR,1);
		delayP(gTime);
		digitalWrite(CS,1);
  }
}
void WriteData(unsigned int c)
{
	if(gLCDSize == LCD_18)
	{

		digitalWrite(I2cCS,LOW);
		delayP(gTime);
		digitalWrite(I2cRS,HIGH);
		delayP(gTime);
		digitalWrite(SDA1,((unsigned char)c&0x80));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x40));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x20));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x10));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x08));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x04));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x02));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		digitalWrite(SDA1,((unsigned char)c&0x01));digitalWrite(SCL1,LOW);digitalWrite(SCL1,HIGH);
		delayP(gTime);
		digitalWrite(I2cCS,HIGH);
		delayP(gTime);
	}
	else
	{
		digitalWrite(RS,1);
		delayP(gTime);
		digitalWrite(CS,0);
		delayP(gTime);
		if(c>>8&0x01)
		{
			digitalWrite(D0,1);	
		}
		else
		{
			digitalWrite(D0,0);	
		}
		if(c>>8&0x02)
		{
			digitalWrite(D1,1);	
		}
		else
		{
			digitalWrite(D1,0);	
		}
		if(c>>8&0x04)
		{
			digitalWrite(D2,1);	
		}
		else
		{
			digitalWrite(D2,0);	
		}
		if(c>>8&0x08)
		{
			digitalWrite(D3,1);	
		}
		else
		{
			digitalWrite(D3,0);	
		}
		if(c>>8&0x10)
		{
			digitalWrite(D4,1);	
		}
		else
		{
			digitalWrite(D4,0);	
		}
		if(c>>8&0x20)
		{
			digitalWrite(D5,1);	
		}
		else
		{
			digitalWrite(D5,0);	
		}
		if(c>>8&0x40)
		{
			digitalWrite(D6,1);	
		}
		else
		{
			digitalWrite(D6,0);	
		}
		if(c>>8&0x80)
		{
			digitalWrite(D7,1);	
		}
		else
		{
			digitalWrite(D7,0);	
		}
		digitalWrite(WR,0);
		delayP(gTime);
		digitalWrite(WR,1);
		delayP(gTime);
		if(c&0x01)
		{
			digitalWrite(D0,1);	
		}
		else
		{
			digitalWrite(D0,0);	
		}
		if(c&0x02)
		{
			digitalWrite(D1,1);	
		}
		else
		{
			digitalWrite(D1,0);	
		}
		if(c&0x04)
		{
			digitalWrite(D2,1);	
		}
		else
		{
			digitalWrite(D2,0);	
		}
		if(c&0x08)
		{
			digitalWrite(D3,1);	
		}
		else
		{
			digitalWrite(D3,0);	
		}
		if(c&0x10)
		{
			digitalWrite(D4,1);	
		}
		else
		{
			digitalWrite(D4,0);	
		}
		if(c&0x20)
		{
			digitalWrite(D5,1);	
		}
		else
		{
			digitalWrite(D5,0);	
		}
		if(c&0x40)
		{
			digitalWrite(D6,1);	
		}
		else
		{
			digitalWrite(D6,0);	
		}
		if(c&0x80)
		{
			digitalWrite(D7,1);	
		}
		else
		{
			digitalWrite(D7,0);	
		}
		digitalWrite(WR,0);
		delayP(gTime);
		digitalWrite(WR,1);
		delayP(gTime);
		digitalWrite(CS,1);
	}

}
void WriteCommandData(unsigned int cmd,unsigned int dat)
{
	WriteCommand(cmd);
//	delayP(1000);
//	delay(1);
	WriteData(dat);
}

void SetXY(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1)
{


	switch(gLCDSize)
	{
	case LCD_18:
	
	swap(x0, y0);
	swap(x1, y1);
	y0=disp_y_size-y0;
	y1=disp_y_size-y1;
	swap(y0, y1);
	WriteCommand(0x2b);
	WriteData(0x00);
	WriteData(y0);
	WriteData(0x00);
	WriteData(y1);
	WriteCommand(0x2a);
	WriteData(0x00);
	WriteData(x0);
	WriteData(0x00);
	WriteData(x1);
	WriteCommand(0x2c);
	break;
	case LCD_22:
	swap(x0, y0);
	swap(x1, y1);
	y0=disp_y_size-y0;
	y1=disp_y_size-y1;
	swap(y0, y1);

	WriteCommandData(0x0002,0x0000);
	WriteCommandData(0x0003,x0);
	WriteCommandData(0x0004,0x0000);
	WriteCommandData(0x0005,y0);
	WriteCommandData(0x0006,0x0000);
	WriteCommandData(0x0007,x1);
	WriteCommandData(0x0008,0x0000);
	WriteCommandData(0x0009,y1);
	WriteCommand(0x0022);
	break;
	case LCD_24:
	swap(x0, y0);
	swap(x1, y1);
	y0=disp_y_size-y0;
	y1=disp_y_size-y1;
	swap(y0, y1);

	WriteCommandData(0x0046,(x1<<8)|x0);
	WriteCommandData(0x0047,y1);
	WriteCommandData(0x0048,y0);
	WriteCommandData(0x0020,x0);
	WriteCommandData(0x0021,y0);
	WriteCommand(0x0022);
	break;
	case LCD_28:
	swap(x0, y0);
	swap(x1, y1);
	y0=disp_y_size-y0;
	y1=disp_y_size-y1;
	swap(y0, y1);

	WriteCommandData(0x20,x0);
	WriteCommandData(0x21,y0);
	WriteCommandData(0x50,x0);
	WriteCommandData(0x52,y0);
	WriteCommandData(0x51,x1);
	WriteCommandData(0x53,y1);
	WriteCommand(0x22);
	break;
	default:
	break;
	}
}
void Pant(unsigned int color)
{
   int i,j;
   //SetXY(0,0,disp_x_size,disp_y_size);
   SetXY(0,0,disp_y_size,disp_x_size);
   for(i=0;i<disp_x_size;i++)
	{
	  for (j=0;j<disp_y_size;j++)
	    {
		if(gLCDSize == LCD_18)
		{
		WriteData(color);
		WriteData(color>>8);
		}
		else
		{
		WriteData(color);
		}
		//WriteData(0x00);
		//WriteData(0x00);
	    }

	}
}
void fillScr(unsigned int color)
{
	int i,j;
	SetXY(0,0,disp_y_size,disp_x_size);

  	for(i=0;i<disp_x_size;i++)
	{
	  for (j=0;j<disp_y_size;j++)
	    {	
		if(gLCDSize == LCD_18)
		{
		WriteData(color);
		WriteData(color>>8);
		}
		else
		{
        	WriteData(color);
		}
	    }
	}
}
void fillScrRGB(int r, int g, int b)
{
	int color;
	
	color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
	fillScr(color);
}
void clrScr()
{
	unsigned int i,j;
	SetXY(0,0,disp_y_size,disp_x_size);
	for(i=0;i<240;i++)
	{
	    for(j=0;j<320;j++)
	   {
		if(gLCDSize == LCD_18)
		{
		WriteData(0x0000);
		WriteData(0x0000>>8);
		}
		else
		{    
          	WriteData(0x0000);
		}
	   }
	}
	
}
void LcdOff()
{
	WriteCommand(0x28);
}

void LcdOn()
{
	WriteCommand(0x29);
}

void clrXY()
{
//	if (orient==PORTRAIT)
	if(0)
	{
		SetXY(0,0,disp_x_size,disp_y_size);
	}
	else
	{
		SetXY(0,0,disp_y_size,disp_x_size);
	}
}
void setColor(long int color)
{	
	fch=(color>>8);
	fcl=(color & 0xFF);
}
void setColorRGB(int r, int g, int b)
{
	fch=((r&248)|g>>5);
	fcl=((g&28)<<3|b>>3);
}
void setBackColor(int color)
{
	bch=(int)(color>>8);
	bcl=(int)(color & 0xFF);
}
void setBackColorRGB(int r, int g, int b)
{
	bch=((r&248)|g>>5);
	bcl=((g&28)<<3|b>>3);
}
void drawRect(int x1, int y1, int x2, int y2)
{
	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1);
}
void drawHLine(int x, int y, int l)
{
	int i,j;
	if (l<0)
	{
		l = -l;
		y -= l;
	}
	
	SetXY(x,y,x+l,y);

	for (j=x;j<x+l;j++)
	{
		if(gLCDSize == LCD_18)
		{
		WriteData((fch<<8)|fcl);
		WriteData(((fch<<8)|fcl)>>8);
		}
		else
		{
		WriteData((fch<<8)|fcl);
		}
	}
	
	clrXY();
}

void drawVLine(int x, int y, int l)
{
	int i,j;
	if (l<0)
	{
		l = -l;
		y -= l;
	}
	
	SetXY(x,y,x,y+l);
	for (j=y;j<y+l;j++)
	{	
		if(gLCDSize == LCD_18)
		{
		WriteData((fch<<8)|fcl);
		WriteData(((fch<<8)|fcl)>>8);
		}
		else
		{
		WriteData((fch<<8)|fcl);
		}
	}

	clrXY();
}
void drawLine(int x1, int y1, int x2, int y2)
{
	int t,col,row;
	unsigned int dx,dy;
	short int xstep,ystep;
	
	if (y1==y2)
	{
		drawHLine(x1, y1, x2-x1);
	}
	else if (x1==x2)
	{
		drawVLine(x1, y1, y2-y1);
	}
	else
	{
		dx = (x2 > x1 ? x2 - x1 : x1 - x2);
		xstep =  x2 > x1 ? 1 : -1;
		dy = (y2 > y1 ? y2 - y1 : y1 - y2);
		ystep =  y2 > y1 ? 1 : -1;
		col = x1, row = y1;

		if (dx < dy)
		{
			t = - (dy >> 1);
			while (1)
			{
				SetXY (col, row, col, row);
				if(gLCDSize == LCD_18)
				{
				WriteData((fch<<8)|fcl);
				WriteData(((fch<<8)|fcl)>>8);
				}
				else
				{
				WriteData((fch<<8)|fcl);
				}
				if (row == y2)
				return;
				row += ystep;
				t += dx;
				if (t >= 0)
				{
					col += xstep;
					t   -= dy;
				}
			} 
		}
		else
		{
			t = - (dx >> 1);
			while (1)
			{
				SetXY (col, row, col, row);
				if(gLCDSize == LCD_18)
				{
				WriteData((fch<<8)|fcl);
				WriteData(((fch<<8)|fcl)>>8);
				}
				else
				{
				WriteData ((fch<<8)|fcl);
				}
				if (col == x2)
					return;
				col += xstep;
				t += dy;
				if (t >= 0)
				{
					row += ystep;
					t   -= dx;
				}
			} 
		}
	}
	clrXY();
}
void drawRoundRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		swap(x1, x2);
	}
	if (y1>y2)
	{
		swap(y1, y2);
	}
	if ((x2-x1)>4 && (y2-y1)>4)
	{
		drawPixel(x1+1,y1+1);
		drawPixel(x2-1,y1+1);
		drawPixel(x1+1,y2-1);
		drawPixel(x2-1,y2-1);
		drawHLine(x1+2, y1, x2-x1-4);
		drawHLine(x1+2, y2, x2-x1-4);
		drawVLine(x1, y1+2, y2-y1-4);
		drawVLine(x2, y1+2, y2-y1-4);
	}
}
void drawPixel(int x, int y)
{
	SetXY(x, y, x, y);
//	SetXY(y,x,y,x);
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	setPixel((fch<<8)|fcl);
	}
}
void setPixel(int color)
{
	if(gLCDSize == LCD_18)
	{
	WriteData(color);
	WriteData(color>>8);
	}
	else
	{
	WriteData(color);
	}
}
void fillRect(int x1, int y1, int x2, int y2)
{
	int tmp;
	int i;
	if (x1>x2)
	{
		swap(x1, x2);
	}
	if (y1>y2)
	{
		swap(y1, y2);
	}
	SetXY(x1, y1, x2, y2);
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
	
	if(gLCDSize == LCD_18)
	{
	WriteData(((long)(x2-x1)+1)*((long)(y2-y1)+1));
	WriteData(((long)(x2-x1)+1)*((long)(y2-y1)+1)>>8);
	}
	else
	{
	WriteData(((long)(x2-x1)+1)*((long)(y2-y1)+1));
	}
	if (orient==PORTRAIT)
//	if(0)
	{
		for (i=0; i<(((y2-y1)/2)+1); i++)
		{
			drawHLine(x1, y1+i, x2-x1);
			drawHLine(x1, y2-i, x2-x1);
		//	delay(100);
		}
	}
	else
	{
		for (i=0; i<((x2-x1)/2)+1; i++)
		{
			drawVLine(x1+i, y1, y2-y1);
			drawVLine(x2-i, y1, y2-y1);
		//	delay(100);
		}
	}
	
	
}
void fillRoundRect(int x1, int y1, int x2, int y2)
{
	int tmp;
	int i;
	if (x1>x2)
	{
		swap(x1, x2);
	}
	if (y1>y2)
	{
		swap(y1, y2);
	}

	if ((x2-x1)>4 && (y2-y1)>4)
	{
		for (i=0; i<((y2-y1)/2)+1; i++)
		{
			switch(i)
			{
			case 0:
				drawHLine(x1+2, y1+i, x2-x1-4);
				drawHLine(x1+2, y2-i, x2-x1-4);
				//delay(100);
				break;
			case 1:
				drawHLine(x1+1, y1+i, x2-x1-2);
				drawHLine(x1+1, y2-i, x2-x1-2);
				//delay(100);
				break;
			default:
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
	}
}
void drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;

	SetXY(x, y + radius, x, y + radius);
	
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
	SetXY(x, y - radius, x, y - radius);
	
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
	SetXY(x + radius, y, x + radius, y);
	
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
	SetXY(x - radius, y, x - radius, y);
	
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
 
	while(x1 < y1)
	{
		if(f >= 0) 
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;    
		SetXY(x + x1, y + y1, x + x1, y + y1);
		
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
		SetXY(x - x1, y + y1, x - x1, y + y1);
		
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
		SetXY(x + x1, y - y1, x + x1, y - y1);
		
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
		SetXY(x - x1, y - y1, x - x1, y - y1);
		
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
		SetXY(x + y1, y + x1, x + y1, y + x1);
		
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
		SetXY(x - y1, y + x1, x - y1, y + x1);
		
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
		SetXY(x + y1, y - x1, x + y1, y - x1);
		
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
		SetXY(x - y1, y - x1, x - y1, y - x1);
	
	if(gLCDSize == LCD_18)
	{
	WriteData((fch<<8)|fcl);
	WriteData(((fch<<8)|fcl)>>8);
	}
	else
	{
	WriteData((fch<<8)|fcl);
	}
	}
	clrXY();
}
void fillCircle(int x, int y, int radius)
{
	int y1,x1;
	for(y1=-radius; y1<=0; y1++) 
		for(x1=-radius; x1<=0; x1++)
			if(x1*x1+y1*y1 <= radius*radius) 
			{
				drawHLine(x+x1, y+y1, 2*(-x1));
				drawHLine(x+x1, y-y1, 2*(-x1));
				break;
			}
}
void setFont(unsigned short int mxsize,unsigned short int mysize,unsigned short int moffset)
{
	x_Size=mxsize;
	y_Size=mysize;
	offset=moffset;
}

unsigned short int PgmRead(unsigned short int *p)
{
	unsigned short int data,i;
	data = *p;
	return data;
}
void printChar(unsigned char c, int x, int y)
{
	unsigned char i;
	unsigned char ch;
	unsigned int j;
	unsigned int temp; 
	int zz;

	if(x_Size == 16)
	{
		font = &PBigFont[0];
	}
	else if(x_Size == 32)
	{
		font = &PSevenSegNumFont[0];
	}
	else
	{
		font = &PSmallFont[0];
	}

	if (orient==PORTRAIT)
//	if(0)
	{
		SetXY(x,y,x+x_Size-1,y+y_Size-1); 
		temp=((c-offset)*((x_Size/8)*y_Size))+4;
		for(j=0;j<((x_Size/8)*y_Size);j++)
		{
			ch =(unsigned char) *(&font[temp]);
			for(i=0;i<8;i++)
			{   
				if((ch&(1<<(7-i)))!=0)   
				{
					setPixel((fch<<8)|fcl);
				} 
				else
				{
					setPixel((bch<<8)|bcl);
				}   
			}
			temp++;
		}
	}
	else
	{
		temp=((c-offset)*((x_Size/8)*y_Size))+4;

		for(j=0;j<((x_Size/8)*y_Size);j+=(x_Size/8))
		{
			SetXY(x,y+(j/(x_Size/8)),x+x_Size-1,y+(j/(x_Size/8)));
			for (zz=(x_Size/8)-1; zz>=0; zz--)
			{
				ch = (unsigned char)*(&font[temp+zz]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<i))!=0)   
					{
						setPixel((fch<<8)|fcl);
					} 
					else
					{
						setPixel((bch<<8)|bcl);
					}   
				}
			}
			temp+=(x_Size/8);
		}
	}
	clrXY();

}
void rotateChar(unsigned char c, int x, int y, int pos, int deg)
{
	unsigned char i,j,ch;
	unsigned int temp; 
	unsigned int newx,newy;
	int zz;
	double radian;
	radian=deg*0.0175;  

	//font = &PSmallFont[0];
	if(x_Size == 16)
	{
		font = &PBigFont[0];
	}
	else if(x_Size == 32)
	{
		font = &PSevenSegNumFont[0];
	}
	else
	{
		font = &PSmallFont[0];
	}
	temp=((c-offset)*((x_Size/8)*y_Size))+4;
	for(j=0;j<y_Size;j++) 
	{
		for (zz=0; zz<(x_Size/8); zz++)
		{
			//ch=pgm_read_byte(&cfont.font[temp+zz]); 
			ch=*(&font[temp+zz]);
			for(i=0;i<8;i++)
			{   
				newx=x+(((i+(zz*8)+(pos*x_Size))*cos(radian))-((j)*sin(radian)));
				newy=y+(((j)*cos(radian))+((i+(zz*8)+(pos*x_Size))*sin(radian)));

				SetXY(newx,newy,newx+1,newy+1);
				if((ch&(1<<(7-i)))!=0)
				{
					setPixel((fch<<8)|fcl);
				}
				else 
				{
					setPixel((bch<<8)|bcl);
				} 
			}
		}
		temp+=(x_Size/8);
	}
	clrXY();
}
void print(char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (orient==PORTRAIT)
//	if(0)
	{
	if (x==RIGHT)
		x=(disp_x_size+1)-(stl*x_Size);
	if (x==CENTER)
		x=((disp_x_size+1)-(stl*x_Size))/2;
	}
	else
	{
	if (x==RIGHT)
		x=(disp_y_size+1)-(stl*x_Size);
	if (x==CENTER)
		x=((disp_y_size+1)-(stl*x_Size))/2;
	}

	for (i=0; i<stl; i++)
		if (deg==0)
			printChar(*st++, x + (i*(x_Size)), y);
		else
			rotateChar(*st++, x, y, i, deg);
}
/*void print(String st, int x, int y, int deg)
{
	char buf[st.length()+1];
	st.toCharArray(buf, st.length()+1);
	printP(buf, x, y, deg);
}*/
unsigned char* getFont()
{
	return font;
}

unsigned int getFontXsize()
{
	return x_Size;
}

unsigned int getFontYsize()
{
	return y_Size;
}
int getDisplayXSize()
{
	if (orient==PORTRAIT)
//	if(0)
		return disp_x_size+1;
	else
		return disp_y_size+1;
}

int getDisplayYSize()
{
	if (orient==PORTRAIT)
//	if(0)
		return disp_y_size+1;
	else
		return disp_x_size+1;
}
void printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	int neg=0;
	int c=0, f=0;
	int i;
	
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
			st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=1;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg))
		{
			for (i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y,0);
}
void printNumF(double num, unsigned char dec, int x, int y, char divider, int length, char filler)
{
	char buf[25];
	char st[27];
	int neg=0;
	int c=0, f=0;
	int c2;
	long mult;
	unsigned long inum;
	int i,j;
	
	if (dec<1)
	dec=1;
	if (dec>5)
	dec=5;
	  
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-2-dec); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=divider;
			for (i=0; i<dec; i++)
				st[c+2+i]=48;
			st[c+2+dec]=0;
		}
		else
		{
			st[0]=48;
			st[1]=divider;
			for (i=0; i<dec; i++)
				st[2+i]=48;
			st[2+dec]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=1;
			num=-num;
		}
	  
		mult = 1;
		for (j = 0; j < dec; j++)
			mult = mult * 10;
		inum=(long)(num*mult + 0.5);
	  
		while (inum>0)
		{
			buf[c]=48+(inum % 10);
			c++;
			inum=(inum-(inum % 10))/10;
		}
		if ((num<1) && (num>0))
		{
			buf[c]=48;
			c++;
		}
		while (c<(dec+1))
		{
			buf[c]=48;
			c++;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg-1))
		{
			for (i=0; i<(length-c-neg-1); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		c2=neg;
		for (i=0; i<c; i++)
		{
			st[c2+f]=buf[c-i-1];
			c2++;
			if ((c-(c2-neg))==dec)
			{
				st[c2+f]=divider;
				c2++;
			}
		}
		st[c2+f]=0;
	}

	print(st,x,y,0);
}
void drawBitmap(int x, int y, int sx, int sy, unsigned int* data, int deg, int rox, int roy)
{
	unsigned int col;
	int tx, ty, newx, newy;
	unsigned char r, g, b;
	double radian;
	radian=deg*0.0175;  

	if (deg==0)
		drawBitmapP(x, y, sx, sy, data,0);
	else
	{
		for (ty=0; ty<sy; ty++)
			for (tx=0; tx<sx; tx++)
			{
				col=*(&data[(ty*sx)+tx]);

				newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin(radian)));
				newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin(radian)));

				SetXY(newx, newy, newx, newy);
				WriteData((col<<8)|col);
			}
	}
	clrXY();
}
void drawBitmapP(int x, int y, int sx, int sy, unsigned int* data, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;
	unsigned char r, g, b;

	if (scale==1)
	{
		if (orient==PORTRAIT)
//		if(0)
		{
			SetXY(x, y, x+sx-1, y+sy-1);
			for (tc=0; tc<(sx*sy); tc++)
			{
				//col=pgm_read_word(&data[tc]);
				col=*(&data[tc]);
				WriteData((col>>8)|col);
			}
		}
		else
		{
			for (ty=0; ty<sy; ty++)
			{
				SetXY(x, y+ty, x+sx-1, y+ty);
				for (tx=sx; tx>=0; tx--)
				{
					//col=pgm_read_word(&data[(ty*sx)+tx]);
					col=*(&data[(ty*sx)+tx]);
					WriteData((col>>8)|col);
				}
			}

		}
	}
	else
	{
		if (orient==PORTRAIT)
//		if(0)
		{
			for (ty=0; ty<sy; ty++)
			{
				SetXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
				for (tsy=0; tsy<scale; tsy++)
					for (tx=0; tx<sx; tx++)
					{
						//col=pgm_read_word(&data[(ty*sx)+tx]);
						col=*(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
						WriteData((col>>8)|col);
					}
			}
		}
		else
		{
			for (ty=0; ty<sy; ty++)
			{
				for (tsy=0; tsy<scale; tsy++)
				{
					SetXY(x, y+(ty*scale)+tsy, x+((sx*scale)-1), y+(ty*scale)+tsy);
					for (tx=sx; tx>=0; tx--)
					{
						//col=pgm_read_word(&data[(ty*sx)+tx]);
						col=*(&data[(ty*sx)+tx]);
						WriteData((col>>8)|col);
					}
				}
		}
	}
	clrXY();
	}
}
void Touch_Init(void)
{
//	orient					= PORTRAIT;
	_default_orientation		= CAL_S>>31;
	touch_x_left			= (CAL_X>>14) & 0x3FFF;
	touch_x_right			= CAL_X & 0x3FFF;
	touch_y_top				= (CAL_Y>>14) & 0x3FFF;
	touch_y_bottom			= CAL_Y & 0x3FFF;
	disp_x_size				= (CAL_S>>12) & 0x0FFF;
	disp_y_size				= CAL_S & 0x0FFF;
	prec					= 10;

	pinMode(T_CLK,  OUTPUT);
//	pinMode(T_CS,   OUTPUT);
	pinMode(T_DIN,  OUTPUT);
	pinMode(T_DOUT, INPUT);
	pinMode(T_IRQ,  INPUT);

//	digitalWrite(T_CS,  HIGH);
	digitalWrite(T_CLK, HIGH);
	digitalWrite(T_DIN, HIGH);
	digitalWrite(T_CLK, HIGH);
}

void Touch_WriteData(unsigned char data)
{
	unsigned char temp;
	unsigned char nop;
	unsigned char count;

	temp=data;
	digitalWrite(T_CLK,0);

	for(count=0; count<8; count++)
	{
		if(temp & 0x80)
		{
			digitalWrite(T_DIN, 1);
		}
		else
		{
			digitalWrite(T_DIN, 0);
		}

		temp = temp << 1;
		digitalWrite(T_CLK, 0);
		nop++;
		digitalWrite(T_CLK, 1);
		nop++;
	}

}
unsigned int Touch_ReadData(void)
{
	unsigned char nop;
	unsigned int data = 0;
	unsigned char count;

		for(count=0; count<12; count++)
		{
			data <<= 1;
			digitalWrite(T_CLK, 1);
			nop++;
			digitalWrite(T_CLK, 0);
			nop++;
			if (digitalRead(T_DOUT))
			{
				data++;
			}
		}

	return(data);
}
void Touch_Read(void)
{
	unsigned long tx=0, temp_x=0;
	unsigned long ty=0, temp_y=0;
	int datacount=0;
	int i;

	//digitalWrite(T_CS,0);
	for (i=0; i<prec; i++)
	{
		Touch_WriteData(0x90);
		digitalWrite(T_CLK,1);
		digitalWrite(T_CLK,0);
		temp_x=Touch_ReadData();
		Touch_WriteData(0xD0);
		digitalWrite(T_CLK,1);
		digitalWrite(T_CLK,0);
		temp_y=Touch_ReadData();
			ty+=temp_x;
			tx+=temp_y;
			datacount++;

	}

//	digitalWrite(T_CS,1);

	if (datacount>0)
	{
		if (orient == _default_orientation)
		{
			TP_X=tx/datacount;
			TP_Y=ty/datacount;
		}
		else
		{
			TP_X=ty/datacount;
			TP_Y=tx/datacount;
		}
	}
	else
	{
		TP_X=-1;
		TP_Y=-1;
	}

}

char Touch_DataAvailable(void)
{
	char avail;

	pinMode(T_IRQ,  INPUT);
	avail = !digitalRead(T_IRQ);
	//pinMode(T_IRQ,  OUTPUT);

	return avail;
}
int Touch_GetX(void)
{
	long c;

	//if (orient == _default_orientation)
	if(0)
	{
		c = (long)((long)(TP_X - touch_x_left) * (disp_x_size)) / (long)(touch_x_right - touch_x_left);
		//if (c<0)
		//	c = 0;
		//if (c>disp_x_size)
		//	c = disp_x_size;
	}
	else
	{
		if (_default_orientation == PORTRAIT)
			c = (long)((long)(TP_X - touch_y_top) * (-disp_y_size)) / (long)(touch_y_bottom - touch_y_top) + (long)(disp_y_size);
		else
			c = (long)((long)(TP_X - touch_y_top) * (disp_y_size)) / (long)(touch_y_bottom - touch_y_top);
		//if (c<0)
		//	c = 0;
		//if (c>disp_y_size)
		//	c = disp_y_size;
	}
	if(gLCDSize == LCD_28)
	{
	c = (c-42)*320/(326-42);
	}
	else 
	{
	c = (320-c);
	}
	return c;
}
int Touch_GetY(void)
{
	int c;

	//if (orient == _default_orientation)
	if(1)
	{
		c = (long)((long)(TP_Y - touch_y_top) * (disp_y_size)) / (long)(touch_y_bottom - touch_y_top);
		//if (c<0)
		//	c = 0;
		//if (c>disp_y_size)
		//	c = disp_y_size;
	}
	else
	{
		if (_default_orientation == PORTRAIT)
			c = (long)((long)(TP_Y - touch_x_left) * (disp_x_size)) / (long)(touch_x_right - touch_x_left);
		else
			c = (long)((long)(TP_Y - touch_x_left) * (-disp_x_size)) / (long)(touch_x_right - touch_x_left) + (long)(disp_x_size);
		//if (c<0)
		//	c = 0;
		//if (c>disp_x_size)
		//	c = disp_x_size;
	}
	if(gLCDSize == LCD_28)
	{
	c = (c-29)*240/(325-29);
	}
	else  
	{
	c = (c*240/320);
	}

	return c;
}
void setPrecision(unsigned char precision)
{
  prec = 10;
#if 0
	switch (precision)
	{
		case PREC_LOW:
			prec=1;
			break;
		case PREC_MEDIUM:
			prec=10;
			break;
		case PREC_HI:
			prec=25;
			break;
		case PREC_EXTREME:
			prec=100;
			break;
		default:
			prec=10;
			break;
	}
#endif
}

