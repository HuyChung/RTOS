

/*
 *  ターゲット依存の定義
 */
#include "target_test.h"

/*
 *  各タスクの優先度の定義: Dinh nghia do uu tien cua Task
 */

#define MAIN_PRIORITY	5		/* メインタスクの優先度 */
								/* HIGH_PRIORITYより高くすること */

/*
 *  ターゲットに依存する可能性のある定数の定義
 */

#ifndef STACK_SIZE
#define	STACK_SIZE		4096		/* タスクのスタックサイズ */
#endif /* STACK_SIZE */

#ifndef LOOP_REF
#define LOOP_REF		ULONG_C(1000000)	/* 速度計測用のループ回数 */
#endif /* LOOP_REF */

/*
 *  並行実行されるタスクへのメッセージ領域
 */
char	message[3];

/*
 *  ループ回数
 */
ulong_t	task_loop;		/* タスク内でのループ回数 */
ulong_t	tex_loop;		/* 例外処理ルーチン内でのループ回数 */

/*
 *  サービスコールのエラーのログ出力
 */
Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

extern void	main_task(intptr_t exinf);

#ifdef CPUEXC1
extern void	cpuexc_handler(void *p_excinf);
#endif /* CPUEXC1 */

#endif /* TOPPERS_MACRO_ONLY */
