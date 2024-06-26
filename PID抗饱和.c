#include<stdio.h>
#include<stdlib.h>
int target_x = 0, now_x = 0;
int target_y = 0, now_y = 0;
struct _pid {
    float SetSpeed;            //定义设定值
    float ActualSpeed;        //定义实际值
    float err;                //定义偏差值
    float err_last;            //定义上一个偏差值
    float Kp, Ki, Kd;            //定义比例、积分、微分系数
    float voltage;            //定义电压值（控制执行器的变量）
    float integral;            //定义积分值
    float umax;
    float umin;
}pid_x,pid_y;

void PID_init(struct _pid* pid) {
    pid->SetSpeed = 0.0;
    pid->ActualSpeed = 0.0;
    pid->err = 0.0;
    pid->err_last = 0.0;
    pid->voltage = 0.0;
    pid->integral = 0.0;
    pid->Kp = 0.2;
    pid->Ki = 0.1;       //注意，和上几次相比，这里加大了积分环节的值
    pid->Kd = 0.2;
    pid->umax = 400;
    pid->umin = -200;
}
float PID_realize(struct _pid* pid,float speed) {
    int index;
    pid->SetSpeed = speed;
    pid->err = pid->SetSpeed - pid->ActualSpeed;

    if (pid->ActualSpeed > pid->umax)  //灰色底色表示抗积分饱和的实现
    {

        if (abs(pid->err) > 200)      //蓝色标注为积分分离过程
        {
            index = 0;
        }
        else {
            index = 1;
            if (pid->err < 0)
            {
                pid->integral += pid->err;
            }
        }
    }
    else if (pid->ActualSpeed < pid->umin) {
        if (abs(pid->err) > 200)      //积分分离过程
        {
            index = 0;
        }
        else {
            index = 1;
            if (pid->err > 0)
            {
                pid->integral += pid->err;
            }
        }
    }
    else {
        if (abs(pid->err) > 200)                    //积分分离过程
        {
            index = 0;
        }
        else {
            index = 1;
            pid->integral += pid->err;
        }
    }

    pid->voltage = pid->Kp * pid->err + index * pid->Ki * pid->integral / 2 + pid->Kd * (pid->err - pid->err_last);  //梯形积分
    pid->err_last = pid->err;
    pid->ActualSpeed = pid->voltage * 1.0;
    return pid->ActualSpeed;
}

//int main() {
//    PID_init(&pid_x);
//    int count = 0;
//    target_x = 73;
//    now_x = 116;
//    while (count < 1000)
//    {
//        float speed = PID_realize(&pid_x, target_x - now_x);
//        printf("%f\n", speed);
//        count++;
//    }
//    return 0;
//}