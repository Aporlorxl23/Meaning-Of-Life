#include <stdio.h>
#include <stdlib.h>

void C_Application_Firewall(char* in_buf){
    for(char c = *in_buf++; c != '\x00'; c = *in_buf++) {
        if(c=='A') {
            printf("You have been blocked!\n");
            printf("Your IP has been reported to the authorities.\n");
            exit(-1);
        }
    }
}

void CAFtest() {
    char buf[256] = {0};
    printf("\nC Application Firewall Test - please try a payload:\n");
    gets(buf);
    C_Application_Firewall(buf);
    printf(buf);
}

int main(int argc, char* argv[]) {
    while(1) {
        CAFtest();
    }
}
// gcc caf.c -o caf
