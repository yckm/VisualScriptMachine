**版本: v0.6**
## 一、概述
### 1.1 系统概述
为方便作业人员简单的编写脚本就能实现对机械臂的控制、工具的控制，特设计 ` 脚本机 `。本系统允许通过编写脚本实现:

   - 单臂控制
   - 双臂并行控制
   - 工具控制
### 1.2 术语

- ` 基座坐标系 `:机械臂的基座坐标系是以机器人安装基座为基准，用来描述机器人本体运动的直角坐标系。基座坐标系一般位于机器人安装底座的几何中心位置。它是机器人所有运动的基础参考坐标系。出厂前设置好后续不再改变。
- ` 工件坐标系 `:工件坐标系是固定在工件上的笛卡尔坐标系，用于描述工件相对于 ` 基座坐标系 `的位置。机器人可以拥有若干工件坐标系，或者表示不同工件，或者表示同一工件在不同位置的若干副本。本文后续称呼 ` 当前坐标系 `指的是 ` 当前工件坐标系 `。
- ` 工具作用点 `: 下文简称 `PT`(  Point of application of force on the Tool ), 实际和作业对象发生接触的中心位置。 如果连接有末端执行器，则一般在执行器中心轴末端（或夹爪中点）。
- ` 工具偏移量 `: 工具偏移量是 PT 相对于第6轴法兰末端中心的偏移 `{ x, y, z, a, b, c }`。
- ` 同步信号量 `: 一种用于多线程或多进程间同步的机制。它用于防止被抢占。低优先级的任务持有信号量，高优先级的任务需要这个信号量，只有当低优先级的任务释放信号量，高优先级的任务才能获取信号量。同步信号量主要用于控制线程的执行顺序。
- `可撤销 `。当机械臂运动达到特定条件,机械臂运动停止。
## 二、系统架构
### 2.1 系统架构
![](https://cdn.nlark.com/yuque/0/2024/jpeg/34658927/1706068518442-24f2db7e-d11a-4912-b5ae-9a489b1d975b.jpeg)
脚本机 由四个模块构成:

- ` 脚本解析模块 `  :  解析用户脚本,构建指令树。
- ` 执行模块 `  : 执行指令树指令,实现对 NC 和工具的控制。
- ` 调度模块 ` :  进行全局调度,包括
   - 信号管理
   - 全局变量管理
   - 坐标系管理
   - 任务调度

脚本机 通过 ` 通道 `实现对机械臂的控制。
### 2.2 通道

- ` 通道 `: 外部通过"通道"发送控制指令，实现对机械臂的控制。
   - 一个通道控制且仅控制一个 NC。
   - 通道和 NC 绑定后无法更改。
   - 通道由通道 ID 定义，ID 为无符号整数，从 0 开始。通道 ID 和 NC进行绑定。
   - 同一通道内，指令顺序执行；
   - 通道间相互独立，互不干扰，但可通过 ` 同步信号量 ` 进行协调。

![](https://cdn.nlark.com/yuque/0/2024/jpeg/34658927/1705374340239-835d6e9a-4ae1-4f1a-b164-bb5b0daad6ab.jpeg)
通道
### 2.3 工具
 	工具的定义包括以下几个方面：

   - 工具类型：
| **代码** | **描述** |
| --- | --- |
| 0 | 0 号工具 |
| 1 | 剥线器 |
| 2 | 挂线器 |
| 3 | 断线器 |
| 4 | 电动扳手 |
| 5 | 夹爪 |

   - 工具编号：每个工具都有一个唯一的编号，用于在控制系统中识别和区分,工具编号由系统预先进行定义。
      - 0 : 0 号工具, 偏移量为 0 ,  参数不运行进行修改。
      - 10~19 : 剥线器
      - 20~29 : 挂线器
      - 30~39 : 断线器
      - 40~49 : 电动扳手
      - 50~59 : 夹爪
   - 工具参数：工具参数包括工具的进深、长度、工具偏移量 等。

![](https://cdn.nlark.com/yuque/0/2024/jpeg/34658927/1706495401563-f886483e-fbba-4125-92d0-dddb41b73a1c.jpeg)

   - 动作
| **代码** | **描述** | 动作 |
| --- | --- | --- |
| 0 | 0 号工具 |  |
| 1 | 剥线器 | 0 复位
1 夹紧
2  剥线
3  松开 |
| 2 | 挂线器 |  |
| 3 | 断线器 | 1 夹紧
2  松开 |
| 4 | 电动扳手 | 1 打紧
2 松开 |
| 5 | 夹爪 | 1 夹紧
2  松开 |

## 三、类型和变量
### 3.1 枚举类型
#### 3.1.1 可撤销
| 描述 | 代码 |
| --- | --- |
| 不执行撤销 | 0 |
| 用户执行撤销 | 1 |
| 碰撞撤销 | 2 |

#### 3.1.2  机器人状态信息
| **描述** | **代码** |
| --- | --- |
| 机器人启动 | 0 |
| 机器人初始化 | 1 |
| 机器人登出，暂未使用 | 2 |
| 机器人登陆，暂未使用 | 3 |
| 机器人下电 | 4 |
| 机器人下使能 | 5 |
| 机器人上使能 | 6 |
| 机器人更新 | 7 |

#### 3.1.3 任务状态信息
| **描述** | **代码** |
| --- | --- |
| 任务未执行 | 0 |
| 任务正在执行 | 1 |
| 任务已经暂停 | 2 |
| 任务已经停止 | 3 |
| 任务正常执行结束 | 4 |
| 任务被中断（任务已经结束） | 5 |
| 任务出错（任务已经结束） | 6 |
| 任务非法，当前状态下任务不能执行 | 7 |
| 任务参数错误（任务已经结束） | 8 |

#### 3.1.4 安全控制器状态信息
| **描述** | **代码** |
| --- | --- |
| 初始化 | 0 |
| 等待 | 2 |
| 配置模式 | 3 |
| 下电状态 | 4 |
| 正常运行状态 | 5 |
| 恢复模式 | 6 |
| Stop2 | 7 |
| Stop1 | 8 |
| Stop0 | 9 |
| 模型配置状态 | 10 |
| 缩减模式状态 | 12 |
| 引导 | 13 |
| 致命错误状态 | 14 |
| 更新状态 | 99 |

### 3.2 同步信号量
#### 3.2.1 通道信号量
` 通道信号量 ` : 用于通道间通信和协调的信号量。通道信号强调的是通道间的同步：一个 A 通道完成了某一个动作就通过信号量通知 B 通道，B 再进行某些动作。通道信号量由系统预定义,固定名称,,系统定义了 16 组 int 类型的信号量 `c0、c1... c15`, 默认值为 `0`。
#### 3.2.2 工具信号量
` 工具信号量 `: 用于通道和工具协调条的信号量。工具完成动作后,告知通道。通道信号量由 ` 系统预定义 `,固定名称,系统定义了 16 组 int 类型的信号量 `t0、t1... t15`, 默认值为 `0`。
### 3.3 程序异常代码
| **描述** | **代码** |
| --- | --- |
| 正常 | 0 |
| 参数错误 | 1 |
| 类型错误 | 2 |
| 略 | ... |

## 四、语言要素
### 4.1 数据类型
| 数据类型 | 描述 | 示例 |
| --- | --- | --- |
| 布尔 | 包含两个值：false和true。
- true 在系统中代指真。
- false 在系统中代指假。
 | true、false |
| 数字 | 表示双精度类型的实浮点数 | 1、1.2、-3.1 |
| 字符串 | 字符串由一对双引号或单引号来表示 | "default"、"tool coord" |
| 列表 | 可容纳其他数据类型的表 | {1,2,3}  或 {"ab",1,2} |
| 函数 | 可执行的一段代码 
function 开头、end 结尾 | **function** add(x,y) -- 定义两数相加
    return x+y -- 返回和
**end** |
| nil | 表示一种没有任何有效值，它只有一个值 -- nil 。
对于全局变量，nil 还有一个"删除"作用，给全局变量赋一个 nil 值，等同于把它们删掉。 |  |

### 4.2 变量

- 变量有三种类型：全局变量、局部变量、表中的域。
- 变量在使用前，需要在代码中进行声明，即创建该变量。
- 变量全是全局变量，哪怕是语句块或是函数里，除非用 local 显式声明为局部变量。
- 局部变量的作用域为从声明位置开始到所在语句块结束。
- 变量的默认值均为 nil。
```cpp
a = 5               -- 全局变量

function joke  
    c = 5           -- 全局变量
    local d = 6     -- 局部变量
end
```
### 4.3  循环
| 循环类型 | 描述 | 语法 |
| --- | --- | --- |
| while 循环条件 | 在条件为 true 时，让程序重复地执行某些语句。执行语句前会先检查条件是否为 true。 | **while(condition)**
**do**
   -- 代码
**end** |

### 4.4 流程控制
| 类型 | 描述 | 语法 |
| --- | --- | --- |
| if | **if 语句** 由一个布尔表达式作为条件判断，其后紧跟其他语句组成。 | if(布尔表达式)
then
   -- 在布尔表达式为 true 时执行的语句
end |
| if .. 
elseif ...
else | if 语句 可以与 elseif、 else 语句搭配使用, 在 if 条件表达式为 false 时执行 elseif，elseif 条件表达式为 false 时 else 语句代码。 | if( 布尔表达式 1)
then
   -- 在布尔表达式 1 为 true 时执行该语句块 
elseif( 布尔表达式 2)
then
   -- 在布尔表达式 2 为 true 时执行该语句块 
else 
   -- 如果以上布尔表达式都不为 true 则执行该语句块 
end |

### 4.5 运算符
#### 4.5.1 算数运算
| 操作符 | 描述 | 实例 |
| --- | --- | --- |
| + | 加法 | A + B 输出结果 30 |
| - | 减法 | A - B 输出结果 -10 |
| * | 乘法 | A * B 输出结果 200 |
| / | 除法 | B / A 输出结果 2 |

#### 4.5.2  关系运算
|  |  |  |
| --- | --- | --- |
| == | 等于，检测两个值是否相等，相等返回 true，否则返回 false | (A == B) 为 false。 |
| ~= | 不等于，检测两个值是否相等，不相等返回 true，否则返回 false | (A ~= B) 为 true。 |
| > | 大于，如果左边的值大于右边的值，返回 true，否则返回 false | (A > B) 为 false。 |
| < | 小于，如果左边的值大于右边的值，返回 false，否则返回 true | (A < B) 为 true。 |
| >= | 大于等于，如果左边的值大于等于右边的值，返回 true，否则返回 false | (A >= B) 返回 false。 |
| <= | 小于等于， 如果左边的值小于等于右边的值，返回 true，否则返回 false | (A <= B) 返回 true。 |

#### 4.5.3 逻辑运算
| 操作符 | 描述 | 实例 |
| --- | --- | --- |
| and | 逻辑与操作符。 若 A 为 false，则返回 A，否则返回 B。 | (A and B) 为 false。 |
| or | 逻辑或操作符。 若 A 为 true，则返回 A，否则返回 B。 | (A or B) 为 true。 |
| not | 逻辑非操作符。与逻辑运算结果相反，如果条件为 true，逻辑非为 false。 | not(A and B) 为 true。 |


## 五、脚本定义
### 5.1 概述
本章对脚本结构,脚本函数进行定义。
系统提供函数返回值由两部分构成,` 实际返回值 `和 ` 异常代码 `构成.格式如下
```lua
{
  err: 异常代码,i数字类型, 参考 3.3,
  data: 实际返回值,不同函数该值类型不尽相同
}
```
第五章中系统函数返回值描述时, 只描述 data 的值,不再对 err 进行赘述。
### 5.2 脚本示例
```lua
use_wobj("default")  --设置使用默认工件坐标系作为当前坐标系
use_tool("boxianqi")  --设置使用剥线器工具

set_joints_speed(45)	  --关节全局角速度为45%
set_tail_speed(10)  --工具作用点速度设置为10mm/s

movej ({0,-51,84,39,78,0 })  --按关节运动到剥线器
movej_d ({0,0,0,0,0,30 })  --6轴旋转30°取工具

movej ({0,51,84,39,78,0 })  --移动到预备位置

prepared_pose,err=get_tcp()   --记录当前末端位姿,回退时候需要用到

target_pose,err=get_target_pose()  --获取作业点的位置姿态

update_wobj("temp",taget_pose)  --建立临时工具坐标系
use_wobj("temp")  --设置临时工具坐标系为当前坐标系

target_pose[2]=target_pose[2]-20  --z方向上离作业顶点10mm为下一步移动到作业点附近

movel_c(target_pose)  --移动到作业点附近

wait(0)  --等待手动介入

tool (1,1)   --工器具1执行1号动作
signal,err=wait_signal("c1",10)  --等待来自C1的信号,10s后超时

if signal==1 then  --假设-1表示成功
	movel_d ({0,30,0,0,0,0 })  --工具从线缆退出
else
	wait(0)  --执行失败等待手动控制
end

use_wobj("default")   --使用"default"为当前坐标系
movel (prepared_pose)   --运动工会准备位姿

movej ({0,-51,84,34,78,0 })  --将工具移动到工具挂载位置附近
movej_d ({0,0,0,0,0,-30 })  --6轴旋转-30°取工具,和工具脱离

movej ({0,51,84,39,78,0 })  --移动到预备位置
```
### 5.3 辅助函数
#### 5.3.1 **send_signal**
| 函数签名 | **send_signal ( signal_name ：字符串常量，value：数字变量  )** |
| --- | --- |
| 作用 | 将信号量的值设置为指定数字 。
信号量限定为 3.2 所列。 |
| 参数说明 | `signal_name`：信号量的名称
`value`：信号量的值 |
| 返回值 |  信号是否设置成功 |
| 示例 | send_signal("c1"，0 ) -- 将 c1 的值设置为 0  |

#### 5.3.2 **wait_signal**
| 函数签名 | **wait_signal ( signal_name ：字符串常量，timeout：数字变量 )** |
| --- | --- |
| 作用 | 等待信号量 。
收到信号或者等待超时函数返回。该函数用于接收来自其他通道和工具的信号。
信号量限定为 3.2 所列。 |
| 参数说明 | `signal_name`：信号量的名称
`timeout`：超时时间，单位秒。 当超过 timeout 秒没等到信号的话，就不再等待了。`timeout=0`表示直到等到信号才返回。 |
| 返回值 | ` 信号值 `。信号值为 0 表示是超时返回的。 |
| 示例 | signal = wait_signal("c1" ,10) --  等待 c1 信号或者 10 秒后超时返回 |

#### 5.3.3 **wait**
| 函数签名 | **wait ( timeout : 数字变量 )** |
| --- | --- |
| 作用 | 等待手动介入, 完成手动操作后函数返回。
如果设置了超时时间, 即使手动介入未结束函数也返回。 |
| 参数说明 | `timeout`: 超时时间，单位秒 。`timeout<=0` 表示只有手动介入结束后函数才返回。 |
| 返回值 | 无 |
| 声明周期 | 脚本运行期间 |

#### 5.3.4 **get_joints**

| 函数签名 | **get_joints** **()** |
| --- | --- |
| 作用 | 获取关节角度列表。 |
| 参数说明 | 
 |
| 返回值 |  `[j1,j2,j3,j4,j5,j6]`， 对应 1-6 关节的目标关节角度，单位 deg。 |

#### 5.3.5 **get_tcp **

| 函数签名 | **get_tcp()** |
| --- | --- |
| 作用 | 获取当前 PT 的位置和姿态。 |
| 参数说明 | 
 || 返回值 | `{ x，y，z，a，b，c }`。其中 ，- `x、y、z` 为末端在基座坐标系下的位置，单位mm- `a、b、c` 表示末端的姿态，单位deg。 |

#### 5.3.6 **get_state**
| 函数签名 | **get_state()** |
| --- | --- |
| 作用 | 获取机器人的状态,包括 ` 机器人状态 `，` 任务状态 `、` 碰撞状态 ` |
| 参数说明 |  |
| 阻塞 | 阻塞 |
| 返回值 | `{r,t,c}`,其中`r`: 机器人的状态，`t`: 任务状态，`c`: 机器人的碰撞状态，参考 3.1 |

#### 5.3.7 **set_collision**
| 函数签名 | **set_collision ( level : 数字变量 )** |
| --- | --- |
| 作用 | 设置碰撞检测 |
| 参数说明 | `level `: 检测等级。其中,- -1 不进行检测; - 0 检测机械臂自身; - 1 检测机械臂和外部碰撞 |
| 阻塞 | 阻塞 |
| 返回值 | 无 |
| 生命周期 | 仅对当前脚本有效。 |

#### **5.3.8 get_target_pose**
| 函数签名 | **get_target_pose( ) ** |
| --- | --- |
| **作用** | 获取当前坐标系下作业目标的位置和姿态。 |
| **参数说明** |  |
| **阻塞** | **阻塞** |
| **返回值** | `** [**x, y, z, a, b, c]`,当前坐标系下作业目标的位置和姿态`x、y、z` 为末端在基座坐标系下的位置，单位mm`a、b、c` 表示末端的姿态，单位deg。 |

#### **5.3.8 rad**
| 函数签名 | **rad( v: 数值 ) ** |
| --- | --- |
| **作用** | 将角度转换为弧度(脚本中使用的为角度,但在使用 正弦等函数时候需要的参数为弧度) |
| **参数说明** | `v`: 角度 |
| **阻塞** | **阻塞** |
| **返回值** | 弧度 |

#### **5.3.8 log**
| 函数签名 | **log( content ) ** |
| --- | --- |
| **作用** | 记录日志, 并将日志传给 PLC. |
| **参数说明** | `content`日志内容.类型包括 ` 数值,布尔,字符串,表类型 ` |
| **阻塞** | **阻塞** |
| **返回值** | 无 |


### 5.4 运动设置函数
系统上电后默认参数

   - 坐标系 :  相对于基座坐标系无偏移工具坐标系;
   - PT :  0 号工具;
   - 全局关节速度倍率 : 50%;
   - 全局 PT 速度 : 500mm/s。

#### 5.4.1 update_wobj
| 函数签名 | update_wobj ( name : 字符串**常量 **,  wobj: 数组) |
| --- | --- |
| 作用 | 创建或更新工件坐标系。 |
| 参数说明 | `name`:坐标系名称。`wobj`: `[x, y, z, a, b, c]` ,工件坐标系相对于基坐标系的偏移（单位：mm，deg）。 |
| 返回值 | 无 |
| 生命周期 | 一直有效。 |

#### 5.4.2  **use_tool**
| 函数签名 | **use_tool ( tool : 字符串常量 )** |
| --- | --- |
| 作用 | 设置当前使用的 PT。 |
| 参数说明 | `tool`：设置使用的 PT。 |
| 返回值 | 无 |
| 示例 | update_tool("default") |
| 生命周期 | NC 运行期间有效。NC 重启后使用默认工具 。 |

#### 5.4.3 **use_wobj**
| 函数签名 | **use_wobj ( wobj : 字符串常量 )** |
| --- | --- |
| 作用 | 设置当前使用的坐标。 |
| 参数说明 | `wobj`： 设置使用的工件坐标系的名称。 |
| 返回值 | `true/false` ,设置是否成功 |
| 示例 | 无 |
| 生命周期 | NC 运行期间有效。NC 重启后使用默认工件坐标系。 |

#### 5.4.4 **set_joints_speed**
| 函数签名 | **set_joints_speed( v: 数字**变量, a: 数字变量 **)** |
| --- | --- |
| 作用 | 设置全局关节角速度倍率。对关节运动函数生效 。 |
| 参数说明 | `v`：关节角速度倍率，单位（系统设定速度的百分比%），取值范围（0,100]。
`a`:  关节角加速度，单位（系统设定加速度的百分比%），取值范围（0,100]。 |
| 返回值 | 无 |
| 示例 | **set_joints_speed**(50) -- 设定速度倍率 为 50% |
| 生命周期 | NC 运行期间有效。NC 重启后默认速度倍率为 50。 |

#### 5.4.5 **set_tail_speed**
| 函数签名 | **set_tail_speed( v: 数字变量, a: 数字变量 )** |
| --- | --- |
| 作用 | 设定 PT 直线运动速度。
对关直线动函数生效 。 |
| 参数说明 | `v`：PT 直线运动速度,单位 mm/s。
`a` : 末端加速度，单位（mm/s2）。 |
| 返回值 | 无 |
| 示例 | **set_tail_speed**(50) -- 末端速度为 50mm/s |
| 生命周期 | NC 运行期间有效。NC 重启后默认速度倍率为 500mm/s。 |

#### 5.4.6 exit
| 函数签名 | **exit()** |
| --- | --- |
| 作用 | 终止械臂运动并退出当前脚本。 |
| 参数说明 |  |
| 返回值 | 无 |

### 5.5 运动函数(不可撤销)
#### 5.5.1 movej
| 函数签名 | **movej ( joints ：数组 )** |
| --- | --- |
| 作用 | 控制机械臂从当前状态，按照关节运动的方式移动到 ` 目标关节角 ` 状态。 |
| 参数说明 | joints:  数组对应 1-6 关节的目标关节角度，单位 deg，范围 [-360°，360°]。 |
| 返回值 | 无 |
| 示例 | movej( { 45 ， 54 ，72 ，90 ，-10 ，-21.3 }  ) |

#### 5.5.2 movep
| 函数签名 | **movep (  pose：数组  )** |
| --- | --- |
| 作用 | 在当前坐标系下, 控制机械臂从当前状态，按照关节运动的方式移动到 PT 目标位置 。 |
| 参数说明 | pose : {x，y，z，a，b，c}。其中 ，
`x、y、z` PT 目标位置，单位mm
`a、b、c` PT 目标姿态，单位deg。 |
| 返回值 | 无 |
| 示例 | movep( { 250 ，670 ，-810 ，20 ，60 ，-18 }  ) |

#### 5.5.3 movel
| 函数签名 | **movel (  pose：数组 )** |
| --- | --- |
| 作用 | 在当前坐标系下, 将 PT按直线运动到指定位姿。 |
| 参数说明 | pose : {x，y，z，a，b，c}。其中 ，`x、y、z` 当前坐标系下 PT 的位置，单位mm`a、b、c` 当前坐标系下 PT 的姿态，单位deg。 |
| 返回值 | 无 |
| 示例 | movel( { 250 ，670 ，-810 ，20 ，60 ，-18 }  ) |

#### 5.5.4 movej_d
| 函数签名 | movej_d ( joints_offset ：数组  ) |
| --- | --- |
| 作用 | 控制机械臂从当前状态，按照关节运动的方式移动一个增量。 |
| 参数说明 | `joints_offset`:  数组对应 1-6 关节的角度增量，单位 deg，范围 [-360°，360°]。 |
| 返回值 | 无 |
| 示例 | movej_d( { 45 ， 54 ，72 ，90 ，-10 ，-21.3 }  ) |

#### 5.5.5 movep_d
| 函数签名 | **movep_d (  pose_offset：数组 )** |
| --- | --- |
| 作用 | 在当前坐标系下, 控制机械臂从当前状态，按照关节运动的方式将 PT 移动一个增量。 |
| 参数说明 | `pose_offset` : {Δx，Δy，Δz，Δa，Δb，Δc}。其中 ，`Δx、Δy、Δz` 为末端在基座坐标系下的位置，单位mm`Δa、Δb、Δc` 表示末端的姿态，单位deg。 |
| 返回值 | 无 |
| 示例 | movep_d( { 250 ，670 ，-810 ，20 ，60 ，-18 }  ) |

#### 5.5.6 movel_d
| 函数签名 | **movel_d (  pose_offset：数组 )** |
| --- | --- |
| 作用 | 在当前坐标系下, 将 PT按直线运动一个增量。 |
| 参数说明 | `pose_offset` : { Δx，Δy，Δz，Δa，Δb，Δc }。其中 ，`Δx、Δy、Δz` 为末端在基座坐标系下的位置，单位mm`Δa、Δb、Δc` 表示末端的姿态，单位deg。 || 返回值 | 无 |
| 示例 | movel_d(  250 ，670 ，-810 ，20 ，60 ，-18  ) |


## 六、控制台函数
### 6.1 概述
脚本机 提供 c++接口用于对脚本进行控制,方便和其他系统进行集成。
### 6.2 API
#### 6.2.1 **execute_script**
| 函数签名 | ** int execute_script(std::string script)** |
| --- | --- |
| 作用 | 执行脚本。脚本遵循本文档规范。 |
| 阻塞 | **非阻塞** |
| 参数说明 | script :  脚本文本内容。 |
| 返回值 | 任务 ID。 |
| 示例 | execute("channel=1\\nstepover=false\\nuse_wobj("default")\\nuse_tool("boxianqi")\\nset_joints_speed(45)\\n	set_tail_speed(10)\\nmovej({0,-51,84,39,78,0})\\nmovej_d({0,0,0,0,0,30})\\nmovej({0,51,84,39,78,0})") |


#### 6.2.2 **get_channel_state**
| 函数签名 | ** ChannelState get_channel_state(int channel)** |
| --- | --- |
| 作用 | 获取脚本运行状态 |
| 阻塞 | **阻塞** |
| 参数说明 | `channel`: 通道 ID。 |
| 返回值 | ```cpp
struct ChannelState {
	int channel; // 通道号
	int robState; // 机器人状态
	int robTaskState; // 机器人任务状态
	int safetyState; // 机器人安全控制器状态
	int line; // 正在执行指令所在行号
	int status; // 指令执行状态
}
```
 |

#### 6.2.3 **pause**
| 函数签名 | ** bool pause(int channel)** |
| --- | --- |
| 作用 | 立即暂停脚本执行。方法调用后, 立即暂停当前执行指令。 |
| 阻塞 | **非阻塞** |
| 参数说明 | `channel`: 通道 ID。 |
| 返回值 | `true/false` |

#### 6.2.4 **pause_delay**
| 函数签名 | ** bool pause_delay(int channel)** |
| --- | --- |
| 作用 | 延迟暂停脚本执行。函数调用后, 系统等待正在执行指令动作完成后暂停脚本执行。 |
| 阻塞 | **非阻塞** |
| 参数说明 | `channel`: 通道 ID。 |
| 返回值 | `true/false` |

#### 6.2.5 **resume**
| 函数签名 | ** bool resume(int channel)** |
| --- | --- |
| 作用 | 继续脚本执行并重置信号(暂停信号,停止信号)。该函数作用于以下情况- 脚本调用 wait 暂停执行- 脚本的 `stepover`被设置未 `true`导致脚本只能单步执行- 调用 `pause`导致的脚本暂停执行 |
| 阻塞 | **非阻塞** |
| 参数说明 | `channel`: 通道 ID。 |
| 返回值 | `true/false` |

#### 6.2.6 **kill**
| 函数签名 | ** bool kill (int channel)** |
| --- | --- |
| 作用 | 立即终止正在执行的任务。发送终止指令后,立即终止当前正在执行的指令动作并退出脚本执行。 |
| 阻塞 | **非阻塞** |
| 参数说明 | `channel`: 通道 ID。 |
| 返回值 | `true/false` |

#### 6.2.7 **kill_delay**
| 函数签名 | ** bool kill_delay (int channel)** |
| --- | --- |
| 作用 | 延迟终止终止正在执行的任务。发送终止指令后, 系统等待正在执行指令动作完成后退出脚本执行。 |
| 阻塞 | **非阻塞** |
| 参数说明 | `channel`: 通道 ID。 |
| 返回值 | `true/false` |

#### 6.2.8 **send_signal**
| 函数签名 | **bool send_signal ( std::string signal_name，int value )** |
| --- | --- |
| 作用 | 将信号量的值设置为指定数字 。信号量限定为 3.2 所列。 |
| 参数说明 | `signal_name`：信号量的名称`value`：信号量的值 |
| 返回值 |  信号是否设置成功 |

#### 6.2.9 update_wobj
| 函数签名 | bool update_wobj ( int channel, **std::string **name ** **, std::vector<double> wobj) |
| --- | --- |
| 作用 | 创建或更新工件坐标系。 |
| 参数说明 | `channel`: 通道 ID。`name`:坐标系名称。`wobj`: `[x, y, z, a, b, c]` ,工件坐标系相对于基坐标系的偏移（单位：mm，deg）。 |
| 返回值 | `true/false` ,设置是否成功 |
| 生命周期 | 一直有效。 |

#### 6.2.10 **set_joints_speed**
| 函数签名 | **bool set_joints_speed ( int channel, int v ,int a)** |
| --- | --- |
| 作用 | 设置全局关节角速度倍率。对关节运动函数生效 。 |
| 参数说明 | `channel`: 通道 ID。`v`：关节角速度倍率，单位（系统设定速度的百分比%），取值范围（0,100]。`a`：关节 加 角速度倍率，单位（系统设定速度的百分比%），取值范围（0,100]。 |
| 返回值 | `true/false` ,设置是否成功 |
| 生命周期 | NC 运行期间有效。NC 重启后默认速度倍率为 50。 |

#### 6.2.11 **set_tail_speed**
| 函数签名 | **bool set_tail_speed ( int channel, double v, double a )** |
| --- | --- |
| 作用 | 设定 PT 直线运动速度。对关直线动函数生效 。 |
| 参数说明 | `channel`: 通道 ID。`v`：PT 直线运动速度,单位 mm/s。`a`:  PT 直线运动加速度,单位 mm/s2。 |
| 返回值 | `true/false` ,设置是否成功 |
| 生命周期 | NC 运行期间有效。NC 重启后默认速度倍率为 500mm/s。 |

#### 6.2.12 **execute_cmd**
| 函数签名 | ** int execute_cmd ( int channel, std::string cmd )** |
| --- | --- |
| 作用 | 执行 一条指令。指令由 ` 五、脚本定义 `定义。如果对全局变量进行设置, 指令执行上下文使用系统默认。 |
| 阻塞 | **非阻塞** |
| 参数说明 | **cmd** :  一条指令内容 |
| 返回值 | 任务 ID。 |
| 示例 | execute("use_wobj("default")") |

#### 6.2.13 **stop**
| 函数签名 | ** bool stop(int channel)** |
| --- | --- |
| 作用 | 终止当前正在执行的指令并停止机械臂运动(对正在执行的单条指令有效, 不终止脚本) |
| 阻塞 | **非阻塞** |
| 参数说明 | `channel`: 通道 ID。 |
| 返回值 | `true/false` |

