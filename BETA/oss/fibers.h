// an insert for XP if the winbase.h does not include correctly

typedef VOID (WINAPI *PFIBER_START_ROUTINE)(LPVOID) ;
typedef PFIBER_START_ROUTINE LPFIBER_START_ROUTINE ;

WINBASEAPI LPVOID WINAPI
CreateFiber(IN SIZE_T,IN LPFIBER_START_ROUTINE,IN LPVOID) ;

WINBASEAPI LPVOID WINAPI CreateFiberEx(SIZE_T,SIZE_T,DWORD,LPFIBER_START_ROUTINE,LPVOID) ;

WINBASEAPI VOID WINAPI DeleteFiber(IN LPVOID) ;

WINBASEAPI LPVOID WINAPI ConvertThreadToFiber(IN LPVOID lpParameter) ;

WINBASEAPI BOOL WINAPI ConvertFiberToThread(VOID) ;

WINBASEAPI VOID WINAPI SwitchToFiber(IN LPVOID) ;

WINBASEAPI BOOL WINAPI SwitchToThread(VOID) ;