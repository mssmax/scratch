namespace mailer
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txtPassword = new System.Windows.Forms.TextBox();
            this.txtUserName = new System.Windows.Forms.TextBox();
            this.btnSend = new System.Windows.Forms.Button();
            this.txtSubject = new System.Windows.Forms.TextBox();
            this.txtBodyFile = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.btnSelectBody = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label9 = new System.Windows.Forms.Label();
            this.udBatchSize = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.udBetweenBatches = new System.Windows.Forms.NumericUpDown();
            this.udBetweenEmails = new System.Windows.Forms.NumericUpDown();
            this.btnSelectRcpt = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.txtRcptFile = new System.Windows.Forms.TextBox();
            this.txtFrom = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.lblProgress = new System.Windows.Forms.Label();
            this.lstAtts = new System.Windows.Forms.ListBox();
            this.label10 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udBatchSize)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udBetweenBatches)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udBetweenEmails)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.txtPassword);
            this.groupBox1.Controls.Add(this.txtUserName);
            this.groupBox1.Location = new System.Drawing.Point(25, 25);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(197, 115);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Credentials";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(14, 67);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Password:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.Control;
            this.label1.ForeColor = System.Drawing.Color.Black;
            this.label1.Location = new System.Drawing.Point(11, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(32, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "User:";
            // 
            // txtPassword
            // 
            this.txtPassword.BackColor = System.Drawing.SystemColors.Control;
            this.txtPassword.Location = new System.Drawing.Point(14, 82);
            this.txtPassword.Name = "txtPassword";
            this.txtPassword.Size = new System.Drawing.Size(172, 20);
            this.txtPassword.TabIndex = 1;
            this.txtPassword.UseSystemPasswordChar = true;
            this.txtPassword.TextChanged += new System.EventHandler(this.RecolorTextBox);
            // 
            // txtUserName
            // 
            this.txtUserName.BackColor = System.Drawing.SystemColors.Control;
            this.txtUserName.Location = new System.Drawing.Point(14, 38);
            this.txtUserName.Name = "txtUserName";
            this.txtUserName.Size = new System.Drawing.Size(172, 20);
            this.txtUserName.TabIndex = 0;
            this.txtUserName.Text = "mssmax@gmail.com";
            this.txtUserName.TextChanged += new System.EventHandler(this.RecolorTextBox);
            // 
            // btnSend
            // 
            this.btnSend.Enabled = false;
            this.btnSend.Location = new System.Drawing.Point(25, 376);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(133, 31);
            this.btnSend.TabIndex = 8;
            this.btnSend.Text = "&Start";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.Send_Click);
            // 
            // txtSubject
            // 
            this.txtSubject.BackColor = System.Drawing.SystemColors.Control;
            this.txtSubject.Location = new System.Drawing.Point(25, 223);
            this.txtSubject.Name = "txtSubject";
            this.txtSubject.Size = new System.Drawing.Size(350, 20);
            this.txtSubject.TabIndex = 2;
            this.txtSubject.Text = "Fantastic offer";
            this.txtSubject.TextChanged += new System.EventHandler(this.RecolorTextBox);
            // 
            // txtBodyFile
            // 
            this.txtBodyFile.BackColor = System.Drawing.SystemColors.Control;
            this.txtBodyFile.Location = new System.Drawing.Point(48, 273);
            this.txtBodyFile.Name = "txtBodyFile";
            this.txtBodyFile.Size = new System.Drawing.Size(327, 20);
            this.txtBodyFile.TabIndex = 4;
            this.txtBodyFile.Text = "data\\body.html";
            this.txtBodyFile.TextChanged += new System.EventHandler(this.RecolorTextBox);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(25, 207);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(46, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Subject:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(25, 254);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(80, 13);
            this.label4.TabIndex = 5;
            this.label4.Text = "Body file (TXT):";
            // 
            // btnSelectBody
            // 
            this.btnSelectBody.Location = new System.Drawing.Point(23, 273);
            this.btnSelectBody.Name = "btnSelectBody";
            this.btnSelectBody.Size = new System.Drawing.Size(24, 21);
            this.btnSelectBody.TabIndex = 3;
            this.btnSelectBody.Text = "...";
            this.btnSelectBody.UseVisualStyleBackColor = true;
            this.btnSelectBody.Click += new System.EventHandler(this.btnSelectBody_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Controls.Add(this.udBatchSize);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.udBetweenBatches);
            this.groupBox2.Controls.Add(this.udBetweenEmails);
            this.groupBox2.Location = new System.Drawing.Point(377, 41);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(208, 100);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Batch parameters";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(15, 25);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(59, 13);
            this.label9.TabIndex = 5;
            this.label9.Text = "Batch size:";
            // 
            // udBatchSize
            // 
            this.udBatchSize.Location = new System.Drawing.Point(157, 22);
            this.udBatchSize.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.udBatchSize.Name = "udBatchSize";
            this.udBatchSize.Size = new System.Drawing.Size(44, 20);
            this.udBatchSize.TabIndex = 0;
            this.udBatchSize.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.udBatchSize.ValueChanged += new System.EventHandler(this.RecolorTextBox);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(15, 74);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(118, 13);
            this.label6.TabIndex = 3;
            this.label6.Text = "Between batches (min):";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(15, 49);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(110, 13);
            this.label5.TabIndex = 2;
            this.label5.Text = "Between emails (sec):";
            // 
            // udBetweenBatches
            // 
            this.udBetweenBatches.Location = new System.Drawing.Point(157, 71);
            this.udBetweenBatches.Maximum = new decimal(new int[] {
            1440,
            0,
            0,
            0});
            this.udBetweenBatches.Name = "udBetweenBatches";
            this.udBetweenBatches.Size = new System.Drawing.Size(44, 20);
            this.udBetweenBatches.TabIndex = 2;
            this.udBetweenBatches.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // udBetweenEmails
            // 
            this.udBetweenEmails.Location = new System.Drawing.Point(157, 46);
            this.udBetweenEmails.Name = "udBetweenEmails";
            this.udBetweenEmails.Size = new System.Drawing.Size(44, 20);
            this.udBetweenEmails.TabIndex = 1;
            this.udBetweenEmails.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            // 
            // btnSelectRcpt
            // 
            this.btnSelectRcpt.Location = new System.Drawing.Point(25, 325);
            this.btnSelectRcpt.Name = "btnSelectRcpt";
            this.btnSelectRcpt.Size = new System.Drawing.Size(24, 21);
            this.btnSelectRcpt.TabIndex = 5;
            this.btnSelectRcpt.Text = "...";
            this.btnSelectRcpt.UseVisualStyleBackColor = true;
            this.btnSelectRcpt.Click += new System.EventHandler(this.btnSelectRcpt_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(25, 306);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(101, 13);
            this.label7.TabIndex = 8;
            this.label7.Text = "Recipient file (CSV):";
            // 
            // txtRcptFile
            // 
            this.txtRcptFile.BackColor = System.Drawing.SystemColors.Control;
            this.txtRcptFile.Location = new System.Drawing.Point(50, 325);
            this.txtRcptFile.Name = "txtRcptFile";
            this.txtRcptFile.Size = new System.Drawing.Size(325, 20);
            this.txtRcptFile.TabIndex = 6;
            this.txtRcptFile.Text = "data\\rcpt.txt";
            this.txtRcptFile.TextChanged += new System.EventHandler(this.RecolorTextBox);
            // 
            // txtFrom
            // 
            this.txtFrom.BackColor = System.Drawing.SystemColors.Control;
            this.txtFrom.Location = new System.Drawing.Point(25, 177);
            this.txtFrom.Name = "txtFrom";
            this.txtFrom.Size = new System.Drawing.Size(194, 20);
            this.txtFrom.TabIndex = 1;
            this.txtFrom.Text = "mssmax@gmail.com";
            this.txtFrom.TextChanged += new System.EventHandler(this.RecolorTextBox);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(25, 159);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(33, 13);
            this.label8.TabIndex = 10;
            this.label8.Text = "From:";
            // 
            // lblProgress
            // 
            this.lblProgress.AutoSize = true;
            this.lblProgress.Location = new System.Drawing.Point(201, 384);
            this.lblProgress.Name = "lblProgress";
            this.lblProgress.Size = new System.Drawing.Size(0, 13);
            this.lblProgress.TabIndex = 11;
            // 
            // lstAtts
            // 
            this.lstAtts.FormattingEnabled = true;
            this.lstAtts.HorizontalScrollbar = true;
            this.lstAtts.Location = new System.Drawing.Point(380, 223);
            this.lstAtts.Name = "lstAtts";
            this.lstAtts.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.lstAtts.Size = new System.Drawing.Size(205, 121);
            this.lstAtts.TabIndex = 12;
            this.lstAtts.DoubleClick += new System.EventHandler(this.lstAtts_DoubleClick);
            this.lstAtts.KeyUp += new System.Windows.Forms.KeyEventHandler(this.lstAtts_KeyUp);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(380, 207);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(69, 13);
            this.label10.TabIndex = 13;
            this.label10.Text = "Attachments:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(596, 420);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.lstAtts);
            this.Controls.Add(this.lblProgress);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.txtFrom);
            this.Controls.Add(this.btnSelectRcpt);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.txtRcptFile);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.btnSelectBody);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.txtBodyFile);
            this.Controls.Add(this.txtSubject);
            this.Controls.Add(this.btnSend);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.KeyPreview = true;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Mailer";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udBatchSize)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udBetweenBatches)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udBetweenEmails)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox txtPassword;
        private System.Windows.Forms.TextBox txtUserName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.TextBox txtSubject;
        private System.Windows.Forms.TextBox txtBodyFile;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button btnSelectBody;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.NumericUpDown udBetweenBatches;
        private System.Windows.Forms.NumericUpDown udBetweenEmails;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button btnSelectRcpt;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox txtRcptFile;
        private System.Windows.Forms.TextBox txtFrom;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.NumericUpDown udBatchSize;
        private System.Windows.Forms.Label lblProgress;
        private System.Windows.Forms.ListBox lstAtts;
        private System.Windows.Forms.Label label10;
    }
}

