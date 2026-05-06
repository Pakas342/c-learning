#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define buf_size (100)

int main() {
    int pipefd_p_w[2];
    int pipefd_c_w[2];
    pipe(pipefd_p_w);
    pipe(pipefd_c_w);

    char buf[buf_size] = {'0'};
    int selection_2 = 0;
    char message[] = "Online";
    char message2[] = "Game time!";
    char *possible[] = {"rock", "paper", "scissors"};

    pid_t pid = fork();

    srand(pid);
    if (pid < 0) {
        printf("ta verga esta rota manin\n");
    } else if (pid > 0) {
        // Parent code

        // closing child write fd, as I'm the child
        close(pipefd_c_w[1]);
        // closing parent read fd, as I'm not reading that shit
        close(pipefd_p_w[0]);

        write(pipefd_p_w[1], message, sizeof(message));
        read(pipefd_c_w[0], buf, sizeof(message));
        if (strcmp(buf, message) == 0) {
            printf("Online\n");
        }
        while (1) {
            memset(buf, 0, buf_size);
            sleep(2);
            write(pipefd_p_w[1], message2, sizeof(message2));
            int selection = rand() % 3;
            write(pipefd_p_w[1], &selection, sizeof(int));
            read(pipefd_c_w[0], &selection_2, sizeof(int));
            if (selection == selection_2) {
                printf("Parent says: we draw\n");
            } else if (selection == 0 && selection_2 == 2 ||
                       selection == 2 && selection_2 == 0) {
                if (selection == 0) {
                    printf("Parent says: I won!\n");
                } else {
                    printf("Parent says: I lost, sad days\n");
                }
            } else {
                if (selection > selection_2) {
                    printf("Parent says: I won!\n");
                } else {
                    printf("Parent says: I lost, sad days\n");
                }
            }
        }

    } else if (pid == 0) {
        // child code

        // closing parent write fd, as I'm the child
        close(pipefd_p_w[1]);
        // closing child read fd, as I'm not reading that shit
        close(pipefd_c_w[0]);
        read(pipefd_p_w[0], buf, sizeof(message));
        if (strcmp(buf, message) == 0) {
            write(pipefd_c_w[1], message, sizeof(message));
            printf("Online\n");
        }
        while (1) {
            memset(buf, 0, buf_size);
            read(pipefd_p_w[0], buf, sizeof(message2));
            if (strcmp(buf, message2) == 0) {
                memset(buf, 0, buf_size);
                int selection = rand() % 3;
                write(pipefd_c_w[1], &selection, sizeof(int));
                read(pipefd_p_w[0], &selection_2, sizeof(int));
                if (selection == selection_2) {
                    printf("Child says: we draw\n");
                } else if (selection == 0 && selection_2 == 2 ||
                           selection == 2 && selection_2 == 0) {
                    if (selection == 0) {
                        printf("Child says: I won!\n");
                    } else {
                        printf("Child says: I lost, sad days\n");
                    }
                } else {
                    if (selection > selection_2) {
                        printf("Child says: I won!\n");
                    } else {
                        printf("Child says: I lost, sad days\n");
                    }
                }
            }
        }
    }
}
