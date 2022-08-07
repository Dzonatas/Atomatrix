struct _tlsp
	{
	HANDLE			memory ;
	HANDLE			detachEvent ;
	int			created ;
	struct _tlsp *		tlspp ;
	} ;

typedef struct _tlsp *		tlsp ;