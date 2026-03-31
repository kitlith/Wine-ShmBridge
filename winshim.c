#include <stdarg.h>
#include <stdio.h>
#include <wine/unixlib.h>
#include "shmbridge.h"

unixlib_handle_t __shmbridge_unixlib_handle;
unixlib_module_t __shmbridge_unixlib_module;

#undef WINE_UNIX_CALL
#define WINE_UNIX_CALL(code,args) __wine_unix_call( __shmbridge_unixlib_handle, (code), (args) )

const UNICODE_STRING shmbridge_so = {
    .Buffer = L"shmbridge",
    .Length = sizeof(L"shmbridge"),
    .MaximumLength = sizeof(L"shmbridge"),
};

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID reserved)
{
    switch( reason )
    {
	case DLL_PROCESS_ATTACH:
	    if( __wine_load_unix_lib(&shmbridge_so, &__shmbridge_unixlib_module, &__shmbridge_unixlib_handle) ) return FALSE;
	    break;
	case DLL_PROCESS_DETACH:
	    if ( __wine_unload_unix_lib(__shmbridge_unixlib_module) ) return FALSE;
	    break;
	case DLL_THREAD_ATTACH:
	    break;
	case DLL_THREAD_DETACH:
	    break;
	default:
	    break;
    }
    return TRUE;
}

void *WINAPI sem_open(const char *name, int oflag, unsigned int mode, unsigned int value) {
    union prm_open args = {
        .name = name,
        .oflag = oflag,
        .mode = mode,
        .value = value,
    };
    WINE_UNIX_CALL(unix_sem_open, &args);
    return args.sem;
}

int WINAPI sem_post(void *sem) {
    return WINE_UNIX_CALL(unix_sem_post, sem);
}

int WINAPI sem_wait(void *sem) {
    return WINE_UNIX_CALL(unix_sem_wait, sem);
}

int WINAPI sem_trywait(void *sem) {
    return WINE_UNIX_CALL(unix_sem_trywait, sem);
}

int WINAPI sem_timedwait(void *sem, const struct timespec *restrict abstime) {
    struct prm_timedwait args = {
        .sem = sem,
        .abstime = abstime,
    };
    return WINE_UNIX_CALL(unix_sem_timedwait, &args);
}

int WINAPI sem_unlink(const char *name) {
    return WINE_UNIX_CALL(unix_sem_unlink, name);
}

int WINAPI sem_close(void *sem) {
    return WINE_UNIX_CALL(unix_sem_close, sem);
}
