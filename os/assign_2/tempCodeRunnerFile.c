
    int childStatus;
    if (fork_id > 0)
        for (int i = 0; i < number; i++)
            printf("PARENT:%d\n", i);
    else
    {
        wait(&childStatus);
        printf("Child Status: %d", childStatus);