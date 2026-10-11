/* =============================================================================
 *  MENU-DRIVEN OPERATING SYSTEM SIMULATOR
 *  ---------------------------------------------------------------------------
 *  Modules:
 *    1. CPU Scheduling      : FCFS, SJF (NP), SRTF (P-SJF), Priority (NP & P),
 *                              Round Robin
 *    2. Memory Allocation   : First Fit, Best Fit, Worst Fit
 *    3. Page Replacement    : FIFO, LRU, Optimal
 *    4. Disk Scheduling     : FCFS, SSTF, SCAN, C-SCAN, LOOK, C-LOOK
 *
 *  Final Semester Project - Operating Systems
 *  Compile : gcc -o os_simulator os.c
 *  Run     : ./os_simulator
 * ===========================================================================*/

#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAX      50
#define QMAX     5000

/* -------------------------------------------------------------------------
 *  GENERIC UTILITIES
 * -----------------------------------------------------------------------*/

void pauseScreen(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* flush leftover */ }
    printf("\nPress ENTER to continue...");
    getchar();
}

int absInt(int x) { return x < 0 ? -x : x; }

void printSectionHeader(const char *title) {
    int len = (int) strlen(title);
    printf("\n");
    for (int i = 0; i < len + 8; i++) printf("=");
    printf("\n    %s\n", title);
    for (int i = 0; i < len + 8; i++) printf("=");
    printf("\n");
}

/* =============================================================================
 *  MODULE 1 : CPU SCHEDULING
 * ===========================================================================*/

typedef struct {
    int pid;
    int at;         /* arrival time     */
    int bt;         /* burst time       */
    int rt;         /* remaining time   */
    int pr;         /* priority         */
    int ct;         /* completion time  */
    int tat;        /* turnaround time  */
    int wt;         /* waiting time     */
    int rsp;        /* response time    */
    int started;    /* has it started running yet */
} Process;

typedef struct {
    int pid;        /* -1 = CPU idle */
    int start;
    int end;
} Gantt;

int inputProcesses(Process p[], int needPriority) {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    if (n <= 0 || n > MAX) { printf("Invalid count. Using 1.\n"); n = 1; }

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("\n-- Process P%d --\n", i + 1);
        printf("  Arrival Time : ");
        scanf("%d", &p[i].at);
        printf("  Burst Time   : ");
        scanf("%d", &p[i].bt);
        p[i].rt = p[i].bt;
        if (needPriority) {
            printf("  Priority (lower number = higher priority): ");
            scanf("%d", &p[i].pr);
        } else {
            p[i].pr = 0;
        }
        p[i].started = 0;
        p[i].ct = p[i].tat = p[i].wt = p[i].rsp = 0;
    }
    return n;
}

void addToGantt(Gantt g[], int *gc, int pid, int t) {
    if (*gc > 0 && g[*gc - 1].pid == pid) {
        g[*gc - 1].end = t + 1;
    } else {
        g[*gc].pid = pid;
        g[*gc].start = t;
        g[*gc].end = t + 1;
        (*gc)++;
    }
}

void printGanttChart(Gantt g[], int gc) {
    printf("\nGantt Chart:\n ");
    for (int i = 0; i < gc; i++) printf("--------");
    printf("-\n|");
    for (int i = 0; i < gc; i++) {
        if (g[i].pid == -1) printf("  IDLE  |");
        else                printf("   P%-3d |", g[i].pid);
    }
    printf("\n ");
    for (int i = 0; i < gc; i++) printf("--------");
    printf("-\n%-3d", g[0].start);
    for (int i = 0; i < gc; i++) printf("%8d", g[i].end);
    printf("\n");
}

void printTable(Process p[], int n, int showPriority) {
    double totalTAT = 0, totalWT = 0, totalRT = 0;
    printf("\n%-5s%-8s%-8s", "PID", "AT", "BT");
    if (showPriority) printf("%-10s", "Priority");
    printf("%-8s%-8s%-8s%-10s\n", "CT", "TAT", "WT", "RT");

    for (int i = 0; i < n; i++) {
        printf("P%-4d%-8d%-8d", p[i].pid, p[i].at, p[i].bt);
        if (showPriority) printf("%-10d", p[i].pr);
        printf("%-8d%-8d%-8d%-10d\n", p[i].ct, p[i].tat, p[i].wt, p[i].rsp);
        totalTAT += p[i].tat;
        totalWT  += p[i].wt;
        totalRT  += p[i].rsp;
    }
    printf("\nAverage Turnaround Time = %.2f", totalTAT / n);
    printf("\nAverage Waiting Time    = %.2f", totalWT / n);
    printf("\nAverage Response Time   = %.2f\n", totalRT / n);
}

/* ---- 1.1 FCFS ---- */
void fcfs(void) {
    printSectionHeader("FCFS SCHEDULING");
    Process p[MAX];
    int n = inputProcesses(p, 0);

    /* stable sort by arrival time */
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (p[j].at > p[j + 1].at ||
               (p[j].at == p[j + 1].at && p[j].pid > p[j + 1].pid)) {
                Process tmp = p[j]; p[j] = p[j + 1]; p[j + 1] = tmp;
            }

    Gantt g[MAX]; int gc = 0;
    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].at) {
            g[gc].pid = -1; g[gc].start = time; g[gc].end = p[i].at; gc++;
            time = p[i].at;
        }
        p[i].rsp = time - p[i].at;
        g[gc].pid = p[i].pid; g[gc].start = time; g[gc].end = time + p[i].bt; gc++;
        time += p[i].bt;
        p[i].ct  = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt  = p[i].tat - p[i].bt;
    }
    printGanttChart(g, gc);
    printTable(p, n, 0);
    pauseScreen();
}

/* ---- 1.2 SJF Non-Preemptive ---- */
void sjfNonPreemptive(void) {
    printSectionHeader("SJF SCHEDULING (NON-PREEMPTIVE)");
    Process p[MAX];
    int n = inputProcesses(p, 0);
    int done[MAX] = {0};
    int completed = 0, time = 0;
    Gantt g[MAX]; int gc = 0;

    while (completed < n) {
        int idx = -1, minBT = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= time && p[i].bt < minBT) {
                minBT = p[i].bt; idx = i;
            }
        }
        if (idx == -1) {
            int nextAT = INT_MAX;
            for (int i = 0; i < n; i++)
                if (!done[i] && p[i].at < nextAT) nextAT = p[i].at;
            g[gc].pid = -1; g[gc].start = time; g[gc].end = nextAT; gc++;
            time = nextAT;
            continue;
        }
        p[idx].rsp = time - p[idx].at;
        g[gc].pid = p[idx].pid; g[gc].start = time; g[gc].end = time + p[idx].bt; gc++;
        time += p[idx].bt;
        p[idx].ct  = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt  = p[idx].tat - p[idx].bt;
        done[idx] = 1;
        completed++;
    }
    printGanttChart(g, gc);
    printTable(p, n, 0);
    pauseScreen();
}

/* ---- 1.3 SRTF (Preemptive SJF) ---- */
void srtf(void) {
    printSectionHeader("SRTF SCHEDULING (PREEMPTIVE SJF)");
    Process p[MAX];
    int n = inputProcesses(p, 0);
    int completed = 0, time = 0;
    Gantt g[MAX]; int gc = 0;

    while (completed < n) {
        int idx = -1, minRT = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && p[i].rt < minRT) {
                minRT = p[i].rt; idx = i;
            }
        }
        if (idx == -1) {
            addToGantt(g, &gc, -1, time);
            time++;
            continue;
        }
        if (!p[idx].started) { p[idx].rsp = time - p[idx].at; p[idx].started = 1; }
        addToGantt(g, &gc, p[idx].pid, time);
        p[idx].rt--;
        time++;
        if (p[idx].rt == 0) {
            p[idx].ct  = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt  = p[idx].tat - p[idx].bt;
            completed++;
        }
    }
    printGanttChart(g, gc);
    printTable(p, n, 0);
    pauseScreen();
}

/* ---- 1.4 Priority Non-Preemptive ---- */
void priorityNonPreemptive(void) {
    printSectionHeader("PRIORITY SCHEDULING (NON-PREEMPTIVE)");
    Process p[MAX];
    int n = inputProcesses(p, 1);
    int done[MAX] = {0};
    int completed = 0, time = 0;
    Gantt g[MAX]; int gc = 0;

    while (completed < n) {
        int idx = -1, minPR = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= time && p[i].pr < minPR) {
                minPR = p[i].pr; idx = i;
            }
        }
        if (idx == -1) {
            int nextAT = INT_MAX;
            for (int i = 0; i < n; i++)
                if (!done[i] && p[i].at < nextAT) nextAT = p[i].at;
            g[gc].pid = -1; g[gc].start = time; g[gc].end = nextAT; gc++;
            time = nextAT;
            continue;
        }
        p[idx].rsp = time - p[idx].at;
        g[gc].pid = p[idx].pid; g[gc].start = time; g[gc].end = time + p[idx].bt; gc++;
        time += p[idx].bt;
        p[idx].ct  = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt  = p[idx].tat - p[idx].bt;
        done[idx] = 1;
        completed++;
    }
    printGanttChart(g, gc);
    printTable(p, n, 1);
    pauseScreen();
}

/* ---- 1.5 Priority Preemptive ---- */
void priorityPreemptive(void) {
    printSectionHeader("PRIORITY SCHEDULING (PREEMPTIVE)");
    Process p[MAX];
    int n = inputProcesses(p, 1);
    int completed = 0, time = 0;
    Gantt g[MAX]; int gc = 0;

    while (completed < n) {
        int idx = -1, minPR = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && p[i].pr < minPR) {
                minPR = p[i].pr; idx = i;
            }
        }
        if (idx == -1) {
            addToGantt(g, &gc, -1, time);
            time++;
            continue;
        }
        if (!p[idx].started) { p[idx].rsp = time - p[idx].at; p[idx].started = 1; }
        addToGantt(g, &gc, p[idx].pid, time);
        p[idx].rt--;
        time++;
        if (p[idx].rt == 0) {
            p[idx].ct  = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt  = p[idx].tat - p[idx].bt;
            completed++;
        }
    }
    printGanttChart(g, gc);
    printTable(p, n, 1);
    pauseScreen();
}

/* ---- 1.6 Round Robin ---- */
void roundRobin(void) {
    printSectionHeader("ROUND ROBIN SCHEDULING");
    Process p[MAX];
    int n = inputProcesses(p, 0);
    int quantum;
    printf("Enter Time Quantum: ");
    scanf("%d", &quantum);
    if (quantum <= 0) quantum = 1;

    int order[MAX];
    for (int i = 0; i < n; i++) order[i] = i;
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (p[order[j]].at > p[order[j + 1]].at) {
                int tmp = order[j]; order[j] = order[j + 1]; order[j + 1] = tmp;
            }

    int queue[QMAX], front = 0, rear = 0;
    Gantt g[MAX * 20]; int gc = 0;
    int completed = 0, time, idx = 0;

    time = p[order[0]].at;
    while (idx < n && p[order[idx]].at <= time) queue[rear++] = order[idx++];

    while (completed < n) {
        if (front == rear) {
            if (idx < n) {
                time = p[order[idx]].at;
                while (idx < n && p[order[idx]].at <= time) queue[rear++] = order[idx++];
            }
            continue;
        }
        int cur = queue[front++];
        if (!p[cur].started) { p[cur].rsp = time - p[cur].at; p[cur].started = 1; }
        int exec = (p[cur].rt < quantum) ? p[cur].rt : quantum;
        int start = time;
        time += exec;
        p[cur].rt -= exec;
        g[gc].pid = p[cur].pid; g[gc].start = start; g[gc].end = time; gc++;

        while (idx < n && p[order[idx]].at <= time) queue[rear++] = order[idx++];

        if (p[cur].rt > 0) {
            queue[rear++] = cur;
        } else {
            p[cur].ct  = time;
            p[cur].tat = p[cur].ct - p[cur].at;
            p[cur].wt  = p[cur].tat - p[cur].bt;
            completed++;
        }
    }
    printGanttChart(g, gc);
    printTable(p, n, 0);
    pauseScreen();
}

void cpuMenu(void) {
    int choice;
    do {
        printSectionHeader("CPU SCHEDULING ALGORITHMS");
        printf("1. FCFS (First Come First Serve)\n");
        printf("2. SJF - Non-Preemptive\n");
        printf("3. SRTF - Preemptive SJF\n");
        printf("4. Priority Scheduling - Non-Preemptive\n");
        printf("5. Priority Scheduling - Preemptive\n");
        printf("6. Round Robin\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: fcfs(); break;
            case 2: sjfNonPreemptive(); break;
            case 3: srtf(); break;
            case 4: priorityNonPreemptive(); break;
            case 5: priorityPreemptive(); break;
            case 6: roundRobin(); break;
            case 0: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
}

/* =============================================================================
 *  MODULE 2 : MEMORY ALLOCATION
 * ===========================================================================*/

int inputBlocksAndProcesses(int blocks[], int *bn, int procs[], int *pn) {
    printf("Enter number of memory blocks: ");
    scanf("%d", bn);
    for (int i = 0; i < *bn; i++) {
        printf("  Block %d size: ", i + 1);
        scanf("%d", &blocks[i]);
    }
    printf("Enter number of processes: ");
    scanf("%d", pn);
    for (int i = 0; i < *pn; i++) {
        printf("  Process %d size: ", i + 1);
        scanf("%d", &procs[i]);
    }
    return 0;
}

void printAllocation(int blocks[], int procs[], int pn, int allocation[]) {
    int totalFrag = 0, allocated = 0;
    printf("\n%-10s%-10s%-14s%-14s%-10s\n", "Process", "Size", "Block No.", "Block Size", "Frag");
    for (int i = 0; i < pn; i++) {
        printf("P%-9d%-10d", i + 1, procs[i]);
        if (allocation[i] != -1) {
            int b = allocation[i];
            printf("%-14d%-14d%-10d\n", b + 1, blocks[b], blocks[b] - procs[i]);
            totalFrag += blocks[b] - procs[i];
            allocated++;
        } else {
            printf("%-14s%-14s%-10s\n", "-", "-", "Not Allocated");
        }
    }
    printf("\nProcesses Allocated       : %d / %d\n", allocated, pn);
    printf("Total Internal Fragmentation : %d\n", totalFrag);
}

void firstFit(void) {
    printSectionHeader("FIRST FIT MEMORY ALLOCATION");
    int blocks[MAX], bn, procs[MAX], pn;
    inputBlocksAndProcesses(blocks, &bn, procs, &pn);
    int allocation[MAX], used[MAX] = {0};
    for (int i = 0; i < pn; i++) {
        allocation[i] = -1;
        for (int j = 0; j < bn; j++) {
            if (!used[j] && blocks[j] >= procs[i]) {
                allocation[i] = j;
                used[j] = 1;
                break;
            }
        }
    }
    printAllocation(blocks, procs, pn, allocation);
    pauseScreen();
}

void bestFit(void) {
    printSectionHeader("BEST FIT MEMORY ALLOCATION");
    int blocks[MAX], bn, procs[MAX], pn;
    inputBlocksAndProcesses(blocks, &bn, procs, &pn);
    int allocation[MAX], used[MAX] = {0};
    for (int i = 0; i < pn; i++) {
        allocation[i] = -1;
        int best = -1;
        for (int j = 0; j < bn; j++) {
            if (!used[j] && blocks[j] >= procs[i]) {
                if (best == -1 || blocks[j] < blocks[best]) best = j;
            }
        }
        if (best != -1) { allocation[i] = best; used[best] = 1; }
    }
    printAllocation(blocks, procs, pn, allocation);
    pauseScreen();
}

void worstFit(void) {
    printSectionHeader("WORST FIT MEMORY ALLOCATION");
    int blocks[MAX], bn, procs[MAX], pn;
    inputBlocksAndProcesses(blocks, &bn, procs, &pn);
    int allocation[MAX], used[MAX] = {0};
    for (int i = 0; i < pn; i++) {
        allocation[i] = -1;
        int worst = -1;
        for (int j = 0; j < bn; j++) {
            if (!used[j] && blocks[j] >= procs[i]) {
                if (worst == -1 || blocks[j] > blocks[worst]) worst = j;
            }
        }
        if (worst != -1) { allocation[i] = worst; used[worst] = 1; }
    }
    printAllocation(blocks, procs, pn, allocation);
    pauseScreen();
}

void memoryMenu(void) {
    int choice;
    do {
        printSectionHeader("MEMORY ALLOCATION ALGORITHMS");
        printf("1. First Fit\n");
        printf("2. Best Fit\n");
        printf("3. Worst Fit\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: firstFit(); break;
            case 2: bestFit(); break;
            case 3: worstFit(); break;
            case 0: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
}

/* =============================================================================
 *  MODULE 3 : PAGE REPLACEMENT
 * ===========================================================================*/

int inputRefString(int ref[]) {
    int n;
    printf("Enter number of pages in reference string: ");
    scanf("%d", &n);
    printf("Enter the reference string (space separated): ");
    for (int i = 0; i < n; i++) scanf("%d", &ref[i]);
    return n;
}

void printFrameState(int mem[], int frames, int hit) {
    printf("\t[ ");
    for (int j = 0; j < frames; j++) {
        if (mem[j] == -1) printf("- "); else printf("%d ", mem[j]);
    }
    printf("]\t%s", hit ? "Hit" : "FAULT");
}

void fifoPage(void) {
    printSectionHeader("FIFO PAGE REPLACEMENT");
    int ref[MAX];
    int n = inputRefString(ref);
    int frames;
    printf("Enter number of frames: ");
    scanf("%d", &frames);

    int mem[MAX];
    for (int i = 0; i < frames; i++) mem[i] = -1;
    int front = 0, faults = 0;

    printf("\nRef\tFrame State\t\tStatus\n");
    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < frames; j++) if (mem[j] == ref[i]) { found = 1; break; }
        if (!found) {
            mem[front] = ref[i];
            front = (front + 1) % frames;
            faults++;
        }
        printf("%d", ref[i]);
        printFrameState(mem, frames, found);
        printf("\n");
    }
    printf("\nTotal Page Faults = %d", faults);
    printf("\nTotal Page Hits   = %d", n - faults);
    printf("\nHit Ratio         = %.2f%%\n", (double)(n - faults) * 100 / n);
    pauseScreen();
}

void lruPage(void) {
    printSectionHeader("LRU PAGE REPLACEMENT");
    int ref[MAX];
    int n = inputRefString(ref);
    int frames;
    printf("Enter number of frames: ");
    scanf("%d", &frames);

    int mem[MAX], lastUsed[MAX];
    for (int i = 0; i < frames; i++) { mem[i] = -1; lastUsed[i] = -1; }
    int faults = 0;

    printf("\nRef\tFrame State\t\tStatus\n");
    for (int i = 0; i < n; i++) {
        int found = -1;
        for (int j = 0; j < frames; j++) if (mem[j] == ref[i]) { found = j; break; }
        if (found != -1) {
            lastUsed[found] = i;
        } else {
            int emptyIdx = -1;
            for (int j = 0; j < frames; j++) if (mem[j] == -1) { emptyIdx = j; break; }
            if (emptyIdx != -1) {
                mem[emptyIdx] = ref[i]; lastUsed[emptyIdx] = i;
            } else {
                int lruIdx = 0;
                for (int j = 1; j < frames; j++)
                    if (lastUsed[j] < lastUsed[lruIdx]) lruIdx = j;
                mem[lruIdx] = ref[i]; lastUsed[lruIdx] = i;
            }
            faults++;
        }
        printf("%d", ref[i]);
        printFrameState(mem, frames, found != -1);
        printf("\n");
    }
    printf("\nTotal Page Faults = %d", faults);
    printf("\nTotal Page Hits   = %d", n - faults);
    printf("\nHit Ratio         = %.2f%%\n", (double)(n - faults) * 100 / n);
    pauseScreen();
}

void optimalPage(void) {
    printSectionHeader("OPTIMAL PAGE REPLACEMENT");
    int ref[MAX];
    int n = inputRefString(ref);
    int frames;
    printf("Enter number of frames: ");
    scanf("%d", &frames);

    int mem[MAX];
    for (int i = 0; i < frames; i++) mem[i] = -1;
    int faults = 0;

    printf("\nRef\tFrame State\t\tStatus\n");
    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < frames; j++) if (mem[j] == ref[i]) { found = 1; break; }
        if (!found) {
            int emptyIdx = -1;
            for (int j = 0; j < frames; j++) if (mem[j] == -1) { emptyIdx = j; break; }
            if (emptyIdx != -1) {
                mem[emptyIdx] = ref[i];
            } else {
                int replaceIdx = 0, farthest = -1;
                for (int j = 0; j < frames; j++) {
                    int nextUse = INT_MAX;
                    for (int k = i + 1; k < n; k++) {
                        if (ref[k] == mem[j]) { nextUse = k; break; }
                    }
                    if (nextUse == INT_MAX) { replaceIdx = j; break; }
                    if (nextUse > farthest) { farthest = nextUse; replaceIdx = j; }
                }
                mem[replaceIdx] = ref[i];
            }
            faults++;
        }
        printf("%d", ref[i]);
        printFrameState(mem, frames, found);
        printf("\n");
    }
    printf("\nTotal Page Faults = %d", faults);
    printf("\nTotal Page Hits   = %d", n - faults);
    printf("\nHit Ratio         = %.2f%%\n", (double)(n - faults) * 100 / n);
    pauseScreen();
}

void pageMenu(void) {
    int choice;
    do {
        printSectionHeader("PAGE REPLACEMENT ALGORITHMS");
        printf("1. FIFO\n");
        printf("2. LRU (Least Recently Used)\n");
        printf("3. Optimal\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: fifoPage(); break;
            case 2: lruPage(); break;
            case 3: optimalPage(); break;
            case 0: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
}

/* =============================================================================
 *  MODULE 4 : DISK SCHEDULING
 * ===========================================================================*/

int inputRequests(int req[]) {
    int n;
    printf("Enter number of disk requests: ");
    scanf("%d", &n);
    printf("Enter cylinder requests (space separated): ");
    for (int i = 0; i < n; i++) scanf("%d", &req[i]);
    return n;
}

void sortAsc(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (arr[j] > arr[j + 1]) {
                int t = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = t;
            }
}

void printSeekResult(int totalSeek, int n) {
    printf("\n\nTotal Seek Time   = %d cylinders", totalSeek);
    printf("\nAverage Seek Time = %.2f cylinders\n", (double) totalSeek / n);
}

void fcfsDisk(void) {
    printSectionHeader("FCFS DISK SCHEDULING");
    int req[MAX];
    int n = inputRequests(req);
    int head;
    printf("Enter initial head position: ");
    scanf("%d", &head);

    int cur = head, totalSeek = 0;
    printf("\nSeek Sequence: %d", head);
    for (int i = 0; i < n; i++) {
        totalSeek += absInt(req[i] - cur);
        cur = req[i];
        printf(" -> %d", cur);
    }
    printSeekResult(totalSeek, n);
    pauseScreen();
}

void sstfDisk(void) {
    printSectionHeader("SSTF DISK SCHEDULING");
    int req[MAX];
    int n = inputRequests(req);
    int head;
    printf("Enter initial head position: ");
    scanf("%d", &head);

    int visited[MAX] = {0};
    int cur = head, totalSeek = 0;
    printf("\nSeek Sequence: %d", head);
    for (int c = 0; c < n; c++) {
        int minDist = INT_MAX, idx = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                int d = absInt(req[j] - cur);
                if (d < minDist) { minDist = d; idx = j; }
            }
        }
        visited[idx] = 1;
        totalSeek += minDist;
        cur = req[idx];
        printf(" -> %d", cur);
    }
    printSeekResult(totalSeek, n);
    pauseScreen();
}

void scanDisk(void) {
    printSectionHeader("SCAN (ELEVATOR) DISK SCHEDULING");
    int req[MAX];
    int n = inputRequests(req);
    int head, maxCyl;
    char dir[10];
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter maximum cylinder / disk size (e.g. 199): ");
    scanf("%d", &maxCyl);
    printf("Enter direction (up/down): ");
    scanf("%s", dir);

    sortAsc(req, n);
    int cur = head, totalSeek = 0;
    printf("\nSeek Sequence: %d", head);

    if (strcmp(dir, "up") == 0) {
        for (int i = 0; i < n; i++)
            if (req[i] >= head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
        if (cur != maxCyl) { totalSeek += absInt(maxCyl - cur); cur = maxCyl; printf(" -> %d", cur); }
        for (int i = n - 1; i >= 0; i--)
            if (req[i] < head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
    } else {
        for (int i = n - 1; i >= 0; i--)
            if (req[i] <= head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
        if (cur != 0) { totalSeek += absInt(0 - cur); cur = 0; printf(" -> %d", cur); }
        for (int i = 0; i < n; i++)
            if (req[i] > head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
    }
    printSeekResult(totalSeek, n);
    pauseScreen();
}

void cscanDisk(void) {
    printSectionHeader("C-SCAN DISK SCHEDULING");
    int req[MAX];
    int n = inputRequests(req);
    int head, maxCyl;
    char dir[10];
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter maximum cylinder / disk size (e.g. 199): ");
    scanf("%d", &maxCyl);
    printf("Enter direction (up/down): ");
    scanf("%s", dir);

    sortAsc(req, n);
    int cur = head, totalSeek = 0;
    printf("\nSeek Sequence: %d", head);

    if (strcmp(dir, "up") == 0) {
        for (int i = 0; i < n; i++)
            if (req[i] >= head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
        totalSeek += absInt(maxCyl - cur); cur = maxCyl; printf(" -> %d", cur);
        totalSeek += absInt(maxCyl - 0);   cur = 0;      printf(" -> %d", cur);
        for (int i = 0; i < n; i++)
            if (req[i] < head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
    } else {
        for (int i = n - 1; i >= 0; i--)
            if (req[i] <= head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
        totalSeek += absInt(cur - 0);      cur = 0;      printf(" -> %d", cur);
        totalSeek += absInt(maxCyl - 0);   cur = maxCyl; printf(" -> %d", cur);
        for (int i = n - 1; i >= 0; i--)
            if (req[i] > head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
    }
    printSeekResult(totalSeek, n);
    printf("(Note: total includes the return jump traversal, per standard convention.)\n");
    pauseScreen();
}

void lookDisk(void) {
    printSectionHeader("LOOK DISK SCHEDULING");
    int req[MAX];
    int n = inputRequests(req);
    int head;
    char dir[10];
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter direction (up/down): ");
    scanf("%s", dir);

    sortAsc(req, n);
    int cur = head, totalSeek = 0;
    printf("\nSeek Sequence: %d", head);

    if (strcmp(dir, "up") == 0) {
        for (int i = 0; i < n; i++)
            if (req[i] >= head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
        for (int i = n - 1; i >= 0; i--)
            if (req[i] < head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
    } else {
        for (int i = n - 1; i >= 0; i--)
            if (req[i] <= head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
        for (int i = 0; i < n; i++)
            if (req[i] > head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
    }
    printSeekResult(totalSeek, n);
    pauseScreen();
}

void clookDisk(void) {
    printSectionHeader("C-LOOK DISK SCHEDULING");
    int req[MAX];
    int n = inputRequests(req);
    int head;
    char dir[10];
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter direction (up/down): ");
    scanf("%s", dir);

    sortAsc(req, n);
    int cur = head, totalSeek = 0;
    printf("\nSeek Sequence: %d", head);

    if (strcmp(dir, "up") == 0) {
        int firstUpIdx = -1;
        for (int i = 0; i < n; i++)
            if (req[i] >= head) {
                if (firstUpIdx == -1) firstUpIdx = i;
                totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur);
            }
        for (int i = 0; i < n; i++) {
            if (req[i] < head) {
                totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur);
            }
        }
    } else {
        for (int i = n - 1; i >= 0; i--)
            if (req[i] <= head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
        for (int i = n - 1; i >= 0; i--)
            if (req[i] > head) { totalSeek += absInt(req[i] - cur); cur = req[i]; printf(" -> %d", cur); }
    }
    printSeekResult(totalSeek, n);
    pauseScreen();
}

void diskMenu(void) {
    int choice;
    do {
        printSectionHeader("DISK SCHEDULING ALGORITHMS");
        printf("1. FCFS\n");
        printf("2. SSTF\n");
        printf("3. SCAN (Elevator)\n");
        printf("4. C-SCAN\n");
        printf("5. LOOK\n");
        printf("6. C-LOOK\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: fcfsDisk(); break;
            case 2: sstfDisk(); break;
            case 3: scanDisk(); break;
            case 4: cscanDisk(); break;
            case 5: lookDisk(); break;
            case 6: clookDisk(); break;
            case 0: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
}

/* =============================================================================
 *  MAIN MENU
 * ===========================================================================*/

void showBanner(void) {
    printf("\n");
    printf("############################################################\n");
    printf("#                                                          #\n");
    printf("#      OPERATING SYSTEM SIMULATOR  -  FINAL PROJECT        #\n");
    printf("#      CPU Scheduling | Memory | Paging | Disk Scheduling  #\n");
    printf("#                                                          #\n");
    printf("############################################################\n");
}

int main(void) {
    int choice;
    showBanner();
    do {
        printf("\n================ MAIN MENU ================\n");
        printf("1. CPU Scheduling Algorithms\n");
        printf("2. Memory Allocation Algorithms\n");
        printf("3. Page Replacement Algorithms\n");
        printf("4. Disk Scheduling Algorithms\n");
        printf("0. Exit\n");
        printf("=============================================\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) { break; }

        switch (choice) {
            case 1: cpuMenu(); break;
            case 2: memoryMenu(); break;
            case 3: pageMenu(); break;
            case 4: diskMenu(); break;
            case 0: printf("\nExiting Simulator. Thank you!\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 0);

    return 0;
}