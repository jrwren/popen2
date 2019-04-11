/**
 * popen2:
 * @argv: argv to be passed to execvp
 * @in: stdin fd of new process
 * @out: stdout fd of new process
 * @err: stderr fd of new process
 * 
 * returns: pid of the child process or a negative number if error is set.
 **/
pid_t popen2(const char *const argv[], int *in, int *out, int *err);
