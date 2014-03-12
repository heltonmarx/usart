#ifndef HW_SERIAL_H_INCLUDED
#define HW_SERIAL_H_INCLUDED

#include <string.h>

#define EIORECV         	(1002)
#define ETIMEOUT        	(1010)

/**
 *	Open serial port ("/dev/ttySX or /dev/ttyUSBX").
 *
 * 	@param [in] uart	String with device path
 * 	@param [in] bps		Baudrate (e.g.: 19200)
 *
 *	@return	Success, file descriptor is returned.
 *	@return Error, -1 is returned.
 */
int open_serial(const char *uart, int bps);

/**
 *	Close serial port device.
 *
 * 	@param [in] fd		File descriptor.
 */
void close_serial(int fd);

/**
 *	Write data on serial port device
 *
 *	@param [in] fd		File descriptor.
 *	@param [in] bf		Src Buffer.
 *	@param [in] n		Length of buffer.
 *
 * 	@return		Success, number of bytes written is returned.
 *	@return		On error, -1 is returned.
 */
int write_serial(int fd, const void *bf, size_t n);

/**
 *	Read data from serial port device.
 *
 *	@param [in] fd		File descriptor.
 *	@param [out] bf		Dst Buffer
 *	@param [in]  n		Length of buffer.
 *	@param [in]	timeout	Timeout to read data, if 0, timeout is disable
 *
 * 	@return		Success, length of read data is returned.
 *	@return		On error, -1 is returned.
 */
int read_serial(int fd, void *bf, size_t n, int timeout);

#endif /* HW_SERIAL_H_INCLUDED */

