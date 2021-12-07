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

int main() {
  int semd = semget(KEY1, 1, 0);
  int shmd = shmget(KEY2, 1, 0);

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1; //setting the operation to down
  semop(semd, &sb, 1); //perform the operation

  int *d = shmat(shmd, 0, 0);

  int file = open("data.txt", O_RDONLY | O_APPEND, 0644);
  lseek(file, -1 * (*d), SEEK_END);
  char r[2000];
  int val = read(file, r, *d);
  if (val < 0) {
    printf("Error: %s\n", strerror(errno));
    return 0;
  }
  r[*d] = '\0';
  printf("Previous line:%s\n", r);

  printf("Add line: ");
  fgets(r, 2000, stdin);
  lseek(file, 0, SEEK_END);
  val = write(file, r, strlen(r));
  if (val < 0) {
    printf("Error: %s\n", strerror(errno));
    return 0;
  }

  shmdt(r);
  sb.sem_op = 1; //set the operation to up
  semop(semd, &sb, 1); //perform the operation


  return 0;
}
