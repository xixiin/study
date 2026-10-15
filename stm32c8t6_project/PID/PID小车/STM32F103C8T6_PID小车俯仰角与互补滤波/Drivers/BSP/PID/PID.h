#ifndef __PID_H
#define __PID_H

#include "sys.h"

/* PID 控制器句柄结构体 */
typedef struct
{
    /* ---- 参数（用户可在线修改）---- */
    float Kp;
    float Ki;
    float Kd;

    /* ---- 输入 / 输出 ---- */
    float Target;       /* 目标值（设定值） */
    float Actual;       /* 实际值（反馈值） */
    float Out;          /* 本次原始输出（滤波前） */
    float OutFilt;      /* 低通滤波后的输出 */

    /* ---- 内部状态 ---- */
    float Err0;         /* 当前误差 */
    float Err1;         /* 上一次误差 */
    float ErrInt;       /* 积分累加 */
    float ActualPrev;   /* 上一次实际值（微分先行） */

    /* ---- 限幅 ---- */
    float OutMax;       /* 输出上限 */
    float OutMin;       /* 输出下限 */
    float IntMax;       /* 积分上限 */
    float IntMin;       /* 积分下限 */

    /* ---- 输出滤波系数（0~1，越大响应越快）---- */
    float FiltCoef;
} PID_HandleTypeDef;

/* 函数声明 */
void PID_Init(PID_HandleTypeDef *pid,
              float kp, float ki, float kd,
              float filtCoef,
              float outMin, float outMax,
              float intMin, float intMax);

void PID_SetTarget(PID_HandleTypeDef *pid, float target);
float PID_Compute(PID_HandleTypeDef *pid, float actual);
void PID_Reset(PID_HandleTypeDef *pid);

#endif
