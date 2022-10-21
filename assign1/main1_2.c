// CSB19057
// Prasanjit Dutta

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>

char **read_input(char *input)
{
    char **cmd = malloc(8 * sizeof(char *));
    int index = 0;
    char *parsed = strtok(input, " ");
    while (parsed != NULL)
    {
        cmd[index] = parsed;
        index++;
        parsed = strtok(NULL, " ");
    }
    cmd[index] = NULL;
    return cmd;
}

int main()
{
    int st;
    char **cmd;
    char *input;
    pid_t child_pid;

    while (1)
    {
        input = readline("myshell> ");
        cmd = read_input(input);
        if (!cmd[0])
        {
            free(input);
            free(cmd);
            continue;
        }

        child_pid = fork();
        if (child_pid == 0)
        {
            execvp(cmd[0], cmd);
            printf("No command\n");
        }
        else
            waitpid(child_pid, &st, WUNTRACED);
        free(input);
        free(cmd);
    }
    return 0;
}
