#define P_NONE	       0	/* none set */
#define P_OR	       1	/* read/write by module principality */
#define P_OW	       2
#define P_OX	       4	/* execute/inherit by module principality */
#define P_TR	       8	/* read/write by modular scope */
#define P_TW	      16
#define P_TX	      32	/* execute/inherit by modular scope */
#define P_AR	      64	/* read/write by any module */
#define P_AW	     128	
#define P_AX	     256	/* execute/inherit by any module */
#define P_CHOWN      512	/* chown on inherit or access */
#define P_UNIQUE    1024	/* no variable/program in child can match */
#define P_UNX       2048	/* unused */
#define P_UNY       4096	/* unused */
#define P_UNZ       8192	/* unused */
#define P_PGMR     16384	/* programmer perms */
#define P_WIZARD   32768	/* wizard perms */


#define P_INHERIT(X,v)  ((						\
		(v->perms & (P_AX|P_TX|P_OX))				\
		) ? 1 : 0 )