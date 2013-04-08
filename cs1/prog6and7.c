#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

typedef struct Job {
  unsigned int id;
  unsigned int arrivalTime;
  unsigned int priority;
  unsigned int burstTime;
  unsigned int remainingTime;
  bool done;
} Job;

unsigned int time_width = 3;

int jobsN/* = 5*/;

Job jobs[100]/* = {
  {0, 8, 3, 8, 8, false},
  {1, 5, 1, 5, 5, false},
  {2, 0, 4, 12, 12, false},
  {3, 3, 5, 9, 9, false},
  {4, 9, 2, 7, 7, false}
}*/;

int total_burst_time = 0;

void sortJobsByArrivalTime() {
  int i, j;
  Job tmp;
  for (i = 1; i < jobsN; i++) {
    tmp = jobs[i];
    for (j = i; j > 0 && jobs[j-1].arrivalTime <= tmp.arrivalTime; j--) {
      jobs[j] = jobs[j-1];
    }
    jobs[j] = tmp;
  }
}

void printGCStart() {
  unsigned int i;
  printf(" --");
  for (i = 1; i < total_burst_time ; i++) {
    printf("---");
  }
  printf("\n|");
}

void printGCEnd() {
  unsigned int i;
  printf("\n");
  for (i = 0; i < total_burst_time ; i++) {
    printf("|--");
  }
  printf("|\n");
  for (i = 0; i <= total_burst_time ; i++) {
    printf("%-*u", time_width, i);
  }
  printf("\n\n");
}

void scheduleFCFS() {
  unsigned int i, x, time = 0;
  float y,
        avg_waiting_time = 0.0,
        avg_turnaround_time = 0.0;
  printf("First Come First Serve Scheduling:\n");
  printGCStart();
  for (i = 0; i < jobsN; i++) {
    x = jobs[i].burstTime * time_width;
    printf("%*u", x - (x/2), jobs[i].id);
    printf("%*c", x/2, '|' );
    y = (float)(time - jobs[i].arrivalTime) / jobsN;
    avg_waiting_time += y;
    avg_turnaround_time += y + (jobs[i].burstTime / jobsN);
    time += jobs[i].burstTime;
  }
  printGCEnd();
  printf("Average Waiting Time: %f\n", avg_waiting_time);
  printf("Average Turnaround Time: %f\n", avg_turnaround_time);
}

void scheduleSJF() {
  unsigned int time = 0, i, j, x, next = 0, nextMin;
  float y,
        avg_waiting_time = 0.0,
        avg_turnaround_time = 0.0;
  printf("Shortest Job First Scheduling:\n");
  printGCStart();
  for (i = 0; i < jobsN; i++) {
    jobs[i].done = false;
  }
  for (i = 0; i < jobsN; i++) {
    nextMin = INT_MAX;
    for (j = 0; j < jobsN && jobs[j].arrivalTime <= time; j++) {
      if(!jobs[j].done && jobs[j].burstTime < nextMin) {
        next = j;
        nextMin = jobs[j].burstTime;
      }
    }
    x = jobs[next].burstTime * time_width;
    printf("%*u", x - (x/2), jobs[next].id);
    printf("%*c", x/2, '|' );
    jobs[next].done = true;
    y = (float)(time - jobs[next].arrivalTime) / jobsN;
    avg_waiting_time += y;
    avg_turnaround_time += y + (jobs[next].burstTime / jobsN);
    time += jobs[next].burstTime;
  }
  printGCEnd();
  printf("Average Waiting Time: %f\n", avg_waiting_time);
  printf("Average Turnaround Time: %f\n", avg_turnaround_time);
}

void schedulePriority() {
  unsigned int time = 0, i, j, x, next = 0, nextMin;
  float y,
        avg_waiting_time = 0.0,
        avg_turnaround_time = 0.0;
  printf("Priority Scheduling:\n");
  printGCStart();
  for (i = 0; i < jobsN; i++) {
    jobs[i].done = false;
  }
  for (i = 0; i < jobsN; i++) {
    nextMin = INT_MAX;
    for (j = 0; jobs[j].arrivalTime <= time && j < jobsN; j++) {
      if(!jobs[j].done && jobs[j].priority < nextMin) {
        next = j;
        nextMin = jobs[next].priority;
      }
    }
    x = jobs[next].burstTime * time_width;
    printf("%*u", x - (x/2), jobs[next].id);
    printf("%*c", x/2, '|' );
    jobs[next].done = true;
    y = (float)(time - jobs[next].arrivalTime) / jobsN;
    avg_waiting_time += y;
    avg_turnaround_time += y + (jobs[next].burstTime / jobsN);
    time += jobs[next].burstTime;
  }
  printGCEnd();
  printf("Average Waiting Time: %f\n", avg_waiting_time);
  printf("Average Turnaround Time: %f\n", avg_turnaround_time);
}

void scheduleRR (unsigned int tq) {
  unsigned int time = 0, i, x, z, next = 0;
  float y,
        avg_waiting_time = 0.0,
        avg_turnaround_time = 0.0;
  printf("Round Robin Scheduling:\n");
  printGCStart();
  for (i = 0; i < jobsN; i++) {
    jobs[i].remainingTime = jobs[i].burstTime;
  }
  i = 0;
  while(time < total_burst_time) {
    if (jobs[i].arrivalTime <= time) {
      if (jobs[i].remainingTime) {
        z = (tq < jobs[i].remainingTime) ?tq :jobs[i].remainingTime;
        x = z * time_width;
        printf("%*u", x - (x/2), jobs[i].id);
        printf("%*c", x/2, '|' );
        jobs[i].done = true;
        y = (float)(time - jobs[i].arrivalTime) / jobsN;
        avg_waiting_time += y;
        avg_turnaround_time += y + (jobs[i].burstTime / jobsN);
        time += z;
        jobs[i].remainingTime -= z;
      }
      i = (i + 1) % jobsN;
    } else {
      i = 0;
    }
  }
  printGCEnd();
  printf("Average Waiting Time: %f\n", avg_waiting_time);
  printf("Average Turnaround Time: %f\n", avg_turnaround_time);
}

void scheduleSRTF() {
  int time = 0, i, j, x, next, pre = -1, preStart = 0, nextMin;
  float y,
        avg_waiting_time = 0.0,
        avg_turnaround_time = 0.0;
  printf("Shortest Remaining Time First Scheduling:\n");
  printGCStart();
  for (i = 0; i < jobsN; i++) {
    jobs[i].remainingTime = jobs[i].burstTime;
  }
  i = 0;
  while(time <= total_burst_time) {
    next = -1;
    nextMin = INT_MAX;
    for (j = 0; jobs[j].arrivalTime <= time && j < jobsN; j++) {
      i = j;
      if(jobs[j].remainingTime && jobs[j].remainingTime < nextMin) {
        next = j;
        nextMin = jobs[j].remainingTime;
        if (pre == -1) {
          pre = next;
        }
      }
    }
    i++;
    if(next != pre || time == total_burst_time) {
      x = (time - preStart);
      y = (float)(preStart - jobs[pre].arrivalTime) / jobsN;
      avg_waiting_time += y;
      avg_turnaround_time += y + (x / jobsN);
      x *= time_width;
      printf("%*u", x - (x/2), jobs[pre].id);
      printf("%*c", x/2, '|' );
      jobs[pre].done = true;
      pre = next;
      preStart = time;
      if (time == total_burst_time) {
        break;
      }
    }
    x = (i < jobsN) ?jobs[i].arrivalTime :total_burst_time;
    x = x - time;
    x = (x < jobs[next].remainingTime) ?x :jobs[next].remainingTime;
    jobs[next].remainingTime -= x;
    time += x;
  }
  printGCEnd();
  printf("Average Waiting Time: %f\n", avg_waiting_time);
  printf("Average Turnaround Time: %f\n", avg_turnaround_time);
}

int main(int argc, char const *argv[]) {
  int i, tq = 3;
  unsigned int ch;
  printf("Enter the number of jobs: ");
  scanf("%u", &jobsN);
  printf("\nEnter the details of jobs: ");
  for (i = 0; i < jobsN; i++) {
    printf("Job %u:\n", i+1);
    printf("  ID: ");
    scanf("%u", &jobs[i].id);
    printf("  Arrival Time: ");
    scanf("%u", &jobs[i].arrivalTime);
    printf("  Priority: ");
    scanf("%u", &jobs[i].priority);
    printf("  Burst Time: ");
    scanf("%u", &jobs[i].burstTime);
    jobs[i].remainingTime = jobs[i].burstTime;
    jobs[i].done = false;
    total_burst_time += jobs[i].burstTime;
  }
  sortJobsByArrivalTime();

  ch = true;
  while(ch) {
    printf("Select a Scheduling:\n");
    printf("1: First Come First Serve\n");
    printf("2: Shortest Job First\n");
    printf("3: Priority Scheduling\n");
    printf("4: Round Robin\n");
    printf("5: Shortest Remaining Time First\n");
    printf("0: Exit\n");
    printf("\nEnter your choice: ");
    fflush(stdin);
    scanf("%u", &ch);
    printf("\n");
    switch(ch) {
      case 1:
        scheduleFCFS();
        break;
      case 2:
        scheduleSJF();
        break;
      case 3:
        schedulePriority();
        break;
      case 4:
        printf("Enter the value of time quantum: \n");
        scanf("%u", &tq);
        scheduleRR(tq);
        break;
      case 5:
        scheduleSRTF();
        break;
      case 0:
        printf("Thank You...\n");
        break;
      default:
        printf("Invalid Choice!\n");
    }
    printf("\n\n");
  }
  return 0;
}