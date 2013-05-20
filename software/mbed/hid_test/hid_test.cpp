/*
 * USB HID Example, with serial console (not using Serial object, cause this is too large
 * Peter Brier
 */

#include "mbed.h"
#include "USBHID.h"

//We declare a USBHID device
USBHID hid(
 64,  // out 
 64,  // in
 0x1234, // vid
 0x0006, // pid
 0x0001, // p_release
 false // connect
);

//This report will contain data to be sent
HID_REPORT send_report, recv_report;
Ticker tic;


volatile int read_res=0, send_res=0, counter=0;

void tic_handler() {
    send_report.data[2] = counter++;
     send_res = hid.send(&send_report);	
}


int main(void) {

    static int hid_res = 0;

	hid.connect(); // do not block
 	
	//Fill the report
    for(int i = 0; i < 64; i++)
        send_report.data[i] = i;
    send_report.length = 64;

	
    tic.attach(tic_handler, 0.1);
	
    while (1)
	{
      if (hid.readNB(&recv_report)) 
	  {
	    read_res++;
    	send_report.data[1] = hid_res;		
      }
	  wait_ms(1);
	}
}
