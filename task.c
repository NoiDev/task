
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TASK_FILE_NAME    ".tasks"
#define TEMP_FILE_PATTERN ".tasks.XXXXXX"

#define DEFAULT_TASK_LENGTH 20
#define TEMP_NAME_LENGTH    20

#define NO_TASKS_LEFT "(no tasks left)\n"

void print_missing_task_file_help_text(char *TaskFilePath) {
    printf("No task file found (%s).\n", TaskFilePath);
    printf(" \"task push [name]\" to add a task or\n");
    printf(" \"task init\" to create an empty task list\n");
}

int main(int argc, const char* argv[]) {

    char *TaskFilePath = TASK_FILE_NAME;

    FILE *TaskFile;

    TaskFile = fopen(TaskFilePath, "r");

    /* peek current task (default) */
    if (argc <= 1) {
        if (TaskFile) {
            size_t TaskLength = DEFAULT_TASK_LENGTH;
            ssize_t Result;
            char *CurrentTask = malloc(TaskLength * sizeof(char));
            Result = getline(&CurrentTask, &TaskLength, TaskFile);
            if (Result == -1) {
                strcpy(CurrentTask, NO_TASKS_LEFT);
            }
            printf("Current Task: %s", CurrentTask);
            free(CurrentTask);
            fclose(TaskFile);
        } else {
            print_missing_task_file_help_text(TaskFilePath);
        }

    /* list tasks */
    } else if (strcmp(argv[1], "list") == 0) {
        if (argc > 2) {
            printf("Too many arguments.\n");
            printf(" (Anything after \"list\" is ignored.)\n");
        }
        if (TaskFile) {
            size_t TaskLength = DEFAULT_TASK_LENGTH;
            ssize_t Result;
            char *CurrentTask = malloc(TaskLength * sizeof(char));
            int TaskCount = 0;
            while ((Result = getline(&CurrentTask, &TaskLength, TaskFile)) != -1) {
                printf("%s", CurrentTask);
                TaskCount++;
            }
            if (TaskCount<=0) {
                printf("%s", NO_TASKS_LEFT);
            }
            free(CurrentTask);
            fclose(TaskFile);
        } else {
            print_missing_task_file_help_text(TaskFilePath);
        }

    /* push new task */
    } else if (strcmp(argv[1], "push") == 0) {
        if (argc <= 2) {
            printf("Must pass a task to push.\n");
            printf(" \"task push [name]\" to add a task\n");
            return 0;
        } else if (argc > 3) {
            printf("Too many arguments.  Did you forget to quote your task?.\n");
            printf(" \"task push \"long task name\" to add a task\n");
            return 0;
        }

        FILE *TempFile;
        char TempFilePath[] = TEMP_FILE_PATTERN;
        mktemp(&TempFilePath[0]);
        TempFile = fopen(TempFilePath, "w");

        if (TempFile) {
            fputs(argv[2], TempFile);
            fputc('\n', TempFile);

            if (TaskFile) {
                size_t TaskLength = DEFAULT_TASK_LENGTH;
                ssize_t Result;
                char *CurrentTask = malloc(TaskLength * sizeof(char));
                while ((Result = getline(&CurrentTask, &TaskLength, TaskFile)) != -1) {
                    fprintf(TempFile, "%s", CurrentTask);
                }
                free(CurrentTask);
                fclose(TaskFile);
                remove(TaskFilePath);
            } else {
                printf("Created a new task file (%s).\n", TaskFilePath);
            }
            fclose(TempFile);
            rename(TempFilePath, TaskFilePath);

            printf("Pushed New Task: %s\n", argv[2]);
        }

    /* pop top task */
    } else if (strcmp(argv[1], "pop") == 0) {
        if (argc > 2) {
            printf("Too many arguments.  (Popping tasks by name not supported.)\n"); 
            printf(" \"task pop\" to pop the top task off the stack\n");
            return 0;
        }

        if (TaskFile) {
            FILE *TempFile;
            char TempFilePath[] = TEMP_FILE_PATTERN;
            mktemp(&TempFilePath[0]);
            TempFile = fopen(TempFilePath, "w");

            if (TempFile) {
                size_t TaskLength = DEFAULT_TASK_LENGTH;
                ssize_t Result;
                char *CurrentTask = malloc(TaskLength * sizeof(char));

                if ((Result = getline(&CurrentTask, &TaskLength, TaskFile)) != -1) {
                    printf("Completed Task: %s", CurrentTask);
                }

                /* Pull the next task off of TempFile separately to report it to the user */
                if ((Result = getline(&CurrentTask, &TaskLength, TaskFile)) != -1) {
                    fputs(CurrentTask, TempFile);
                } else {
                    strcpy(CurrentTask, NO_TASKS_LEFT);
                }
                printf("Next Task: %s", CurrentTask);

                /* Copy the remaining tasks */
                while ((Result = getline(&CurrentTask, &TaskLength, TaskFile)) != -1) {
                    fputs(CurrentTask, TempFile);
                }

                free(CurrentTask);
                fclose(TempFile);
                remove(TaskFilePath);
                rename(TempFilePath, TaskFilePath);
            }
            fclose(TaskFile);
        } else {
            print_missing_task_file_help_text(TaskFilePath);
        }

    /* init new task file */
    } else if (strcmp(argv[1], "init") == 0) {
        if (argc > 2) {
            printf("Too many arguments.\n");
            printf(" (Anything after \"init\" is ignored.)\n");
        }
        if (TaskFile) {
            printf("Task file already exists (%s).\n", TaskFilePath);
        } else {
            TaskFile = fopen(TaskFilePath, "w");
            printf("Created an empty task file (%s).\n", TaskFilePath);
        }
        fclose(TaskFile);

    /* help */
    } else if (strcmp(argv[1], "help") == 0) {
        if (argc > 2) {
            printf("Too many arguments.\n");
            printf("\"task help\" to see this screen.\n");
        }

        printf("\"task\" is a utility to manage a simple task stack.\n");
        printf("Supported commands:\n");
        printf("    (no command) - View the task on the top of the stack.\n");
        printf("    list         - View the full task stack.\n");
        printf("    pop          - Remove the top task from the stack.\n");
        printf("    push [task]  - Add a new task to the stack.\n");
        printf("    init         - Create an empty task file.\n");
        printf("    help         - View this screen.\n");

    /* unrecognized command */
    } else {
        printf("Command (%s) is not recognised.\n", argv[1]);
        printf(" \"task help\" for a list of commands .\n");
    }

    return 0;
}
