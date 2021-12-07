#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>

#define KEY1 24601
#define KEY2 24602

int main(int argc, char const *argv[]) {
  if (argc < 2) printf("Not enough arguments\n");

  else if (strcmp(argv[1], "create") == 0) {
    int file = open("data.txt", O_CREAT | O_TRUNC, 0644);
    if (file < 0) {
      printf("Error: %s\n", strerror(errno));
      return 0;
    }

    int semd = semget(KEY1, 1, IPC_CREAT | IPC_EXCL | 0644);
    union semun us;
    us.val = 1;
    int r = semctl(semd, 0, SETVAL, us);

    int shmd = shmget(KEY2, sizeof(int), IPC_CREAT | 0644);
  }

  else if (strcmp(argv[1], "remove") == 0) {
    int semd = semget(KEY1, 1, 0);
    int shmd = shmget(KEY2, 1, 0);

    semctl(semd, IPC_RMID, 0);
    shmctl(shmd, IPC_RMID, 0);

    int file = open("data.txt", O_RDONLY, 0644);
    if (file < 0) {
      printf("Error: %s\n", strerror(errno));
      return 0;
    }

    struct stat st;
    stat("data.txt", &st);
    char *r = malloc(st.st_size + 1);
    int val = read(file, r, st.st_size);
    r[st.st_size] = '\0';
    if (val < 0) {
      printf("Error: %s\n", strerror(errno));
      return 0;
    }
    printf("Full story:\n%s\n", r);
  }

  else printf("Invalid arguments\nUse create or remove");

  return 0;
}
