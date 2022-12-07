#define WORKER_MAX 5
#define CONFIG_FILE "cron.config"

struct tm *timeinfo;

struct timer_options {
    pthread_t thread_id;
    int hour;
    int minute;
    int second;
    char command[1024];
} timer[WORKER_MAX];
