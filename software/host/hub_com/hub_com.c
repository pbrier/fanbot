//
// SerialSend.c - This program sends text via serial port
// Written by Ted Burke - last updated 16-4-2013
//
// The text to send is specified as command line arguments.
// By default, the highest available serial port is used.
// The default baud rate is 38400 baud.
//
// To compile with MinGW:
//
//      gcc -o SerialSend.exe SerialSend.c
//
// To compile with cl, the Microsoft compiler:
//
//      cl SerialSend.c
//
// To run (this example sends the characters "S365 E120"):
//
//      SerialSend.exe "S356 E120"
//
  
#include <windows.h>
#include <stdio.h>
  

enum HubOpcodes { 
  REQUEST_ID,
  TAG_ID,
  PLAY_FRAME,
  LED_FRAME,
  POS_FRAME,
  REQUEST_STATUS,
  CONFIG_FRAME,
  ID_REPORT,
  STATUS_REPORT,
  RESET = 0xDEAD,
};
  
// Send data frame with the syntax '#' '#' [Opcode LSB] [Opcode MSB] [Length LSB] [Length MSB] [DATA 0] ... [Data N] [Checksum LSB] [Checksum MSB] 
  
int SendData(HANDLE hSerial, unsigned short int opcode, unsigned char *buffer, unsigned short int length)
{
  unsigned short int checksum = 0;
  unsigned char buf[6];
  DWORD tx_bytes = 0;
  
  buf[0] = '#';
  buf[1] = '#';
  buf[2] = opcode & 0xFF;
  buf[3] = opcode >> 8;
  buf[4] = length & 0xFF;
  buf[5] = length >> 8;
  
  for(int i=2; i<6; i++)
    checksum += buf[i];
  
  WriteFile(hSerial, buf, 6, &tx_bytes, NULL);
  if ( tx_bytes != 6 ) return -1;  
 
  for(int i=0; i<length; i++)
    checksum += buffer[i];
      
  WriteFile(hSerial, buffer, length, &tx_bytes, NULL);
  if ( tx_bytes != length ) return -1;
  
  buf[0] = checksum & 0xFF;
  buf[1] = checksum >> 8;

  WriteFile(hSerial, buf, 2, &tx_bytes, NULL);
  if ( tx_bytes != 2 ) return -1;  
  return 0;
  
}

/**
*** main()
**/
int main(int argc, char *argv[])
{
    // Declare variables and structures
    int m, n;
    unsigned char buffer[MAX_PATH];
    unsigned char text_to_send[MAX_PATH];
    unsigned char digits[MAX_PATH];
    int baudrate = 38400;
    int dev_num = 50;
    int parse_hex_bytes = 0;
    char dev_name[MAX_PATH];
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    DWORD rx_bytes=0;
     
    // Parse command line arguments
    int argn = 1;
    strcpy(buffer, "");
    while(argn < argc)
    {
        if (strcmp(argv[argn], "/baudrate") == 0)
        {
            // Parse baud rate
            if (++argn < argc && (baudrate = atoi(argv[argn]) > 0))
            {
                fprintf(stderr, "%d baud specified\n", baudrate);
            }
            else
            {
                fprintf(stderr, "Baud rate error\n");
                return 1;
            }
        }
        else if (strcmp(argv[argn], "/devnum") == 0)
        {
            // Parse device number. SerialSend actually just
            // begins searching at this number and continues
            // working down to zero.
            if (++argn < argc)
            {
                dev_num = atoi(argv[argn]);
                fprintf(stderr, "Device number %d specified\n", dev_num);
            }
            else
            {
                fprintf(stderr, "Device number error\n");
                return 1;
            }
        }      
        else if (strcmp(argv[argn], "/hex") == 0)
        {
            // Parse flag for hex byte parsing.
            // If this flag is set, then arbitrary byte values can be
            // included in the string to send using '\x' notation.
            // For example, the command "SerialSend /hex Hello\x0D"
            // sends six bytes in total, the last being the carriage
            // return character, '\r' which has hex value 0x0D.
            parse_hex_bytes = 1;
        }
        else
        {
            // This command line argument is the text to send
            strcpy(buffer, argv[argn]);
        }
         
        // Next command line argument
        argn++;
    }
     
    // Check that some text to send was provided
    if (strlen(buffer) == 0)
    {
        fprintf(stderr, "Usage:\n\n\tSerialSend [/baudrate BAUDRATE] ");
        fprintf(stderr, "[/devnum DEVICE_NUMBER] [/hex] \"TEXT_TO_SEND\"\n");
        return 1;
    }
     
    // If hex parsing is enabled, modify text to send
    n = 0; m = 0;
    while(n < strlen(buffer))
    {
        if (parse_hex_bytes && buffer[n] == '\\')
        {
            n++;
            if (buffer[n] == '\\') text_to_send[m] = '\\';
            else if (buffer[n] == 'n') text_to_send[m] = '\n';
            else if (buffer[n] == 'r') text_to_send[m] = '\r';
            else if (buffer[n] == 'x')
            {
                digits[0] = buffer[++n];
                digits[1] = buffer[++n];
                digits[2] = '\0';
                text_to_send[m] = strtol(digits, NULL, 16);
            }
        }
        else
        {
            text_to_send[m] = buffer[n];
        }
         
        m++; n++;
    }
    text_to_send[m] = '\0'; // Null character to terminate string
     
    // Open the highest available serial port number
    fprintf(stderr, "Searching serial ports...\n");
    while(dev_num >= 0)
    {
        fprintf(stderr, "\r                        ");
        fprintf(stderr, "\rTrying COM%d...", dev_num);
        sprintf(dev_name, "\\\\.\\COM%d", dev_num);
        hSerial = CreateFile(
        dev_name, GENERIC_READ|GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
        if (hSerial == INVALID_HANDLE_VALUE) dev_num--;
        else break;
    }
 
    if (dev_num < 0)
    {
        fprintf(stderr, "No serial port available\n");
        return 1;
    }
 
    fprintf(stderr, "OK\n");
 
    // Set device parameters (38400 baud, 1 start bit,
    // 1 stop bit, no parity)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams) == 0)
    {
        fprintf(stderr, "Error getting device state\n");
        CloseHandle(hSerial);
        return 1;
    }
    dcbSerialParams.BaudRate = CBR_38400;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if(SetCommState(hSerial, &dcbSerialParams) == 0)
    {
        fprintf(stderr, "Error setting device parameters\n");
        CloseHandle(hSerial);
        return 1;
    }
 
    // Set COM port timeout settings
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if(SetCommTimeouts(hSerial, &timeouts) == 0)
    {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }
     
    
    // flush input buffer    
    //ReadFile(hSerial,  buffer, sizeof(buffer), &rx_bytes, NULL);
       
       
    // Send data frame with the syntax '#' '#' [Opcode LSB] [Opcode MSB] [Length LSB] [Length MSB] [DATA 0] ... [Data N] [Checksum LSB] [Checksum MSB] 
    printf("Result: size=%d, result=%d\n", m, SendData(hSerial, 0xFF01, text_to_send, m)); 
    
    Sleep(100);
    memset(buffer,0,sizeof(buffer));

    ReadFile(hSerial,  buffer, sizeof(buffer), &rx_bytes, NULL);
    int val = buffer[0];
    val += buffer[1] << 8;
    for( int i=0; i<rx_bytes; i++)
      printf("%d: %d\n", i, (int)buffer[i]);
    printf("%d\n", (int)val); 

    // Close serial port
    fprintf(stderr, "Closing serial port...");
    if (CloseHandle(hSerial) == 0)
    {
        fprintf(stderr, "Error\n", dev_name);
        return 1;
    }
    fprintf(stderr, "OK\n");
 
    // exit normally
    return 0;
}