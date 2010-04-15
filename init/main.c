/* 
 * Copyright 2009, 2010 Nick Johnson 
 * ISC Licensed, see LICENSE for details
 */

#include <flux/arch.h>
#include <flux/signal.h>
#include <flux/request.h>
#include <flux/proc.h>
#include <flux/driver.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	req_t *req = ralloc();
	req->transid = 42;

	_sctl(SCTL_POLICY, SIG_READ, 1);

	fire(1, SIG_READ , req);
	fire(1, SIG_WRITE, req);

	req = sigpull(SIG_READ);

	if (req->transid != 42) {
		exit(0);
	}

	for(;;);
}
