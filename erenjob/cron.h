#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "types.h"

void get_time(void);
void runner(struct timer_options *timer);
int pre_runner();
void init(void);

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
    sleep(1);
    printf("[+] Waiting %d:%d:%d, Command: \"%s\"\n", timer->hour, timer->minute, timer->second, timer->command);
    while(1) {
        get_time();
        if (timer->hour == timeinfo->tm_hour && timer->minute == timeinfo->tm_min && timer->second == timeinfo->tm_sec){
            system(timer->command);
            printf("[*] Command: \"%s\" completed!\n", timer->command);
            break;
        }
    }
}

int pre_runner() {
    FILE *fp = fopen(CONFIG_FILE,"r");
    if (fp == NULL) {
        printf("[!] %s file not found!\n", CONFIG_FILE);
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
    fclose(fp);
    return 0;
}

void init(void) {
    pre_runner();
}
