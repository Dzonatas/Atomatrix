#define ALCHEMATRIX_VERSION_MAJOR	0
#define ALCHEMATRIX_VERSION_MINOR	1
#define ALCHEMATRIX_VERSION_CYCLE	0
#define ALCHEMATRIX_VERSION_ISSUE	10
#define ALCHEMATRIX_VERSION_MAJOR_Q	"0"
#define ALCHEMATRIX_VERSION_MINOR_Q	"1"
#define ALCHEMATRIX_VERSION_CYCLE_Q	"0"
#define ALCHEMATRIX_VERSION_ISSUE_Q	"10"

#if ALCHEMATRIX_VERSION_CYCLE == 0 && ALCHEMATRIX_VERSION_ISSUE == 0
# define ALCHEMATRIX_VERSION		ALCHEMATRIX_VERSION_MAJOR.ALCHEMATRIX_VERSION_MINOR
# define ALCHEMATRIX_VERSION_Q		ALCHEMATRIX_VERSION_MAJOR_Q "." ALCHEMATRIX_VERSION_MINOR_Q
#elif ALCHEMATRIX_VERSION_ISSUE == 0
# define ALCHEMATRIX_VERSION		ALCHEMATRIX_VERSION_MAJOR.ALCHEMATRIX_VERSION_MINOR.ALCHEMATRIX_VERSION_CYCLE
# define ALCHEMATRIX_VERSION_Q		ALCHEMATRIX_VERSION_MAJOR_Q "." ALCHEMATRIX_VERSION_MINOR_Q "." ALCHEMATRIX_VERSION_CYCLE_Q
#else
# define ALCHEMATRIX_VERSION		ALCHEMATRIX_VERSION_MAJOR.ALCHEMATRIX_VERSION_MINOR.ALCHEMATRIX_VERSION_CYCLE.ALCHEMATRIX_VERSION_ISSUE
# define ALCHEMATRIX_VERSION_Q		ALCHEMATRIX_VERSION_MAJOR_Q "." ALCHEMATRIX_VERSION_MINOR_Q "." ALCHEMATRIX_VERSION_CYCLE_Q "." ALCHEMATRIX_VERSION_ISSUE_Q
#endif

#define ALCHEMATRIX_RC_VERSION		ALCHEMATRIX_VERSION_MAJOR, ALCHEMATRIX_VERSION_MINOR, ALCHEMATRIX_VERSION_CYCLE, ALCHEMATRIX_VERSION_ISSUE
#define ALCHEMATRIX_RC_COMMENTS		"ATOMATRIX " ATOMATRIX
#define ALCHEMATRIX_RC_VERSION_Q	ALCHEMATRIX_VERSION_Q
#define ALCHEMATRIX_RC_DESCRIPTION	"Alchematrix " ALCHEMATRIX_VERSION_Q

#define ALCHEMATRIX			ALCHEMATRIX_VERSION_Q
