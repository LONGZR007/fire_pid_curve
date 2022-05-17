# 野火PID调试助手通信协议包

## 1、介绍

该软件包是与野火多功能调试助手之PID调试助手通信的协议包。

![野火上位机](img/%E9%87%8E%E7%81%AB%E4%B8%8A%E4%BD%8D%E6%9C%BA.png)

### 1.1 目录结构

| 名称 | 说明 |
| ---- | ---- |
| examples | 例子目录 |
| inc  | 头文件目录 |
| src  | 源代码目录 |

### 1.2 许可证

遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3 依赖

- 依赖 RT-Thread 的串口设备。

## 2、如何打开 hello

使用 package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages
    tools packages --->
        [*]  fire PID communication protocol debugging assistant.
```

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、API 使用

在打开 package 后，当进行 bsp 编译时，它会被加入到 bsp 工程中进行编译。

1. 初始化协议：
    ``` c
    /**
     * @brief   初始化接收协议
     * @param   name:串口设备名
     * @return  初始化结果.
     */
    int32_t protocol_init(const char *name);
    ```
    初始化调用，传入串口设备名，成功返回0，失败返回-1；

1. 协议接收数据
    ``` c
    /**
     * @brief   接收数据处理
     * @param   *data:  要计算的数据的数组.
     * @param   data_len: 数据的大小
     * @return  void.
     */
    void protocol_data_recv(uint8_t *data, uint16_t data_len);
    ```
    在任务中轮询调用，如```protocol_data_recv(0, 0);```；

1. 发送数据给上位机
    ``` c
    /**
     * @brief 设置上位机的值
     * @param cmd：命令
     * @param ch: 曲线通道
     * @param data：参数指针
     * @param num：参数个数
     * @retval 无
     */
    void set_computer_value(uint8_t cmd, uint8_t ch, void *data, uint8_t num);
    ```
    发送数据给上位机，发送方式请参考[example/example.c](example/example.c)；

1. 接收帧处理
    ``` c
    /**
     * @brief   查询帧类型（命令）
     * @param   *data:  帧数据
     * @param   data_len: 帧数据的大小
     * @return  帧类型（命令）.
     */
    uint8_t protocol_frame_parse(uint8_t *data, uint16_t *data_len);
    ```
    根据返回的命令类型处理不同的命令，请参考[example/example.c](example/example.c)；

## 4、注意事项

请参考该上位机的通信协议

## 5、联系方式 & 感谢

* 维护：LONGZR007
* 主页：https://gitee.com/LONGZR007
