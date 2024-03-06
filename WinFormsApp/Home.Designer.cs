namespace WinFormsApp
{
    partial class Home
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Home));
            txtOutput = new TextBox();
            txtScript = new TextBox();
            label1 = new Label();
            btnExecuteScript = new Button();
            btnContinue = new Button();
            btnStop = new Button();
            btnKill = new Button();
            lbPrsk = new Label();
            btnStepOver = new Button();
            btnNextCmd = new Button();
            btnDebugExit = new Button();
            lbDebug = new Label();
            j0 = new TextBox();
            j5 = new TextBox();
            j4 = new TextBox();
            j3 = new TextBox();
            j2 = new TextBox();
            j1 = new TextBox();
            t1 = new TextBox();
            t2 = new TextBox();
            t3 = new TextBox();
            t4 = new TextBox();
            t5 = new TextBox();
            t0 = new TextBox();
            label4 = new Label();
            label5 = new Label();
            btnUpdateBotState = new Button();
            label6 = new Label();
            robState = new TextBox();
            robTaskState = new TextBox();
            safetyState = new TextBox();
            label7 = new Label();
            label8 = new Label();
            label9 = new Label();
            btnNextLine = new Button();
            textBox1 = new TextBox();
            txtCmd = new TextBox();
            txtChannel = new TextBox();
            label2 = new Label();
            label3 = new Label();
            label10 = new Label();
            txtLog = new TextBox();
            SuspendLayout();
            // 
            // txtOutput
            // 
            txtOutput.Font = new Font("Microsoft YaHei UI", 12F);
            txtOutput.Location = new Point(11, 1240);
            txtOutput.Multiline = true;
            txtOutput.Name = "txtOutput";
            txtOutput.ReadOnly = true;
            txtOutput.ScrollBars = ScrollBars.Vertical;
            txtOutput.Size = new Size(472, 129);
            txtOutput.TabIndex = 1;
            // 
            // txtScript
            // 
            txtScript.Location = new Point(52, 32);
            txtScript.Multiline = true;
            txtScript.Name = "txtScript";
            txtScript.Size = new Size(433, 624);
            txtScript.TabIndex = 2;
            txtScript.Text = resources.GetString("txtScript.Text");
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(12, 9);
            label1.Name = "label1";
            label1.Size = new Size(39, 20);
            label1.TabIndex = 3;
            label1.Text = "脚本";
            // 
            // btnExecuteScript
            // 
            btnExecuteScript.BackColor = Color.LimeGreen;
            btnExecuteScript.ForeColor = SystemColors.ActiveCaptionText;
            btnExecuteScript.Location = new Point(301, 662);
            btnExecuteScript.Name = "btnExecuteScript";
            btnExecuteScript.Size = new Size(185, 55);
            btnExecuteScript.TabIndex = 5;
            btnExecuteScript.Text = "执行脚本";
            btnExecuteScript.UseVisualStyleBackColor = false;
            btnExecuteScript.Click += btnExecuteScript_Click;
            // 
            // btnContinue
            // 
            btnContinue.Location = new Point(104, 784);
            btnContinue.Name = "btnContinue";
            btnContinue.Size = new Size(128, 55);
            btnContinue.TabIndex = 7;
            btnContinue.Text = "继续";
            btnContinue.UseVisualStyleBackColor = true;
            btnContinue.Click += btnContinue_Click;
            // 
            // btnStop
            // 
            btnStop.Location = new Point(238, 784);
            btnStop.Name = "btnStop";
            btnStop.Size = new Size(128, 55);
            btnStop.TabIndex = 8;
            btnStop.Text = "停止";
            btnStop.UseVisualStyleBackColor = true;
            btnStop.Click += btnStop_Click;
            // 
            // btnKill
            // 
            btnKill.Location = new Point(372, 784);
            btnKill.Name = "btnKill";
            btnKill.Size = new Size(114, 55);
            btnKill.TabIndex = 9;
            btnKill.Text = "退出";
            btnKill.UseVisualStyleBackColor = true;
            btnKill.Click += btnKill_Click;
            // 
            // lbPrsk
            // 
            lbPrsk.AutoSize = true;
            lbPrsk.Font = new Font("Microsoft YaHei UI", 15.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            lbPrsk.ForeColor = SystemColors.AppWorkspace;
            lbPrsk.Location = new Point(17, 795);
            lbPrsk.Name = "lbPrsk";
            lbPrsk.Size = new Size(64, 28);
            lbPrsk.TabIndex = 12;
            lbPrsk.Text = "PRSK";
            // 
            // btnStepOver
            // 
            btnStepOver.BackColor = Color.FromArgb(255, 128, 0);
            btnStepOver.Location = new Point(107, 662);
            btnStepOver.Name = "btnStepOver";
            btnStepOver.Size = new Size(188, 55);
            btnStepOver.TabIndex = 13;
            btnStepOver.Tag = "";
            btnStepOver.Text = "调试脚本";
            btnStepOver.UseVisualStyleBackColor = false;
            btnStepOver.Click += btnStepOver_Click;
            // 
            // btnNextCmd
            // 
            btnNextCmd.BackColor = Color.LightSkyBlue;
            btnNextCmd.ForeColor = Color.Transparent;
            btnNextCmd.Location = new Point(238, 723);
            btnNextCmd.Name = "btnNextCmd";
            btnNextCmd.Size = new Size(128, 55);
            btnNextCmd.TabIndex = 14;
            btnNextCmd.Text = "下一断点";
            btnNextCmd.UseVisualStyleBackColor = false;
            btnNextCmd.Click += btnNextCmd_Click;
            // 
            // btnDebugExit
            // 
            btnDebugExit.Location = new Point(372, 723);
            btnDebugExit.Name = "btnDebugExit";
            btnDebugExit.Size = new Size(114, 55);
            btnDebugExit.TabIndex = 15;
            btnDebugExit.Text = "退出调试";
            btnDebugExit.UseVisualStyleBackColor = true;
            btnDebugExit.Click += btnDebugExit_Click;
            // 
            // lbDebug
            // 
            lbDebug.AutoSize = true;
            lbDebug.Font = new Font("Microsoft YaHei UI", 15.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            lbDebug.ForeColor = SystemColors.AppWorkspace;
            lbDebug.Location = new Point(17, 734);
            lbDebug.Name = "lbDebug";
            lbDebug.Size = new Size(60, 28);
            lbDebug.TabIndex = 16;
            lbDebug.Text = "调 试";
            // 
            // j0
            // 
            j0.Location = new Point(87, 1391);
            j0.Name = "j0";
            j0.Size = new Size(40, 27);
            j0.TabIndex = 61;
            j0.Text = "1";
            // 
            // j5
            // 
            j5.BackColor = Color.BlanchedAlmond;
            j5.Location = new Point(321, 1391);
            j5.Name = "j5";
            j5.Size = new Size(40, 27);
            j5.TabIndex = 66;
            j5.Text = "6";
            // 
            // j4
            // 
            j4.Location = new Point(275, 1391);
            j4.Name = "j4";
            j4.Size = new Size(40, 27);
            j4.TabIndex = 65;
            j4.Text = "5";
            // 
            // j3
            // 
            j3.BackColor = Color.BlanchedAlmond;
            j3.Location = new Point(229, 1391);
            j3.Name = "j3";
            j3.Size = new Size(40, 27);
            j3.TabIndex = 64;
            j3.Text = "4";
            // 
            // j2
            // 
            j2.Location = new Point(183, 1391);
            j2.Name = "j2";
            j2.Size = new Size(40, 27);
            j2.TabIndex = 63;
            j2.Text = "3";
            // 
            // j1
            // 
            j1.BackColor = Color.BlanchedAlmond;
            j1.Location = new Point(133, 1391);
            j1.Name = "j1";
            j1.Size = new Size(40, 27);
            j1.TabIndex = 62;
            j1.Text = "2";
            // 
            // t1
            // 
            t1.BackColor = Color.BlanchedAlmond;
            t1.Location = new Point(133, 1435);
            t1.Name = "t1";
            t1.Size = new Size(40, 27);
            t1.TabIndex = 68;
            t1.Text = "1.8";
            // 
            // t2
            // 
            t2.Location = new Point(183, 1435);
            t2.Name = "t2";
            t2.Size = new Size(40, 27);
            t2.TabIndex = 69;
            t2.Text = "1.9";
            // 
            // t3
            // 
            t3.BackColor = Color.BlanchedAlmond;
            t3.Location = new Point(229, 1435);
            t3.Name = "t3";
            t3.Size = new Size(40, 27);
            t3.TabIndex = 70;
            t3.Text = "2.0";
            // 
            // t4
            // 
            t4.Location = new Point(275, 1435);
            t4.Name = "t4";
            t4.Size = new Size(40, 27);
            t4.TabIndex = 71;
            t4.Text = "2.1";
            // 
            // t5
            // 
            t5.BackColor = Color.BlanchedAlmond;
            t5.Location = new Point(321, 1435);
            t5.Name = "t5";
            t5.Size = new Size(40, 27);
            t5.TabIndex = 72;
            t5.Text = "2.2";
            // 
            // t0
            // 
            t0.Location = new Point(87, 1435);
            t0.Name = "t0";
            t0.Size = new Size(40, 27);
            t0.TabIndex = 67;
            t0.Text = "1.7";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Font = new Font("Microsoft YaHei UI", 15.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            label4.ForeColor = SystemColors.AppWorkspace;
            label4.Location = new Point(11, 1388);
            label4.Name = "label4";
            label4.Size = new Size(54, 28);
            label4.TabIndex = 30;
            label4.Text = "关节";
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Font = new Font("Microsoft YaHei UI", 15.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            label5.ForeColor = SystemColors.AppWorkspace;
            label5.Location = new Point(11, 1432);
            label5.Name = "label5";
            label5.Size = new Size(54, 28);
            label5.TabIndex = 31;
            label5.Text = "末端";
            // 
            // btnUpdateBotState
            // 
            btnUpdateBotState.Location = new Point(392, 1388);
            btnUpdateBotState.Name = "btnUpdateBotState";
            btnUpdateBotState.Size = new Size(92, 116);
            btnUpdateBotState.TabIndex = 76;
            btnUpdateBotState.Text = "更新机\r\n器状态";
            btnUpdateBotState.UseVisualStyleBackColor = true;
            btnUpdateBotState.Click += btnUpdateBotState_Click;
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Font = new Font("Microsoft YaHei UI", 15.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            label6.ForeColor = SystemColors.AppWorkspace;
            label6.Location = new Point(11, 1476);
            label6.Name = "label6";
            label6.Size = new Size(54, 28);
            label6.TabIndex = 33;
            label6.Text = "状态";
            // 
            // robState
            // 
            robState.Location = new Point(133, 1477);
            robState.Name = "robState";
            robState.Size = new Size(40, 27);
            robState.TabIndex = 73;
            robState.Text = "0";
            // 
            // robTaskState
            // 
            robTaskState.Location = new Point(229, 1477);
            robTaskState.Name = "robTaskState";
            robTaskState.Size = new Size(40, 27);
            robTaskState.TabIndex = 74;
            robTaskState.Text = "0";
            // 
            // safetyState
            // 
            safetyState.Location = new Point(321, 1477);
            safetyState.Name = "safetyState";
            safetyState.Size = new Size(40, 27);
            safetyState.TabIndex = 75;
            safetyState.Text = "0";
            // 
            // label7
            // 
            label7.AutoSize = true;
            label7.Font = new Font("Microsoft YaHei UI", 15.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            label7.ForeColor = SystemColors.AppWorkspace;
            label7.Location = new Point(183, 1476);
            label7.Name = "label7";
            label7.Size = new Size(38, 28);
            label7.TabIndex = 37;
            label7.Text = "rts";
            // 
            // label8
            // 
            label8.AutoSize = true;
            label8.Font = new Font("Microsoft YaHei UI", 15.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            label8.ForeColor = SystemColors.AppWorkspace;
            label8.Location = new Point(275, 1476);
            label8.Name = "label8";
            label8.Size = new Size(32, 28);
            label8.TabIndex = 38;
            label8.Text = "ss";
            // 
            // label9
            // 
            label9.AutoSize = true;
            label9.Font = new Font("Microsoft YaHei UI", 15.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            label9.ForeColor = SystemColors.AppWorkspace;
            label9.Location = new Point(82, 1476);
            label9.Name = "label9";
            label9.Size = new Size(43, 28);
            label9.TabIndex = 39;
            label9.Text = "rbs";
            // 
            // btnNextLine
            // 
            btnNextLine.BackColor = Color.LightSkyBlue;
            btnNextLine.ForeColor = Color.Transparent;
            btnNextLine.Location = new Point(104, 723);
            btnNextLine.Name = "btnNextLine";
            btnNextLine.Size = new Size(128, 55);
            btnNextLine.TabIndex = 40;
            btnNextLine.Text = "下一步";
            btnNextLine.UseVisualStyleBackColor = false;
            btnNextLine.Click += btnNextLine_Click;
            // 
            // textBox1
            // 
            textBox1.Enabled = false;
            textBox1.Location = new Point(13, 32);
            textBox1.Multiline = true;
            textBox1.Name = "textBox1";
            textBox1.Size = new Size(33, 624);
            textBox1.TabIndex = 41;
            textBox1.Text = " 1\r\n 2\r\n 3\r\n 4\r\n 5\r\n 6\r\n 7\r\n 8\r\n 9\r\n10\r\n11\r\n12\r\n13\r\n14\r\n15\r\n16\r\n17\r\n18\r\n19\r\n20\r\n21\r\n22\r\n23\r\n24\r\n25\r\n26\r\n27\r\n28\r\n29\r\n30\r\n31";
            // 
            // txtCmd
            // 
            txtCmd.Font = new Font("Microsoft YaHei UI", 12F);
            txtCmd.ForeColor = Color.DimGray;
            txtCmd.Location = new Point(12, 884);
            txtCmd.Multiline = true;
            txtCmd.Name = "txtCmd";
            txtCmd.ReadOnly = true;
            txtCmd.ScrollBars = ScrollBars.Vertical;
            txtCmd.Size = new Size(231, 309);
            txtCmd.TabIndex = 42;
            // 
            // txtChannel
            // 
            txtChannel.Font = new Font("Microsoft YaHei UI", 12F);
            txtChannel.ForeColor = Color.DimGray;
            txtChannel.Location = new Point(251, 884);
            txtChannel.Multiline = true;
            txtChannel.Name = "txtChannel";
            txtChannel.ReadOnly = true;
            txtChannel.ScrollBars = ScrollBars.Vertical;
            txtChannel.Size = new Size(233, 309);
            txtChannel.TabIndex = 44;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Font = new Font("Microsoft YaHei UI", 15.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            label2.ForeColor = SystemColors.AppWorkspace;
            label2.Location = new Point(14, 853);
            label2.Name = "label2";
            label2.Size = new Size(54, 28);
            label2.TabIndex = 45;
            label2.Text = "指令";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Font = new Font("Microsoft YaHei UI", 15.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            label3.ForeColor = SystemColors.AppWorkspace;
            label3.Location = new Point(251, 853);
            label3.Name = "label3";
            label3.Size = new Size(54, 28);
            label3.TabIndex = 46;
            label3.Text = "通道";
            // 
            // label10
            // 
            label10.AutoSize = true;
            label10.Font = new Font("Microsoft YaHei UI", 15.75F, FontStyle.Regular, GraphicsUnit.Point, 0);
            label10.ForeColor = SystemColors.AppWorkspace;
            label10.Location = new Point(11, 1209);
            label10.Name = "label10";
            label10.Size = new Size(54, 28);
            label10.TabIndex = 47;
            label10.Text = "输出";
            // 
            // txtLog
            // 
            txtLog.Location = new Point(491, 32);
            txtLog.Multiline = true;
            txtLog.Name = "txtLog";
            txtLog.ReadOnly = true;
            txtLog.Size = new Size(127, 1472);
            txtLog.TabIndex = 77;
            // 
            // Home
            // 
            AutoScaleDimensions = new SizeF(9F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(630, 1516);
            Controls.Add(txtLog);
            Controls.Add(label10);
            Controls.Add(label3);
            Controls.Add(label2);
            Controls.Add(txtChannel);
            Controls.Add(txtCmd);
            Controls.Add(txtOutput);
            Controls.Add(textBox1);
            Controls.Add(btnNextLine);
            Controls.Add(label9);
            Controls.Add(label8);
            Controls.Add(label7);
            Controls.Add(safetyState);
            Controls.Add(robTaskState);
            Controls.Add(robState);
            Controls.Add(label6);
            Controls.Add(btnUpdateBotState);
            Controls.Add(label5);
            Controls.Add(label4);
            Controls.Add(t1);
            Controls.Add(t2);
            Controls.Add(t3);
            Controls.Add(t4);
            Controls.Add(t5);
            Controls.Add(t0);
            Controls.Add(j1);
            Controls.Add(j2);
            Controls.Add(j3);
            Controls.Add(j4);
            Controls.Add(j5);
            Controls.Add(j0);
            Controls.Add(lbDebug);
            Controls.Add(btnDebugExit);
            Controls.Add(btnNextCmd);
            Controls.Add(btnStepOver);
            Controls.Add(lbPrsk);
            Controls.Add(btnKill);
            Controls.Add(btnStop);
            Controls.Add(btnContinue);
            Controls.Add(btnExecuteScript);
            Controls.Add(label1);
            Controls.Add(txtScript);
            Name = "Home";
            Text = "Home";
            Load += Home_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
        private TextBox txtOutput;
        private TextBox txtScript;
        private Label label1;
        private Button btnExecuteScript;
        private Button btnContinue;
        private Button btnStop;
        private Button btnKill;
        private Label lbPrsk;
        private Button btnStepOver;
        private Button btnNextCmd;
        private Button btnDebugExit;
        private Label lbDebug;
        private TextBox j0;
        private TextBox j5;
        private TextBox j4;
        private TextBox j3;
        private TextBox j2;
        private TextBox j1;
        private TextBox t1;
        private TextBox t2;
        private TextBox t3;
        private TextBox t4;
        private TextBox t5;
        private TextBox t0;
        private Label label4;
        private Label label5;
        private Button btnUpdateBotState;
        private Label label6;
        private TextBox robState;
        private TextBox robTaskState;
        private TextBox safetyState;
        private Label label7;
        private Label label8;
        private Label label9;
        private Button btnNextLine;
        private TextBox textBox1;
        private TextBox txtCmd;
        private TextBox txtChannel;
        private Label label2;
        private Label label3;
        private Label label10;
        private TextBox txtLog;
    }
}
