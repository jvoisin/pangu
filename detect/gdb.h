#ifndef _GDB_
#define _GDB_

#define check_strings(str_buff) (strstr(str_buff, "gdb") || strstr(str_buff, "ltrace") || strstr(str_buff, "strace"))

int dbg_file_descriptor();
int dbg_cmdline();
int dbg_ptrace();
int dbg_sigtrap();
int dbg_check_bp();
int dbg_getppid_name();

#endif
