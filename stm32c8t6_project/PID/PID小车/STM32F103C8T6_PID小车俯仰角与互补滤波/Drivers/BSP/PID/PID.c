#include "PID.h"

/**
  * 函    数：PID 控制器初始化
  * 参    数：pid       PID 句柄指针
  *           kp, ki, kd PID 参数
  *           filtCoef  输出滤波系数（0~1，越大响应越快，0=无滤波）
  *           outMin/Max 输出限幅
  *           intMin/Max 积分限幅
  * 返 回 值：无
  */
void PID_Init(PID_HandleTypeDef *pid,
              float kp, float ki, float kd,
              float filtCoef,
              float outMin, float outMax,
              float intMin, float intMax)
{
    /* 参数赋值 */
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;

    /* 输入/输出清零 */
    pid->Target    = 0.0f;
    pid->Actual    = 0.0f;
    pid->Out       = 0.0f;
    pid->OutFilt   = 0.0f;

    /* 内部状态清零 */
    pid->Err0       = 0.0f;
    pid->Err1       = 0.0f;
    pid->ErrInt     = 0.0f;
    pid->ActualPrev = 0.0f;

    /* 限幅参数 */
    pid->OutMax = outMax;
    pid->OutMin = outMin;
    pid->IntMax = intMax;
    pid->IntMin = intMin;

    /* 滤波系数 */
    pid->FiltCoef = filtCoef;
}

/**
  * 函    数：设置 PID 目标值
  * 参    数：pid     PID 句柄指针
  *           target  目标值
  * 返 回 值：无
  */
void PID_SetTarget(PID_HandleTypeDef *pid, float target)
{
    pid->Target = target;
}

/**
  * 函    数：PID 计算（微分先行 + 积分抗饱和 + 输出滤波）
  * 参    数：pid     PID 句柄指针
  *           actual  当前实际值（反馈值）
  * 返 回 值：滤波后的 PID 输出
  * 说    明：每次调用执行一次 PID 迭代
  */
float PID_Compute(PID_HandleTypeDef *pid, float actual)
{
    float output;

    pid->Actual = actual;

    /* ---- 1. 误差计算 ---- */
    pid->Err1 = pid->Err0;                          /* 保存上一次误差 */
    pid->Err0 = pid->Target - pid->Actual;         /* 计算当前误差 */

    /* ---- 2. 积分累加 + 抗饱和钳位 ---- */
    pid->ErrInt += pid->Err0;
    if (pid->ErrInt > pid->IntMax) pid->ErrInt = pid->IntMax;
    if (pid->ErrInt < pid->IntMin) pid->ErrInt = pid->IntMin;
    if (pid->Ki == 0.0f) pid->ErrInt = 0.0f;        /* Ki=0 时清零积分 */

    /* ---- 3. PID 计算（微分先行：微分项用实际值的变化量）---- */
    pid->Out = pid->Kp * pid->Err0
             + pid->Ki * pid->ErrInt
             + pid->Kd * (pid->ActualPrev - pid->Actual);

    pid->ActualPrev = pid->Actual;                  /* 保存本次实际值 */

    /* ---- 4. 输出一阶低通滤波 ---- */
    pid->OutFilt = pid->OutFilt * (1.0f - pid->FiltCoef)
                 + pid->Out * pid->FiltCoef;

    /* ---- 5. 输出限幅 ---- */
    output = pid->OutFilt;
    if (output > pid->OutMax) output = pid->OutMax;
    if (output < pid->OutMin) output = pid->OutMin;

    return output;
}

/**
  * 函    数：PID 复位（清零积分和内部状态，保持参数不变）
  * 参    数：pid  PID 句柄指针
  * 返 回 值：无
  */
void PID_Reset(PID_HandleTypeDef *pid)
{
    pid->Err0       = 0.0f;
    pid->Err1       = 0.0f;
    pid->ErrInt     = 0.0f;
    pid->ActualPrev = 0.0f;
    pid->Out        = 0.0f;
    pid->OutFilt    = 0.0f;
}
