#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "popen2.h"

void test_popen2() {
    int in, out, err;
    int res;
    pid_t pid;
    const char *argv[4];
    char buf[1024];
    argv[0] = "tr";
    argv[1] = "[a-z]";
    argv[2] = "[A-Z]";
    argv[3] = NULL;
    
    if(0>=(pid = popen2(argv, &in, &out, &err))) {
        fprintf(stderr, "TEST FAIL: popen2 errno: %d\n", errno);
        return;
    }
    char* msg = "hello world";
    res = write(in, msg, 12);
    if(res!=12) {
        fprintf(stderr, "TEST FAIL: unexpected amount written %d\n", res);
        return;
    }
    res = close(in);
    if(res!=0) {
        fprintf(stderr, "TEST FAIL: close(in) returned %d\n", res);
        return;
    }
    res = read(out, buf, 1024);
    if(res != 12) {
        fprintf(stderr, "TEST FAIL: unexpected amount written %d\n", res);
        return;
    }
    buf[res] = 0;
    res = close(out);
    if(res!=0) {
        fprintf(stderr, "TEST FAIL: close(out) returned %d\n", res);
        return;
    }
    res = close(err);
    if(res!=0) {
        fprintf(stderr, "TEST FAIL: close(err) returned %d\n", res);
        return;
    }
    if(!strncmp("HELLO WORLD\n", buf, 12)) {
        fprintf(stderr, "TEST FAIL: unexpected results read %s\n", buf);
    }
    if(kill(pid,0)) {
        fprintf(stderr, "TEST FAIL: kill(%d,0) found the process\n", pid);
    }
    fprintf(stderr, "TEST PASS: %s\n",buf);
}

int main() {
    test_popen2();
}