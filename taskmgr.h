#ifndef TASKMGR_H
#define TASKMGR_H

#include "timing.h"


#define MAX_AWAITING_TASKS 50


enum Task {
  Nothing,
  Close_Pozzo1,
  Close_Pozzo2,
  Close_Pozzo3,
  Open_Pozzo1,
  Open_Pozzo2,
  Open_Pozzo3,
  SetIdle
};

Task awaiting_tasks[MAX_AWAITING_TASKS];
DateTime awaiting_tasks_time[MAX_AWAITING_TASKS];  // When the tasks need to be executed

void taskmgr_init() {
  for(int i=0; i < MAX_AWAITING_TASKS; i++) {
    awaiting_tasks[i] = Nothing;
    awaiting_tasks_time[i] = DateTime(0);
  }
}


void taskmgr_add_task(Task task, DateTime when) {
  // Adding in first empty spot
  for(int i=0; i < MAX_AWAITING_TASKS; i++) {
    if(awaiting_tasks[i] == Nothing) {
      awaiting_tasks[i] = task;
      awaiting_tasks_time[i] = when;
      break;
    }
  }
}


void taskmgr_remove_task(int index) {
  awaiting_tasks[index] = Nothing;
  awaiting_tasks_time[index] = DateTime(0);
  int offset = 0;
  for(int i=index; i < MAX_AWAITING_TASKS; i++) {
    if(awaiting_tasks[i] != Nothing) {
      awaiting_tasks[index + offset] = awaiting_tasks[i];
      awaiting_tasks_time[index + offset] = awaiting_tasks_time[i];
      offset++;
    }
  }
}


void taskmgr_do_task(int index) {
  Task task = awaiting_tasks[index];
  switch(task) {
    case Nothing:
      break;
    case Close_Pozzo1:
      close_pozzo_1();
      break;
    case Close_Pozzo2:
      close_pozzo_2();
      break;
    case Close_Pozzo3:
      close_pozzo_3();
      break;
    case Open_Pozzo1:
      open_pozzo_1();
      break;
    case Open_Pozzo2:
      open_pozzo_2();
      break;
    case Open_Pozzo3:
      open_pozzo_3();
      break;
    case SetIdle:
      set_state_idle();
      break;
    default:
      Log(String("ERR| Asked to perform unknown task: ") + task + ".");
      break;
  }
}


void taskmgr_tick() {
  // This function should be invoked inside of loop.
  // Once every loop tick.
  for(int i=MAX_AWAITING_TASKS-1; i >= 0; i--) {
    if(awaiting_tasks[i] == Nothing)
      continue;
    if(rtc.now().unixtime() >= awaiting_tasks_time[i].unixtime()) {
      taskmgr_do_task(i);
      taskmgr_remove_task(i);
    }
  }
}



#endif
