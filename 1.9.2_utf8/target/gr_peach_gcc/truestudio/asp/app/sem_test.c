#include <kernel.h>
#include <t_syslog.h>
#include "target_syssvc.h"
#include "kernel_cfg.h"
#include "syssvc/syslog.h"

#include "sem_test.h"
#include "sample1.h"

/*
 *  並行実行されるタスク
 */

void taskSend(intptr_t exinf){

	volatile ulong_t	i;
	int_t		tskno = (int_t) exinf;
	char		c;

	while(true){
		syslog(LOG_NOTICE, "taskSend is running");
		for (i = 0; i < task_loop; i++);

		counter ++;

		c = message[tskno-1];
		message[tskno-1] = 0;
		switch (c) {
			case 'e':
				syslog(LOG_INFO, "#%d#ext_tsk()", tskno);
				SVC_PERROR(ext_tsk());
				assert(0);

			case 's':
				syslog(LOG_INFO, "#%d#slp_tsk_chung()", tskno);
				SVC_PERROR(slp_tsk());
				break;
		}

	}
	syslog(LOG_INFO, "#%d#ext_tsk()", tskno);
	SVC_PERROR(ext_tsk());
}

void taskReceive(intptr_t exinf){

	volatile ulong_t	i;
	int_t		tskno = (int_t) exinf;
	char		c;

	while(true){
		syslog(LOG_NOTICE, "taskReceive is running");
		for (i = 0; i < task_loop; i++);

		counter ++;

		c = message[tskno-1];
		message[tskno-1] = 0;

		switch (c) {
			case 'e':
				syslog(LOG_INFO, "#%d#ext_tsk()", tskno);
				SVC_PERROR(ext_tsk());
				assert(0);

			case 's':
				syslog(LOG_INFO, "#%d#slp_tsk_chung()", tskno);
				SVC_PERROR(slp_tsk());
				break;
		}

	}
	syslog(LOG_INFO, "#%d#ext_tsk()", tskno);
	SVC_PERROR(ext_tsk());
}
