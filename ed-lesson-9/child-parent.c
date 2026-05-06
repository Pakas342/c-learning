#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define buf_size (100)

int main() {
    int pipefd_p_w[2];
    int pipefd_c_w[2];
    pipe(pipefd_p_w);
    pipe(pipefd_c_w);

    pid_t pid = fork();

    char buf[buf_size] = {'0'};
    char message[] = "hora papi, responde?";
    char message2[] = "Ahi se lo puse mi rey";

    if (pid < 0) {
        printf("ta verga esta rota manin\n");
    } else if (pid > 0) {
        // Parent code

        // closing child write fd, as I'm the child
        close(pipefd_c_w[1]);
        // closing parent read fd, as I'm not reading that shit
        close(pipefd_p_w[0]);

        printf("What time is it, son?\n");
        write(pipefd_p_w[1], message, sizeof(message));
        read(pipefd_c_w[0], buf, 99);
        if (strcmp(buf, message2) == 0) {
            printf("Thank you gonorreita\n");
        }
        wait(NULL);

    } else if (pid == 0) {
        // child code

        // closing parent write fd, as I'm the child
        close(pipefd_p_w[1]);
        // closing child read fd, as I'm not reading that shit
        close(pipefd_c_w[0]);

        time_t t;
        struct tm *tm_info = localtime(&t); // statically allocated memory
        char buf_time[256];
        strftime(buf_time, 256, "%H:%M%p", tm_info);
        read(pipefd_p_w[0], buf, 99);
        if (strcmp(buf, message) == 0) {
            printf("Is: %s, so is Comp2017 time!\n", buf_time);
            write(pipefd_c_w[1], message2, sizeof(message2));
        }
    }
}
