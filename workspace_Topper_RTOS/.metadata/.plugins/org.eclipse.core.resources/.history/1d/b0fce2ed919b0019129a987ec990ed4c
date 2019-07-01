#include <kernel.h>
#include <t_syslog.h>
#include "target_syssvc.h"
#include "kernel_cfg.h"
#include "syssvc/syslog.h"

#include "task_test.h"
#include "sample1.h"
/*
 *  並行実行されるタスク
 */
void task(intptr_t exinf){

	volatile ulong_t	i;
	int_t		n = 0;
	int_t		tskno = (int_t) exinf;
	const char	*graph[] = { "|", "  +", "    *" };
	char		c;

	SVC_PERROR(ena_tex());
	while (true) {
		syslog(LOG_NOTICE, "task%d is running (%03d).   %s",
										tskno, ++n, graph[tskno-1]);
		for (i = 0; i < task_loop; i++);
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
			case 'S':
				syslog(LOG_INFO, "#%d#tslp_tsk(10000)", tskno);
				SVC_PERROR(tslp_tsk(10000));
				break;
			case 'd':
				syslog(LOG_INFO, "#%d#dly_tsk(10000)", tskno);
				SVC_PERROR(dly_tsk(10000));
				break;
			case 'y':
				syslog(LOG_INFO, "#%d#dis_tex()", tskno);
				SVC_PERROR(dis_tex());
				break;
			case 'Y':
				syslog(LOG_INFO, "#%d#ena_tex()", tskno);
				SVC_PERROR(ena_tex());
				break;
	#ifdef CPUEXC1
			case 'z':
				syslog(LOG_NOTICE, "#%d#raise CPU exception", tskno);
				RAISE_CPU_EXCEPTION;
				break;
			case 'Z':
				SVC_PERROR(loc_cpu());
				syslog(LOG_NOTICE, "#%d#raise CPU exception", tskno);
				RAISE_CPU_EXCEPTION;
				SVC_PERROR(unl_cpu());
				break;
	#endif /* CPUEXC1 */
			default:
				break;
		}
	}
}
