#include <stdio.h>
#include<unistd.h> /* read, write */
#include<stdlib.h>
#include <errno.h>
#include <string.h>
//#include<wait.h>
#include<sys/types.h> /* pid_t */
#include<sys/ipc.h>
#include<sys/sem.h>
#include <sys/socket.h>
#include <sys/un.h>
void CreateSem(key_t key, int N);
void InitSem(int N, int V);
void P (int N);
void V (int N);
void DestroySem();
void message(int i, char s);
void waiting (int N);
