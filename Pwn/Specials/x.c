#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void buff_issue(){
  setvbuf(stdin,NULL,_IONBF,0);
  setvbuf(stdout,NULL,_IONBF,0);
  setvbuf(stderr,NULL,_IONBF,0);
  return;
}

int main(){
  buff_issue();

  char arr[0x10]="\0";

  printf("Adinizi girin : ");
  read(STDIN_FILENO,arr,0x100);
  printf("Merhaba %s\nMesajinizi buraya birakin : ",arr);
  read(STDIN_FILENO,arr,0x100);
  printf("Mesajinizi aldık : %s\nEklemek istediginiz birsey varmi? ");
  read(STDIN_FILENO,arr,0x100);
  printf("Tesekkurler");
  return 0;

//gcc -Wl,-z,now -fpie -fstack-protector-all x.c -o chall
//strip chall
}
