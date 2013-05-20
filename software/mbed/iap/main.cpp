/*    IAP demo : demo code for internal EEPROM memory access
 *
 *    LPC11U24FBD64/401 : EEPROM 4KB
 *    Remark: The top 64 bytes of the EEPROM memory are reserved and cannot be written to.
 *
 *     by Tedd OKANO http://mbed.org/users/okano/notebook/iap-in-application-programming-internal-flash-eras/
 *     modified by Suga (supported to LPC11U24)
 */
 
#include    "mbed.h"
#include    "IAP.h"
 
#define     MEM_SIZE        256
#define     TARGET_ADDRESS   64
 
void    memdump( char *p, char *b, int n );
int     isprint( int c );
 
IAP     iap;
 
 
int main() {
    char    mem[ MEM_SIZE ];    //  memory, it should be aligned to word boundary
    char    mem2[ MEM_SIZE ];
    int     r;
 
    printf( "IAP: EEPROM writing test\n" );
    printf( "  device-ID = 0x%08X, serial# = 0x%08X, CPU running %dkHz\n", iap.read_ID(), iap.read_serial(), SystemCoreClock / 1000 );
 
    for ( int i = 0; i < MEM_SIZE; i++ )
        mem[ i ]    = i & 0xFF;
 
    r   = iap.write_eeprom( mem, (char*)TARGET_ADDRESS, MEM_SIZE );
    printf( "copied: SRAM(0x%08X)->EEPROM(0x%08X) for %d bytes. (result=0x%08X)\n", mem, TARGET_ADDRESS, MEM_SIZE, r );
 
    r   = iap.read_eeprom( (char*)TARGET_ADDRESS, mem2, MEM_SIZE );
    printf( "copied: EEPROM(0x%08X)->SRAM(0x%08X) for %d bytes. (result=0x%08X)\n", TARGET_ADDRESS, mem, MEM_SIZE, r );
 
    // compare
    r = memcmp(mem, mem2, MEM_SIZE);
    printf( "compare result     = \"%s\"\n", r ? "FAILED" : "OK" );
 
    printf( "showing the EEPROM contents...\n" );
    memdump( (char*)TARGET_ADDRESS, mem2, MEM_SIZE );
    printf( "Re-invoking the ISP...\n" );
	iap.reinvoke_isp();
}
 
 
void memdump( char *base, char *buf, int n ) {
    unsigned int    *p;
 
    printf( "  memdump from 0x%08X for %d bytes", (unsigned long)base, n );
 
    p   = (unsigned int *)((unsigned int)buf & ~(unsigned int)0x3);
 
    for ( int i = 0; i < (n >> 2); i++, p++ ) {
        if ( !(i % 4) )
            printf( "\n  0x%08X :", (unsigned int)base + i * 4 );
 
        printf( " 0x%08X", *p );
    }
 
    printf( "\n" );
}
 