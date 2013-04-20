// OKANO's IAP code from
// http://mbed.org/users/okano/notebook/iap-in-application-programming-internal-flash-eras/
/**
 * @file
 */
 
#define EEPROM_ADDRESS 64 
 
/** Copy RAM to EEPROM (LPC11U24)
 *  
 *  @param    source_addr    Source RAM address from which data bytes are to be read.
 *  @param    target_addr    Destination EEPROM address where data bytes are to be written.
 *  @param    size           Number of bytes to be written.
 *  @return   error code: CMD_SUCCESS | SRC_ADDR_NOT_MAPPED | DST_ADDR_NOT_MAPPED
 *  Remark: The top 64 bytes of the EEPROM memory are reserved and cannot be written to.
 */
int write_eeprom( char *source_addr, char *target_addr, int size );
 
/** Copy EEPROM to RAM (LPC11U24)
 *  
 *  @param    source_addr    Source EEPROM address from which data bytes are to be read.
 *  @param    target_addr    Destination RAM address where data bytes are to be written.
 *  @param    size           Number of bytes to be written.
 *  @return   error code: CMD_SUCCESS | SRC_ADDR_NOT_MAPPED | DST_ADDR_NOT_MAPPED
 *  Remark: The top 64 bytes of the EEPROM memory are reserved and cannot be written to.
 */
int read_eeprom( char *source_addr, char *target_addr, int size );