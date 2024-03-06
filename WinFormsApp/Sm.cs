using System.Runtime.InteropServices;

namespace WinFormsApp
{

    #region 返回结构体

    struct Tcps
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public double[] values;
    };
    struct Joints
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public double[] values;
    };
    struct States
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public int[] values;
    };

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct Resp
    {
        public bool isOK; // 是否成功

        public IntPtr msg; // 失败消息
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct ExternCmd
    {
        public int id; // 指令 ID
        public int channel;// 通道ID
        public int func;  // EnumExtFunc
        public IntPtr values;
        public double v;  // 速度
        public double a;  // 加速度
        public IntPtr tool;  // 工具
        public IntPtr wobj; // 工件坐标系
        public int status;// 指令状态
        public int toolId; // 工具ID
        public int toolAction; // 工具动作
    };

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct ExternChannelStateModel
    {
        public int channel; // 通道号
        public IntPtr tool; // 当前工具
        public IntPtr wobj; // 当前坐标系	
        public int status; // 当前脚本的状态 0 未运行 1 运行中  2 暂停 3 完成 4 其他状态导致的结束(等价于3)
        public int jointV;// 速率为30%
        public double tcpV;// 100mm/s
        public int jointA; // 关节加速比率[1,100]
        public double tcpA;// 末端加速度 mm/s2
        public int line; // 正在执行指令所在行号
        public IntPtr funcName;// 正在执行的函数的名称
        public int hasErr; // 是否有错误
        public IntPtr err;// 错误信息
        public IntPtr log;// lua脚本中log产生的内容

    };

    /// <summary>
    /// 指令
    /// </summary>
    public class Cmd
    {
        public int id; // 指令 ID
        public int channel;// 通道ID
        public EnumExtFunc func;  // EnumExtFunc
        public double v;  // 速度 
        public double a;  // 加速度
        public string tool;  // 工具
        public string wobj; // 工件坐标系
        public EnumExecuteStatus status;// 指令状态
        public double[] values;
        public int toolId; // 工具ID
        public int toolAction; // 工具动作


        public Cmd(ExternCmd cmd)
        {
            id = cmd.id; // 指令 ID
            channel = cmd.channel;// 通道ID
            func = (EnumExtFunc)cmd.func;  // EnumExtFunc
            v = cmd.v;  // 速度 
            a = cmd.a;  // 加速度
            tool = Utils.C2Str(cmd.tool);  // 工具
            wobj = Utils.C2Str(cmd.wobj); // 工件坐标系
            status = (EnumExecuteStatus)cmd.status;// 指令状态
            values = new double[6];
            Marshal.Copy(cmd.values, values, 0, 6);
            toolId = cmd.toolId;
            toolAction = cmd.toolAction;
        }

        public Cmd()
        {
            id = -1; // 指令 ID
            channel = -1;// 通道ID
            func = EnumExtFunc.none;  // EnumExtFunc
            v = 0.0;  // 速度 
            a = 0.0;  // 加速度
            tool = string.Empty;  // 工具
            wobj = string.Empty; // 工件坐标系
            status = 0;// 指令状态            
            toolId = -1;
            toolAction = -1;
            values= new double[6];
            for (int i = 0; i < 6; i++)
            {
                values[i] = 0;
            }
        }
    }

    public class ChannelStateModel
    {
        public int channe; //  ecm.channel; // 通道号
        public string tool; //  Utils.C2Str(ecm.tool); // 当前工具
        public string wobj; //  Utils.C2Str(ecm.wobj); // 当前坐标系	
        public EnumExecuteStatus status; //  (EnumExecuteStatus)ecm.status; // 当前脚本的状态 
        public int jointV; //  ecm.jointV;// 速率为30%
        public double tcpV; //  ecm.tcpV;// 100mm/s
        public int line; //  ecm.line; // 正在执行指令所在行号
        public int jointA; //  ecm.jointA; // 关节加速度比率
        public double tcpA; //  ecm.tcpA; // 末端加速度
        public string funcName; //  Utils.C2Str(ecm.funcName); // 正在执行的函数的名称
        public int hasErr; //  ecm.hasErr; // 是否有错误
        public string err; //  Utils.C2Str(ecm.err);// 错误信息
        public string log; //  Utils.C2Str(ecm.log);// 错误信息

        public ChannelStateModel(ExternChannelStateModel ecm)
        {
            channe = ecm.channel; // 通道号
            tool = Utils.C2Str(ecm.tool); // 当前工具
            string wobj = Utils.C2Str(ecm.wobj); // 当前坐标系	
            status = (EnumExecuteStatus)ecm.status; // 当前脚本的状态 
            jointV = ecm.jointV;// 速率为30%
            tcpV = ecm.tcpV;// 100mm/s
            line = ecm.line; // 正在执行指令所在行号
            jointA = ecm.jointA; // 关节加速度比率
            tcpA = ecm.tcpA; // 末端加速度
            funcName = Utils.C2Str(ecm.funcName); // 正在执行的函数的名称
            hasErr = ecm.hasErr; // 是否有错误
            err = Utils.C2Str(ecm.err);// 错误信息
            log = Utils.C2Str(ecm.log);// 错误信息
        }
    };
    #endregion

    public class Sm
    {
        /// <summary>
        /// 将来自c++ dll的字符串转换为c#使用的字符串
        /// </summary>
        /// <param name="ptr">指向字符串的指针</param>
        /// <returns></returns>


        const string DllPath = "luaext.dll";

        #region 导入c++方法
        [DllImport(DllPath, EntryPoint = "initScriptMachine", SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        extern static bool initScriptMachine();


        [DllImport(DllPath, EntryPoint = "executeScript", CallingConvention = CallingConvention.Cdecl)]
        extern static IntPtr executeScript(int channel, string script);


        [DllImport(DllPath, EntryPoint = "prst", SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        extern static bool prst(int channel, int value);


        [DllImport(DllPath, EntryPoint = "setCmdStatus", SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        extern static bool setCmdStatus(int channel, int cmdId, int status);


        [DllImport(DllPath, EntryPoint = "getCmd", SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        extern static IntPtr getCmd(int channel);


        [DllImport(DllPath, EntryPoint = "getChannelState", SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        extern static IntPtr getChannelState(int channel);


        [DllImport(DllPath, EntryPoint = "updateBotStatus", SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        extern static bool updateBotStatus(int channel, ref States rts, ref Joints joints, ref Tcps tcp);


        [DllImport(DllPath, EntryPoint = "setDebugMode", SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        extern static bool setDebugMode(int channel, bool enable);


        [DllImport(DllPath, EntryPoint = "nextBreakpoint", SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        extern static bool nextBreakpoint(int channel);


        [DllImport(DllPath, EntryPoint = "nextStep", SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        extern static bool nextStep(int channel);


        [DllImport(DllPath, EntryPoint = "setBreakpoints", SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        extern static bool setBreakpoints(int channel, int[] linenos, int size);



        #endregion

        #region 可供c#调用的方法
        static bool smIinited = false;
        const int channelId = 0;
        static int cmdId = 0; // 指令ID

        /// <summary>
        /// 初始化脚本机
        /// </summary>
        /// <returns></returns>
        public static bool InitSm()
        {
            if (smIinited)
            {
                return true;
            }

            smIinited = true;
            bool resp = initScriptMachine();
            return resp;
        }

        /// <summary>
        /// 执行指令
        /// </summary>
        /// <param name="script">脚本内容</param>
        /// <returns></returns>
        public static string ExecuteScript(string script)
        {
            IntPtr ptr = executeScript(channelId, script);
            Resp resp = Marshal.PtrToStructure<Resp>(ptr);
            return Utils.C2Str(resp.msg);
        }

        /// <summary>
        /// prsk操作
        /// </summary>
        /// <param name="value">操作值</param>
        /// <returns></returns>
        public static bool Prsk(EnumPrst value)
        {
            return prst(channelId, (int)value);
        }

        /// <summary>
        /// 将指令设置为完成状态
        /// </summary>
        public static bool SetCmdFinished()
        {
            return setCmdStatus(channelId, cmdId, (int)EnumExecuteStatus.terminated);
        }

        /// <summary>
        /// 将指令设置为完成状态
        /// </summary>
        public static bool SetCmdRuning()
        {
            return setCmdStatus(channelId, cmdId, (int)EnumExecuteStatus.running);
        }

        /// <summary>
        /// 从脚本机获取指令
        /// </summary>
        /// <param name="channel"></param>
        /// <returns></returns>
        public static Cmd GetCmd()
        {
            IntPtr ptr = getCmd(channelId);
            ExternCmd ec = Marshal.PtrToStructure<ExternCmd>(ptr);
            Cmd cmd = new Cmd(ec);
            cmdId = cmd.id;

            Console.WriteLine($"Cmd Id {cmdId}   func:{cmd.func}");
            return cmd;
        }

        /// <summary>
        /// 获取通道信息
        /// </summary>
        /// <returns></returns>
        public static ChannelStateModel GetChannelState()
        {
            IntPtr ptr = getChannelState(channelId);
            ExternChannelStateModel info = Marshal.PtrToStructure<ExternChannelStateModel>(ptr);
            return new ChannelStateModel(info);
        }

        /// <summary>
        /// plc更新pcs中机器人状态
        /// </summary>
        /// <param name="state">机器人状态</param>
        /// <returns></returns>
        public static bool UpdateBotStatus(int[] rts, double[] joints, double[] tcp)
        {
            States s;
            s.values = new int[rts.Length];
            for (int i = 0; i < rts.Length; i++)
            {
                s.values[i] = rts[i];
            }

            Joints j;
            j.values = new double[6];
            for (int i = 0; i < joints.Length; i++)
            {
                j.values[i] = joints[i];
            }

            Tcps t;
            t.values = new double[6];
            for (int i = 0; i < tcp.Length; i++)
            {
                t.values[i] = tcp[i];
            }

            return updateBotStatus(channelId, ref s, ref j, ref t);
        }
        #endregion

        #region 调试
        /// <summary>
        /// 启用或禁用调试
        /// </summary>
        /// <param name="enable">true 启用调试/false 禁用调试</param>
        /// <returns></returns>
        public static bool SetDebugMode(bool enable)
        {
            return setDebugMode(channelId, enable);
        }

        /// <summary>
        /// 下一个可用断点
        /// </summary>
        /// <returns></returns>
        public static bool NextBreakpoint()
        {
            return nextBreakpoint(channelId);
        }

        /// <summary>
        /// 下一步
        /// </summary>
        /// <returns></returns>
        public static bool NextStep()
        {
            return nextStep(channelId);
        }


        /// <summary>
        /// 设置断点
        /// </summary>
        /// <param name="linenos">断点所在行号</param>
        /// <returns></returns>
        public static bool SetBreakpoints(int[] linenos)
        {
            return setBreakpoints(channelId, linenos, linenos.Length);
        }
        #endregion
    }
}
