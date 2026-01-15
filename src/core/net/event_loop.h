#ifndef PHOTONRPC_EVENT_LOOP_H
#define PHOTONRPC_EVENT_LOOP_H

#include <queue>

#include "poller.h"

class EventLoop {
 public:
  EventLoop();

  ~EventLoop();

  void Loop();

  void AddChannel(Channel* channel);

  void RemoveChannel(Channel* channel);

  void WakeUp();

  std::queue<std::function<void(int)>> work_queue_;

 private:
  Poller poller_;

  bool stopped_;

  int wakeup_fd_;

  Channel wakeup_channel_;

};

#endif  //PHOTONRPC_EVENT_LOOP_H