#pragma once
#include <stdlib.h>
#include "tools/sync_utils.hpp"

struct request {
  static const int max_buffer_size = 65536;
  int ocall_index;  // store the index of specific command, defined in defs.h
  unsigned char buffer[max_buffer_size]; // address of input1, input2, output
  volatile int is_done; // status of current request, finished=0, else=-1
  int resp;
};

class Queue {
 public:
  Queue();
  virtual ~Queue();
  int enqueue(request* elem);
  request* dequeue();
  int front, rear;

 private:
  static const int queue_size = 1024000;
  request* q[queue_size];
  int volatile _lock;
};
