#include <rtthread.h>
#include <finsh.h>
#include "curve_protocol.h"

/* 发送实际值测试 */
void test_curve(void)
{
    static int i = 0;
    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &i, 1);
    i++;
}

/* 同步上位机参数测试 */
void sync_test(void)
{
    /* 发送目标值测试 */
    int i = 200;
    set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &i, 1);

    /* 发送周期值测试 */
    i = 20;
    set_computer_value(SEND_PERIOD_CMD, CURVES_CH1, &i, 1);

    /* 发送pid值测试 */
    float pid[3] = {0.1, 0.3, 0.9};
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid, 3);

    /* 同步按钮测试 */
#if 1
    set_computer_value(SEND_START_CMD, CURVES_CH1, NULL, 0);
#else
    set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);
#endif
}

/**
 * @brief   接收的数据处理
 * @param   void
 * @return  -1：没有找到一个正确的命令.
 */
int8_t receiving_process(void)
{
  uint8_t frame_data[128];         // 要能放下最长的帧
  uint16_t frame_len = 0;          // 帧长度
  uint8_t cmd_type = CMD_NONE;     // 命令类型

  if (protocol_init("uart13"))
  {
      rt_kprintf("protocol_init fail\n");
  }

  sync_test();

  while(1)
  {
    rt_thread_mdelay(50);
    test_curve();
    protocol_data_recv(NULL, 0);
    cmd_type = protocol_frame_parse(frame_data, &frame_len);
    switch (cmd_type)
    {
      case CMD_NONE:
      {
//          rt_kprintf("CMD_NONE\n");
          break;
      }

      case SET_P_I_D_CMD:
      {
        uint32_t temp0 = COMPOUND_32BIT(&frame_data[13]);
        uint32_t temp1 = COMPOUND_32BIT(&frame_data[17]);
        uint32_t temp2 = COMPOUND_32BIT(&frame_data[21]);

        float p_temp, i_temp, d_temp;

        p_temp = *(float *)&temp0;
        i_temp = *(float *)&temp1;
        d_temp = *(float *)&temp2;

        rt_kprintf("p:%d, i:%d, d:%d\n", (uint32_t)p_temp*1000, (uint32_t)i_temp*1000, (uint32_t)d_temp*1000);    // 设置 P I D
      }
      break;

      case SET_TARGET_CMD:
      {
        int actual_temp = COMPOUND_32BIT(&frame_data[13]);    // 得到数据

        rt_kprintf("set actual:%d\n", actual_temp);    // 设置目标值
      }
      break;

      case START_CMD:
      {
          rt_kprintf("start cmd\n");              // 启动电机
      }
      break;

      case STOP_CMD:
      {
          rt_kprintf("stop cmd\n");              // 停止电机
      }
      break;

      case RESET_CMD:
      {
          rt_kprintf("reset cmd\n");          // 复位系统
      }
      break;

      case SET_PERIOD_CMD:
      {
        uint32_t temp = COMPOUND_32BIT(&frame_data[13]);     // 周期数
        rt_kprintf("set period:%d\n", temp);                             // 设置定时器周期1~1000ms
      }
      break;

      default:
          break;
    }
  }
}

