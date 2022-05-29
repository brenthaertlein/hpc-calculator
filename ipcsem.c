#include"ipcsem.h"

int Num;
//function prototypes
void P1(int a);
void P2(int b);
void P3();
int open_socket(char path[]);
void accept_socket(int fd);

int pipe1[2],pipe2[2];

#define SV_SOCK_PATH_0 "/tmp/sandbox/socket_0"
#define BUF_SIZE 100

int main(){
    CreateSem(50,1);
    InitSem(0, 0);
    pipe(pipe1);
    pipe(pipe2);

    int sfd_0 = open_socket(SV_SOCK_PATH_0);
    accept_socket(sfd_0);

//    // terminal user input
//    int a,b;
//    printf("Enter a number  a : ");
//    scanf("%d",&a);
//    printf("Enter a number  b : ");
//    scanf("%d",&b);
//
//    // get data from socket
//
//    // put data into pipes
//    P1(a);
//    P2(b);
//
//    // calculate data from pipes
//    P3();
//
//    // send result back through socket
//
//    for(int i=0;i<3;i++)wait(0);
}

void P1(int a){
    if(fork()==0){
        write(pipe1[1],&a,4);
        V(0);
        exit(0);
    }
}
void P2(int b){

    if(fork()==0){
        write(pipe2[1],&b,4);
        V(0);
        exit(0);
    }

}

void P3(){

    if(fork()==0){
        P(0);
        P(0);
        int a,b;
        read(pipe1[0],&a,4);
        read(pipe2[0],&b,4);
        printf("The result is: %d \n",a+b);
        exit(0);
    }
}


void CreateSem(key_t key, int N) {
    Num=semget(key, N, 0600 | IPC_CREAT);
    if(Num==-1)
    {
        perror("Pb CreateSem");exit(1);
    }
}
/* Destroy semaphore group Num */
void DestroySem() {
    semctl(Num, 0, IPC_RMID, 0);
}
/* Initialize Semaphore */
void InitSem(int N, int V) {
    semctl(Num, N, SETVAL, V);
}
/* P operation */
void P(int N) {
    struct sembuf Tabop;
    Tabop.sem_num=N;
    Tabop.sem_op=-1;
    Tabop.sem_flg=0;
    semop(Num, &Tabop,1);
}
/* V operation */
void V(int N) {
    struct sembuf Tabop;
    Tabop.sem_num=N;
    Tabop.sem_op=1;
    Tabop.sem_flg=0;
    semop(Num, &Tabop, 1);
}


void waiting(int N)
{
    sleep(rand() % N);
}


void message(int i, char s)
{
#define column 20
    int Nb, j ;
    Nb=(i-1)*column;
    for(j=0; j<Nb; j++) putchar(' ');
    printf("%c\n" ,s);
    fflush(stdout);
}

int open_socket(char path[]) {

    struct sockaddr_un addr;

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("Server socket fd = %d\n", sfd);

    if (sfd == -1) {
        printf("Socket file descriptor is not 'legit'");
        exit(1);
    }

    if (remove(path) == -1 && errno != ENOENT) {
        printf("Remove file at %s\n", path);
        exit(100);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        printf("Unable to bind socket\n");
        exit(101);
    }

    if (listen(sfd, 5) == -1) {
        printf("Unable to listen for socket\n");
        exit(102);
    }

    return sfd;
}

void accept_socket(int sfd) {
    ssize_t num_read;
    char buf[BUF_SIZE];
    for (;;) {

        printf("Waiting to accept a connection...\n");

        int cfd = accept(sfd, NULL, NULL);
        printf("Accepted socket fd = %d\n", cfd);

        while ((num_read = read(cfd, buf, BUF_SIZE)) > 0) {
            // buf is now a char array with & delimited values
            // values are num1&num2&operator
            // split on &
            // put num1 into pipe1
            // put num2 into pipe2
            // put operator into pipe3?

            if (write(STDOUT_FILENO, buf, num_read) != num_read) {
                printf("Partial failed write\n");
            }
        }

        if (num_read == -1) {
            printf("Read error\n");
            return;
        }

        if (close(cfd) == -1) {
            printf("Socket closed\n");
            return;
        }
    }
}

