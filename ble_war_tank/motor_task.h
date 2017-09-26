#ifndef __MOTOR_TASK_H__
#define __MOTOR_TASK_H__

typedef struct button {
  int butt_num;
  boolean pressed;
} BUTTON;

class motor_task {
public:
  motor_task(void);
  int create_task(void);

// private:
//   int motor_control(BUTTON *button);
};

#endif // __MOTOR_TASK_H__
