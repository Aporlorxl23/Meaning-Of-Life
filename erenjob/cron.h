#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define WORKER_MAX 5

struct timer_options {
    pthread_t thread_id;
    int hour;
    int minute;
    int second;
    char command[1024];
} timer[WORKER_MAX];

struct tm *timeinfo;
void get_time(void){
    time_t my_time;
    time(&my_time);
    timeinfo = localtime(&my_time);
    /*
    struct tm
    {
        int tm_sec;   // seconds after the minute - [0, 60] including leap second
        int tm_min;   // minutes after the hour - [0, 59]
        int tm_hour;  // hours since midnight - [0, 23]
        int tm_mday;  // day of the month - [1, 31]
        int tm_mon;   // months since January - [0, 11]
        int tm_year;  // years since 1900
        int tm_wday;  // days since Sunday - [0, 6]
        int tm_yday;  // days since January 1 - [0, 365]
        int tm_isdst; // daylight savings time flag
    };
    */
}


void runner(struct timer_options *timer){
    printf("[*] Waiting %d:%d:%d, Command: %s\n", timer->hour, timer->minute, timer->second, timer->command);
    while(1) {
        get_time();
        if (timer->hour == timeinfo->tm_hour && timer->minute == timeinfo->tm_min && timer->second == timeinfo->tm_sec){
            system(timer->command);
            printf("Command: %s complated!\n", timer->command);
            break;
        }
    }
}

int pre_runner(void) {
    FILE *fp = fopen("cron.config","r");
    if (fp == NULL) {
        printf("[*] 'cron.config' file not found!");
        return EXIT_FAILURE;
    }
    int i = WORKER_MAX;
    while (fscanf(fp, "%d %d %d %[^\n]", &timer[i].hour, &timer[i].minute, &timer[i].second, timer[i].command) != EOF) {
        pthread_create(&timer[i].thread_id, NULL, (void *)runner, &timer[i]);
        i--;
    }
    printf("[*] All threads created!\n");
    for (int j = WORKER_MAX; j >= i; j--) {
        pthread_join(timer[j].thread_id, NULL);
    }
    printf("[+] All threads completed!\n"); 
    fclose(fp);
}

void init(void) {
    pre_runner();
}
