// object.h		Copyrighted 2004 BALLARD

typedef long		ObjID;
#define			OBJID	1


struct _object
	{
	atom			orbit;
	atom			interobject;
	atom			interobjects;
	atom			principal;
	atom			precursor;
	atom			intraobject;
	atom			intraobjects;
	unsigned		serinteger ;
	unsigned short		perms;
	};

typedef struct _object *	object;
