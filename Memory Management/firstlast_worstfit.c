#include <stdio.h>

#define MAX 20

void input(int *, int *, int[], int[]);
void firstFit(int, int, int[], int[]);
void bestFit(int, int, int[], int[]);
void worstFit(int, int, int[], int[]);

int main()
{
    int nb, np;
    int block[MAX], process[MAX];

    input(&nb, &np, block, process);

    firstFit(nb, np, block, process);
    bestFit(nb, np, block, process);
    worstFit(nb, np, block, process);

    return 0;
}

void input(int *nb, int *np, int block[], int process[])
{
    printf("Enter number of memory blocks: ");
    scanf("%d", nb);

    printf("Enter number of processes: ");
    scanf("%d", np);

    printf("\nEnter sizes of %d memory blocks:\n", *nb);

    for (int i = 0; i < *nb; i++)
    {
        printf("Block %d: ", i + 1);
        scanf("%d", &block[i]);
    }

    printf("\nEnter sizes of %d processes:\n", *np);

    for (int i = 0; i < *np; i++)
    {
        printf("Process %d: ", i + 1);
        scanf("%d", &process[i]);
    }
}

void firstFit(int nb, int np, int block[], int process[])
{
    int b[MAX], allocation[MAX];

    for (int i = 0; i < nb; i++)
        b[i] = block[i];

    for (int i = 0; i < np; i++)
        allocation[i] = -1;

    for (int i = 0; i < np; i++)
    {
        for (int j = 0; j < nb; j++)
        {
            if (b[j] >= process[i])
            {
                allocation[i] = j;
                b[j] -= process[i];
                break;
            }
        }
    }

    printf("\nFirst Fit\n");
    printf("Process\tSize\tBlock\n");

    for (int i = 0; i < np; i++)
    {
        printf("P%d\t%d\t", i + 1, process[i]);

        if (allocation[i] != -1)
            printf("%d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

void bestFit(int nb, int np, int block[], int process[])
{
    int b[MAX], allocation[MAX];

    for (int i = 0; i < nb; i++)
        b[i] = block[i];

    for (int i = 0; i < np; i++)
        allocation[i] = -1;

    for (int i = 0; i < np; i++)
    {
        int best = -1;

        for (int j = 0; j < nb; j++)
        {
            if (b[j] >= process[i])
            {
                if (best == -1 || b[j] < b[best])
                    best = j;
            }
        }

        if (best != -1)
        {
            allocation[i] = best;
            b[best] -= process[i];
        }
    }

    printf("\nBest Fit\n");
    printf("Process\tSize\tBlock\n");

    for (int i = 0; i < np; i++)
    {
        printf("P%d\t%d\t", i + 1, process[i]);

        if (allocation[i] != -1)
            printf("%d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

void worstFit(int nb, int np, int block[], int process[])
{
    int b[MAX], allocation[MAX];

    for (int i = 0; i < nb; i++)
        b[i] = block[i];

    for (int i = 0; i < np; i++)
        allocation[i] = -1;

    for (int i = 0; i < np; i++)
    {
        int worst = -1;

        for (int j = 0; j < nb; j++)
        {
            if (b[j] >= process[i])
            {
                if (worst == -1 || b[j] > b[worst])
                    worst = j;
            }
        }

        if (worst != -1)
        {
            allocation[i] = worst;
            b[worst] -= process[i];
        }
    }

    printf("\nWorst Fit\n");
    printf("Process\tSize\tBlock\n");

    for (int i = 0; i < np; i++)
    {
        printf("P%d\t%d\t", i + 1, process[i]);

        if (allocation[i] != -1)
            printf("%d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}