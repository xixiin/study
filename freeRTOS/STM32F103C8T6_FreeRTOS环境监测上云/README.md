# 基于 FreeRTOS 的多任务智能环境监测终端（上云）

> 目标：用 FreeRTOS 实时内核，把「温湿度/烟雾采集、OLED 实时显示、云平台上报、本地声光报警、按键调参」拆成**多个任务并发**，并用**消息队列、事件组、互斥量、软件定时器、任务通知**把它们正确同步。

这是配套你现有模板 `STM32F103C8T6_Template` 的**工程骨架**。骨架把 RTOS 结构全部搭好，外设驱动通过 `app_port.c` 薄封装调用你已有的驱动，你只需把对应驱动文件拷进工程即可。

---

## 一、目录结构（把下列文件放进你的模板工程）

```
STM32F103C8T6_FreeRTOS环境监测上云/
├── Users/
│   └── main.c                 ← 替换模板的 Users/main.c
└── freeRTOS/
    ├── FreeRTOSConfig.h       ← 保留模板原 config，加 3 行配置（见下）
    ├── FreeRTOS_demo.h        ← 骨架：任务声明 + 软定时器回调 + ISR 通知
    ├── FreeRTOS_demo.c        ← 骨架：任务/队列/事件组/互斥量/定时器 全在这
    ├── app_port.h             ← 驱动接口定义（Sample_t、事件位、函数声明）
    └── app_port.c             ← 驱动薄封装：调用你已有的 DHT11/ADC/OLED/KEY/ESP8266/MQTT
```

### FreeRTOS 内核依赖的 driver 文件（从你现有工程拷贝到本工程并加入 Include 路径）

| 文件 | 来源工程 | 备注 |
|---|---|---|
| `dht11.c/.h` | `stm32c8t6_project/STM32F103C8T6_DHT11` | 温湿度 |
| `adc.c/.h`（ADC_GetValue） | `stm32c8t6_project/STM32F103C8T6_烟雾报警系统` | 烟雾浓度 |
| `OLED.c/.h` | 你模板已有 | 显示 |
| `key.c/.h` | 你模板已有 | 按键 |
| `esp8266.c/.h` | 你模板已有 / `温湿度上传onenect` | 联网 |
| `onenet.c/.h`（或你的 mqtt 封装） | `stm32c8t6_project/STM32F103C8T6_温湿度上传onenect` | MQTT 上云 |

> `app_port.c` 里调用的函数名（`dht11_read`、`ADC_GetValue`、`OLED_*`、`mqtt_publish_data` 等）为**你现有工程的命名**；如果某些名字不同，按实际 `app_port.c` 里标注的 TODO 改一行即可。

---

## 二、FreeRTOSConfig.h 需要补的 3 行

你的 `FreeRTOSConfig.h` 目前**没有启用软件定时器**，在文件末尾 `#endif` 前加上：

```c
#define configUSE_TIMERS              1
#define configTIMER_TASK_PRIORITY     ( 2 )
#define configTIMER_QUEUE_LENGTH      4
#define configTIMER_TASK_STACK_DEPTH  ( 256 )
```

> 同时在 `FreeRTOS_demo.c` 已提供 `vApplicationGetTimerTaskMemory`（静态分配 timer 任务内存），无需再改。

## 三、按键外部中断 → 任务通知

在 `stm32f1xx_it.c` 的按键 EXTI 中断服务函数里调用（骨架已声明）：

```c
extern void FreeRTOS_KeyISR(void);
void EXTI4_IRQHandler(void)          /* 以你的实际 EXTI 引脚为准 */
{
    if(EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        FreeRTOS_KeyISR();           /* 内部 vTaskNotifyGiveFromISR 唤醒 key 任务 */
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}
```

---

## 四、RTOS 结构与职责

| 任务 | 优先级(0-4) | 周期/触发 | 职责 |
|---|---|---|---|
| `SensorTask` | 3 | `vTaskDelayUntil` 500ms | 采集温湿度+烟雾，判断越限置**事件组**，**队列**发到显示/上云 |
| `DisplayTask` | 2 | 队列有数据 | 取**互斥量**保护 OLED 后刷新显示 |
| `CommTask` | 1 | 队列有数据 | 组 JSON → MQTT 上云 |
| `AlarmTask` | 2 | 事件组 | 越限时闪 LED/蜂鸣器 |
| `KeyTask` | 1 | **任务通知**(EXTI) | 切显示页 / 调阈值 |

**用到并在代码中体现的内核机制：**优先级抢占、`vTaskDelayUntil` 固定周期、`xQueueSend/Receive`、`xEventGroupSetBits/WaitBits`、`xSemaphoreCreateMutex`、`xTimerCreate` 软件定时器（MQTT 心跳）、`vTaskNotifyGiveFromISR` / `ulTaskNotifyTake`。

---

## 五、分阶段开发（保证做得出）

1. **跑通 2 任务**：SensorTask + DisplayTask，队列传数据，OLED 能刷新温湿度。
2. **加 AlarmTask**：事件组触发声光报警，测阈值逻辑。
3. **加 KeyTask**：EXTI 中断 + 任务通知，能切页/调阈值。
4. **加 CommTask**：复用你 ESP8266/MQTT 代码，队列取数据上云，软定时器做心跳。
5. **调优先级 + 联调**：验证不丢数据、不卡顿、及时响应；必要时调大 `configTOTAL_HEAP_SIZE`（17KB 偏紧，可改 24KB）。

---

## 六、可直接写进简历的项目描述

> **项目：基于 FreeRTOS 的多任务智能环境监测终端**
> 开发环境：STM32F103C8T6 + FreeRTOS v202406.05-LTS + Keil MDK5　担任角色：独立开发
> 项目描述：采用 FreeRTOS 实时内核，将温湿度/烟雾采集、OLED 显示、云平台上报、本地报警与按键交互划分为独立任务并发运行，实现环境数据实时监测、越限告警与远程上报。
> 主要工作：基于优先级抢占调度进行多任务设计，使用消息队列在采集/显示/上云任务间传递数据，事件组触发越限报警，互斥量保护 OLED 显示资源，软件定时器负责 MQTT 心跳，按键外部中断通过任务通知（`vTaskNotifyGiveFromISR`）唤醒按键任务；编写 DHT11/ADC/OLED/ESP8266 等驱动并完成整机联调。
