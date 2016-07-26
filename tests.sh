
file_header "\"task\" utility"

section_header "PEEK";

rm .tasks;

test_header "peek: no file";
test_command task;

test_header "peek: empty file";
test_prep    task init;
test_command task;

test_header "peek: non-empty file";
test_prep    task push FIRST_TASK;
test_command task;



section_header "LIST";
rm .tasks;

test_header "list: no file";
test_command task list;

test_header "list: empty file";
test_prep    task init;
test_command task list;

test_header "list: non-empty file";
test_prep    task push FIRST_TASK;
test_command task list;

test_header "list: multi-line file";
test_prep    task push SECOND_TASK;
test_command task list;

test_header "list: errors";
test_command task list UNEXPECTED;



section_header "PUSH";
rm .tasks;

test_header "push: no file";
pretest_state task list
test_command  task push FIRST_TASK;
confirm_state task list

test_header "push: empty file";
test_prep     rm .tasks;
test_prep     task init;
pretest_state task list
test_command  task push FIRST_TASK;
confirm_state task list

test_header "push: non-empty file";
pretest_state task list
test_command  task push SECOND_TASK;
confirm_state task list;

test_header "push: errors: missing task";
test_command task push;

test_header "push: errors: unquoted long task";
test_command task push INVALID TASK;



section_header "POP";
rm .tasks;

test_header "pop: no file";
test_command task pop;

test_header "pop: empty file";
test_prep     task init;
pretest_state task list
test_command  task pop;
confirm_state task list

test_header "pop: multiple items in stack";
test_prep     task push FIRST_TASK;
test_prep     task push SECOND_TASK;
pretest_state task list
test_command  task pop;
confirm_state task list;

test_header "pop: last task in stack";
pretest_state task list
test_command  task pop;
confirm_state task list;

test_header "pop: errors";
test_prep     task push FIRST_TASK;
pretest_state task list
test_command  task pop FIRST_TASK;
confirm_state task list



section_header "INIT";
rm .tasks;

test_header "init: no file present";
pretest_state task list
test_command  task init;
confirm_state task list;

test_header "init: file already present";
test_prep     task push FIRST_TASK;
pretest_state task list
test_command  task init;
confirm_state task list;

test_header "init: errors";
test_prep     rm .tasks;
pretest_state task list
test_command  task init UNEXPECTED;
confirm_state task list;



section_header "HELP";

test_header "help: normal output";
test_command task help;

test_header "help: errors";
test_command task help UNEXPECTED;

section_header "GENERAL";

test_header "unrecognized command";
test_command task UNEXPECTED;

