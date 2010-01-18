#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include <config.h>

/***** PORT ACCESS *****/
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t ind(uint16_t port);

void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
void outd(uint16_t port, uint32_t value);

void iodelay(uint32_t usec);

/***** DEVICE DESCRIPTOR *****/

/* 	device_t specifics:
 * 		bits 00-07: device number
 * 		bits 08-15: bus number
 * 		bits 16-23: device type
 * 		bits 24-30: bus type (0 - native, 1 - PCI)
 */

/* "native" (non-PCI) device numbers:
 * 0 - ATA controller 0
 * 1 - ATA controller 1
 * 2 - ATA controller 2
 * 3 - ATA controller 3
 * 4 - keyboard
 * 5 - VGA controller
 * 6 - floppy controller
 */

/* device types:
 * 0 - keyboard input
 * 1 - mouse input
 * 2 - text/serial output
 * 3 - graphical output
 * 4 - floppy disk
 * 5 - ATA disk
 * 6 - SCSI disk
 * 7 - network device
 */

typedef int32_t device_t;

uint16_t dev_getdevice(device_t dev);
uint16_t dev_getvendor(device_t dev);
uint16_t dev_getclass(device_t dev);
uint16_t dev_getiobase(device_t dev, int bar);
uint16_t dev_getiolimit(device_t dev, int bar);
uint16_t dev_getirqnum(device_t dev);
uint16_t dev_getstatus(device_t dev);
uint16_t dev_command(device_t dev, uint16_t command);

/***** REQUEST STRUCTURE *****/

struct request {
	uint32_t checksum;				/* Checksum (bit parity) */
	uint32_t resource;				/* Resource ID */
	uint16_t datasize;				/* Size of request data */
	uint16_t transid;				/* Transaction ID */
	uint16_t dataoff;				/* Offset of request data */
	uint16_t format;				/* Header format */
	uint32_t fileoff[4];			/* File offset */
	uint8_t  reqdata[PAGESZ-32];	/* Request data area */
} __attribute__ ((packed));

struct request *req_alloc(void);
void			req_free(struct request *r);
struct request *req_catch(void *grant);
struct request *req_checksum(struct request *r);
bool            req_check(struct request *r);

#define REQ_READ 0
#define REQ_WRITE 1

/***** DRIVER INTERFACE STRUCTURE *****/ 

typedef void (*callback_t)(struct request *r);
struct driver_interface {
	void (*init) (device_t dev);	/* Initialize driver on device */
	void (*halt) (void);			/* De-initialize device */

	void (*work) (void);			/* Do background work */
	size_t jobs;					/* Number of background jobs to complete */
};

#endif