
#include <kernel.h>
#include <t_syslog.h>
#include "kernel_cfg.h"
#include "syssvc/serial.h"
#include "syssvc/syslog.h"

#include "task_test.h"
#include "sample1.h"




/*
 *  CPU例外ハンドラ
 */
#ifdef CPUEXC1

void
cpuexc_handler(void *p_excinf)
{
	ID		tskid;

	syslog(LOG_NOTICE, "CPU exception handler (p_excinf = %08p).", p_excinf);
	if (sns_ctx() != true) {
		syslog(LOG_WARNING,
					"sns_ctx() is not true in CPU exception handler.");
	}
	if (sns_dpn() != true) {
		syslog(LOG_WARNING,
					"sns_dpn() is not true in CPU exception handler.");
	}
	syslog(LOG_INFO, "sns_loc = %d sns_dsp = %d sns_tex = %d",
									sns_loc(), sns_dsp(), sns_tex());
	syslog(LOG_INFO, "xsns_dpn = %d xsns_xpn = %d",
									xsns_dpn(p_excinf), xsns_xpn(p_excinf));

	if (xsns_xpn(p_excinf)) {
		syslog(LOG_NOTICE, "Sample program ends with exception.");
		SVC_PERROR(ext_ker());
		assert(0);
	}

	SVC_PERROR(iget_tid(&tskid));
	SVC_PERROR(iras_tex(tskid, 0x8000U));
}

#endif /* CPUEXC1 */

/*
 *  メインタスク
 */
void main_task(intptr_t exinf){

	char	c;
	ID		tskid = TASK1;
	int_t	tskno = 1;
	ER_UINT	ercd;
	PRI		tskpri;
	int_t		n = 0;
#ifndef TASK_LOOP
	volatile ulong_t	i;
	SYSTIM	stime1, stime2;
#endif /* TASK_LOOP */

	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

	/*
	 *  シリアルポートの初期化
	 *
	 *  システムログタスクと同じシリアルポートを使う場合など，シリアル
	 *  ポートがオープン済みの場合にはここでE_OBJエラーになるが，支障は
	 *  ない．
	 */
	ercd = serial_opn_por(TASK_PORTID);
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	SVC_PERROR(serial_ctl_por(TASK_PORTID,
							(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

#ifdef TASK_LOOP
	task_loop = TASK_LOOP;
#else /* TASK_LOOP */

#ifdef MEASURE_TWICE
	task_loop = LOOP_REF;
	SVC_PERROR(get_tim(&stime1));
	for (i = 0; i < task_loop; i++);
	SVC_PERROR(get_tim(&stime2));
#endif /* MEASURE_TWICE */

	task_loop = LOOP_REF;
	SVC_PERROR(get_tim(&stime1));
	for (i = 0; i < task_loop; i++);
	SVC_PERROR(get_tim(&stime2));
	task_loop = LOOP_REF * 400UL / (stime2 - stime1);

#endif /* TASK_LOOP */
	tex_loop = task_loop / 4;

	/*
	 *  タスクの起動
	 */
	SVC_PERROR(act_tsk(TASK1));
	SVC_PERROR(act_tsk(TASK2));
	SVC_PERROR(act_tsk(TASK3));

	do {
		SVC_PERROR(serial_rea_dat(TASK_PORTID, &c, 1));
		switch (c) {
			case 'e':
			case 's':
			case 'S':
			case 'd':
			case 'y':
			case 'Y':
			case 'z':
			case 'Z':
				message[tskno-1] = c;
				break;
			case '1':
				tskno = 1;
				tskid = TASK1;
				break;
			case '2':
				tskno = 2;
				tskid = TASK2;
				break;
			case '3':
				tskno = 3;
				tskid = TASK3;
				break;
			case 'a':
				syslog(LOG_INFO, "#act_tsk(%d)", tskno);
				SVC_PERROR(act_tsk(tskid));
				break;
			case 'A':
				syslog(LOG_INFO, "#can_act(%d)", tskno);
				SVC_PERROR(ercd = can_act(tskid));
				if (ercd >= 0) {
					syslog(LOG_NOTICE, "can_act(%d) returns %d", tskno, ercd);
				}
				break;
			case 't':
				syslog(LOG_INFO, "#ter_tsk(%d)", tskno);
				SVC_PERROR(ter_tsk(tskid));
				break;
			case '>':
				syslog(LOG_INFO, "#chg_pri(%d, HIGH_PRIORITY)", tskno);
				SVC_PERROR(chg_pri(tskid, HIGH_PRIORITY));
				break;
			case '=':
				syslog(LOG_INFO, "#chg_pri(%d, MID_PRIORITY)", tskno);
				SVC_PERROR(chg_pri(tskid, MID_PRIORITY));
				break;
			case '<':
				syslog(LOG_INFO, "#chg_pri(%d, LOW_PRIORITY)", tskno);
				SVC_PERROR(chg_pri(tskid, LOW_PRIORITY));
				break;
			case 'G':
				syslog(LOG_INFO, "#get_pri(%d, &tskpri)", tskno);
				SVC_PERROR(ercd = get_pri(tskid, &tskpri));
				if (ercd >= 0) {
					syslog(LOG_NOTICE, "priority of task %d is %d", tskno, tskpri);
				}
				break;
			case 'w':
				syslog(LOG_INFO, "#wup_tsk(%d)", tskno);
				SVC_PERROR(wup_tsk(tskid));
				break;
			case 'W':
				syslog(LOG_INFO, "#can_wup(%d)", tskno);
				SVC_PERROR(ercd = can_wup(tskid));
				if (ercd >= 0) {
					syslog(LOG_NOTICE, "can_wup(%d) returns %d", tskno, ercd);
				}
				break;
			case 'l':
				syslog(LOG_INFO, "#rel_wai(%d)", tskno);
				SVC_PERROR(rel_wai(tskid));
				break;
			case 'u':
				syslog(LOG_INFO, "#sus_tsk(%d)", tskno);
				SVC_PERROR(sus_tsk(tskid));
				break;
			case 'm':
				syslog(LOG_INFO, "#rsm_tsk(%d)", tskno);
				SVC_PERROR(rsm_tsk(tskid));
				break;
			case 'x':
				syslog(LOG_INFO, "#ras_tex(%d, 0x0001U)", tskno);
				SVC_PERROR(ras_tex(tskid, 0x0001U));
				break;
			case 'X':
				syslog(LOG_INFO, "#ras_tex(%d, 0x0002U)", tskno);
				SVC_PERROR(ras_tex(tskid, 0x0002U));
				break;
			case 'r':
				syslog(LOG_INFO, "#rot_rdq(three priorities)");
				SVC_PERROR(rot_rdq(HIGH_PRIORITY));
				SVC_PERROR(rot_rdq(MID_PRIORITY));
				SVC_PERROR(rot_rdq(LOW_PRIORITY));
				break;
			default:
				break;
		}
	}while(c != '\003' && c != 'Q');

	syslog(LOG_NOTICE, "Sample program ends.");
	SVC_PERROR(ext_ker());
	assert(0);
}
