#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

pid_t popen2(const char *const argv[], int *in, int *out, int *err) {
    int res;
    pid_t pid = 0;
    int inpipefd[2];
    int outpipefd[2];
    int errpipefd[2];
    if(0!=pipe(inpipefd)) {
        perror("allocating pipe for child stdin");
        return -1;
    }
    if(0!=pipe(outpipefd)) {
        close(inpipefd[0]);
        close(inpipefd[1]);
        perror("allocating pipe for child stdout");
        return -1;
    }
    if(0!=pipe(errpipefd)) {
        close(inpipefd[0]);
        close(inpipefd[1]);
        close(outpipefd[0]);
        close(outpipefd[1]);
        perror("allocating pipe for child stderr");
        return -1;
    }
    pid = fork();
    if (0==pid) {
        if (-1==dup2(inpipefd[0], STDIN_FILENO)) {exit(errno);}
        if (-1==dup2(outpipefd[1], STDOUT_FILENO)) {exit(errno);}
        if (-1==dup2(errpipefd[1], STDERR_FILENO)) {exit(errno);}
        close(inpipefd[0]);
        close(inpipefd[1]);
        close(outpipefd[0]);
        close(outpipefd[1]);
        close(errpipefd[0]);
        close(errpipefd[1]);
        execvp(argv[0], (char* const*)argv);
        perror("exec failed");
        exit(1);
    }
    close(inpipefd[0]);
    close(outpipefd[1]);
    close(errpipefd[1]);
    *in = inpipefd[1];
    *out = outpipefd[0];
    *err = errpipefd[0];
    return pid;
}
