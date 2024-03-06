namespace WinFormsApp
{
    public partial class Home : Form
    {
        private readonly Cmd cmd;
        private static int autoCmdId = -1;

        public Home()
        {
            cmd = new Cmd();
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false; // 取消线程间的安全检查
        }

        private void Home_Load(object sender, EventArgs e)
        {
            this.Location = new Point(0, 0);

            Thread listen = new(new ThreadStart(getCmd))
            {
                IsBackground = true
            };
            listen.Start();
        }

        #region 脚本

        /// <summary>
        /// 执行脚本
        /// </summary>
        private void btnExecuteScript_Click(object sender, EventArgs e)
        {
            txtLog.Text = "";
            txtOutput.Text = Sm.ExecuteScript(txtScript.Text);
        }

        private void btnCmdCompleted_Click(object sender, EventArgs e)
        {
            if (cmd == null)
            {
                txtOutput.Text = "没有指令";
                return;
            }
            Sm.SetCmdFinished();
        }

        private void getCmd()
        {
            while (true)
            {
                try
                {
                    Thread.Sleep(50);
                    Cmd c = Sm.GetCmd();

                    cmd.id = c.id; // 指令 ID
                    cmd.channel = c.channel;// 通道ID
                    cmd.func = c.func;  // EnumExtFunc
                    cmd.v = c.v;  // 速度
                    cmd.a = c.a;  // 加速度
                    cmd.tool = c.tool;  // 工具
                    cmd.wobj = c.wobj; // 工件坐标系
                    cmd.status = c.status;// 指令状态
                    cmd.values = c.values;
                    cmd.toolId = c.toolId;
                    cmd.toolAction = c.toolAction;

                    txtCmd.Text = $"id：{cmd.id}\r\nfunc：{(EnumExtFunc)cmd.func}\r\nv：{cmd.v}\r\na：{cmd.a}\r\ntool：{cmd.tool}\r\nwobj：{cmd.wobj}\r\nstatus：{(EnumExecuteStatus)cmd.status}\r\nvalues：{cmd.values[0]},{cmd.values[1]},{cmd.values[2]},{cmd.values[3]},{cmd.values[4]},{cmd.values[5]}\r\n工具ID:{cmd.toolId}\r\n工具动作:{cmd.toolAction}";

                    ChannelStateModel ch = Sm.GetChannelState();
                    txtChannel.Text = $"status：{ch.status}\r\nfunc：{ch.funcName}\r\nline：{ch.line}\r\njointV：{ch.jointV}\r\ntcpV：{ch.tcpV}\r\njointA：{ch.jointA}\r\ntcpA：{ch.tcpA}\r\ntool：{ch.tool}\r\nwobj：{ch.wobj}\r\nhasErr:{ch.hasErr}\r\nerr:{ch.err}\r\nlog:{ch.log}";

                    // 自动完成
                    if (autoCmdId != cmd.id && cmd.id>0)
                    {
                        Thread.Sleep(1000);
                        Sm.SetCmdFinished();
                        autoCmdId = cmd.id;

                        string now = DateTime.Now.ToString("mm:ss");
                        txtLog.Text = txtLog.Text +$"\r\n-----{now}-----\r\nid:{cmd.id}\r\nfunc:{cmd.func}";
                    }
                }
                catch (Exception ex)
                {
                    txtOutput.Text = ex.Message;
                }
            }
        }

        #endregion 脚本

        #region prsk

        /// <summary>
        /// 继续
        /// </summary>
        private void btnContinue_Click(object sender, EventArgs e)
        {
            var resp = Sm.Prsk(EnumPrst.normal);
            txtOutput.Text = $"Prsk.result {resp}";
        }

        /// <summary>
        /// 停止当前指令
        /// </summary>
        private void btnStop_Click(object sender, EventArgs e)
        {
            var resp = Sm.Prsk(EnumPrst.stop);
            txtOutput.Text = $"Prsk.result {resp}";
        }

        /// <summary>
        /// 退出脚本
        /// </summary>
        private void btnKill_Click(object sender, EventArgs e)
        {
            var resp = Sm.Prsk(EnumPrst.terminate);
            txtOutput.Text = $"Prsk.result {resp}";
        }

        #endregion prsk

        #region 调试

        private void btnStepOver_Click(object sender, EventArgs e)
        {
            txtLog.Text = "";
            txtOutput.Text = Sm.ExecuteScript(txtScript.Text);

            var resp = Sm.SetDebugMode(true);
            txtOutput.Text = "启用断点:" + resp.ToString() + "\r\n";

            // 设置断点
            int[] arr = [19];
            resp = Sm.SetBreakpoints(arr);
        }

        private void btnNextCmd_Click(object sender, EventArgs e)
        {
            Sm.NextBreakpoint();
        }

        private void btnDebugExit_Click(object sender, EventArgs e)
        {
            Sm.SetDebugMode(false);
        }

        #endregion 调试

        private void btnUpdateBotState_Click(object sender, EventArgs e)
        {
            int[] state = [int.Parse(robState.Text), int.Parse(robTaskState.Text), int.Parse(safetyState.Text)];
            double[] tcp = [double.Parse(t0.Text), double.Parse(t1.Text), double.Parse(t2.Text), double.Parse(t3.Text), double.Parse(t4.Text), double.Parse(t5.Text)];// 当前工具末端位置
            double[] joints = [double.Parse(j0.Text), double.Parse(j1.Text), double.Parse(j2.Text), double.Parse(j3.Text), double.Parse(j4.Text), double.Parse(j5.Text)];// 当前关键角度

            txtOutput.Text = Sm.UpdateBotStatus(state, joints, tcp).ToString();
        }

        private void btnNextLine_Click(object sender, EventArgs e)
        {
            Sm.NextStep();
        }

        /// <summary>
        /// 这里根据判断调用新松sdk控制机器, 写个示例
        /// </summary>
        /// <param name="cmd"></param>
        /// <returns></returns>
        private string CmdName(Cmd cmd)
        {
            return cmd.func switch
            {
                EnumExtFunc.none => "none",
                EnumExtFunc.movej => "movej",
                EnumExtFunc.movep => "movep",
                EnumExtFunc.movej_d => "movej_d",
                EnumExtFunc.movep_d => "movep_d",
                EnumExtFunc.movel => "movel",
                EnumExtFunc.movel_d => "movel_d",
                EnumExtFunc.update_wobj => "update_wobj",
                EnumExtFunc.use_wobj => "use_wobj",
                EnumExtFunc.use_tool => "use_tool",
                EnumExtFunc.tool => "tool_ctrl",
                _ => "未知",
            };
        }
    }
}