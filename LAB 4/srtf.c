#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct {
    int iPID;
    int iArrival, iBurst;
    int iRemaining;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[]) {
    for (int i = 0; i < n; i++) {
        printf("Enter process ID, Arrival time, Burst time for Process %d: ", i + 1);
        scanf("%d %d %d", &P[i].iPID, &P[i].iArrival, &P[i].iBurst);
        P[i].iRemaining = P[i].iBurst;
        P[i].iStart = P[i].iFinish = P[i].iWaiting = P[i].iResponse = P[i].iTaT = 0;
    }
}

void printProcess(int n, PCB P[]) {
    printf("PID\tArrival\tBurst\tStart\tFinish\tWaiting\tResponse\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t\t%d\n", P[i].iPID, P[i].iArrival, P[i].iBurst, P[i].iStart, P[i].iFinish, P[i].iWaiting, P[i].iResponse, P[i].iTaT);
    }
}

void exportGanttChart(int n, PCB Gantt[]) {
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < n; i++) {
        printf(" %d - %d | P%d |", Gantt[i].iStart, Gantt[i].iFinish, Gantt[i].iPID);
    }
    printf("\n");
}

void calculateAWT(int n, PCB P[]) {
    int totalWaiting = 0;
    for (int i = 0; i < n; i++) {
        totalWaiting += P[i].iWaiting;
    }
    printf("Average Waiting Time: %.2f\n", (float)totalWaiting / n);
}

void calculateATaT(int n, PCB P[]) {
    int totalTaT = 0;
    for (int i = 0; i < n; i++) {
        totalTaT += P[i].iTaT;
    }
    printf("Average Turnaround Time: %.2f\n", (float)totalTaT / n);
}

int findNextProcess(int n, PCB ReadyQueue[], int currentTime) {
    int shortestIndex = -1;
    int minRemaining = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (ReadyQueue[i].iArrival <= currentTime && ReadyQueue[i].iRemaining > 0 && ReadyQueue[i].iRemaining < minRemaining) {
            minRemaining = ReadyQueue[i].iRemaining;
            shortestIndex = i;
        }
    }
    return shortestIndex;
}

int main() {
    PCB Input[10], Gantt[50];
    int iNumberOfProcess, iGantt = 0;

    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);

    inputProcess(iNumberOfProcess, Input);

    int currentTime = 0, completed = 0;

    while (completed < iNumberOfProcess) {
        int index = findNextProcess(iNumberOfProcess, Input, currentTime);

        if (index == -1) {
            currentTime++;
            continue;
        }

        PCB *currentProcess = &Input[index];

        if (currentProcess->iRemaining == currentProcess->iBurst) {
            currentProcess->iStart = currentTime;
            currentProcess->iResponse = currentTime - currentProcess->iArrival;
        }

        currentTime++;
        currentProcess->iRemaining--;

        if (iGantt == 0 || Gantt[iGantt - 1].iPID != currentProcess->iPID) {
            Gantt[iGantt].iPID = currentProcess->iPID;
            Gantt[iGantt].iStart = currentTime - 1;
            Gantt[iGantt].iFinish = currentTime;
            iGantt++;
        } else {
            Gantt[iGantt - 1].iFinish = currentTime;
        }

        if (currentProcess->iRemaining == 0) {
            completed++;
            currentProcess->iFinish = currentTime;
            currentProcess->iTaT = currentProcess->iFinish - currentProcess->iArrival;
            currentProcess->iWaiting = currentProcess->iTaT - currentProcess->iBurst;
        }
    }

    printf("\n===== SRTF Scheduling =====\n");
    exportGanttChart(iGantt, Gantt);
    printProcess(iNumberOfProcess, Input);
    calculateAWT(iNumberOfProcess, Input);
    calculateATaT(iNumberOfProcess, Input);

    return 0;
}
