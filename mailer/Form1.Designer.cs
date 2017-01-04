namespace mailer
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        private bool m_bSending = false;

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
            this.Password = new System.Windows.Forms.TextBox();
            this.UserName = new System.Windows.Forms.TextBox();
            this.Send = new System.Windows.Forms.Button();
            this.BodyText = new System.Windows.Forms.TextBox();
            this.Subject = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.btnSelectBody = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.betweenBatches = new System.Windows.Forms.NumericUpDown();
            this.betweenEmails = new System.Windows.Forms.NumericUpDown();
            this.btnSelectRcpt = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.Recipients = new System.Windows.Forms.TextBox();
            this.From = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.betweenBatches)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.betweenEmails)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.Password);
            this.groupBox1.Controls.Add(this.UserName);
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
            this.label1.Location = new System.Drawing.Point(11, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(32, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "User:";
            // 
            // Password
            // 
            this.Password.Location = new System.Drawing.Point(14, 82);
            this.Password.Name = "Password";
            this.Password.Size = new System.Drawing.Size(172, 20);
            this.Password.TabIndex = 1;
            this.Password.UseSystemPasswordChar = true;
            // 
            // UserName
            // 
            this.UserName.Location = new System.Drawing.Point(14, 38);
            this.UserName.Name = "UserName";
            this.UserName.Size = new System.Drawing.Size(172, 20);
            this.UserName.TabIndex = 0;
            // 
            // Send
            // 
            this.Send.Location = new System.Drawing.Point(25, 376);
            this.Send.Name = "Send";
            this.Send.Size = new System.Drawing.Size(133, 31);
            this.Send.TabIndex = 8;
            this.Send.Text = "Start";
            this.Send.UseVisualStyleBackColor = true;
            this.Send.Click += new System.EventHandler(this.Send_Click);
            // 
            // BodyText
            // 
            this.BodyText.Location = new System.Drawing.Point(25, 223);
            this.BodyText.Name = "BodyText";
            this.BodyText.Size = new System.Drawing.Size(551, 20);
            this.BodyText.TabIndex = 2;
            // 
            // Subject
            // 
            this.Subject.Location = new System.Drawing.Point(25, 278);
            this.Subject.Name = "Subject";
            this.Subject.Size = new System.Drawing.Size(521, 20);
            this.Subject.TabIndex = 3;
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
            this.label4.Location = new System.Drawing.Point(25, 259);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(80, 13);
            this.label4.TabIndex = 5;
            this.label4.Text = "Body file (TXT):";
            // 
            // btnSelectBody
            // 
            this.btnSelectBody.Location = new System.Drawing.Point(552, 278);
            this.btnSelectBody.Name = "btnSelectBody";
            this.btnSelectBody.Size = new System.Drawing.Size(24, 21);
            this.btnSelectBody.TabIndex = 4;
            this.btnSelectBody.Text = "...";
            this.btnSelectBody.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.betweenBatches);
            this.groupBox2.Controls.Add(this.betweenEmails);
            this.groupBox2.Location = new System.Drawing.Point(377, 41);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(199, 98);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Intervals";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(17, 63);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(118, 13);
            this.label6.TabIndex = 3;
            this.label6.Text = "Between batches (min):";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(17, 33);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(110, 13);
            this.label5.TabIndex = 2;
            this.label5.Text = "Between emails (sec):";
            // 
            // betweenBatches
            // 
            this.betweenBatches.Location = new System.Drawing.Point(141, 59);
            this.betweenBatches.Maximum = new decimal(new int[] {
            1440,
            0,
            0,
            0});
            this.betweenBatches.Name = "betweenBatches";
            this.betweenBatches.Size = new System.Drawing.Size(51, 20);
            this.betweenBatches.TabIndex = 1;
            this.betweenBatches.Value = new decimal(new int[] {
            60,
            0,
            0,
            0});
            // 
            // betweenEmails
            // 
            this.betweenEmails.Location = new System.Drawing.Point(141, 29);
            this.betweenEmails.Name = "betweenEmails";
            this.betweenEmails.Size = new System.Drawing.Size(51, 20);
            this.betweenEmails.TabIndex = 0;
            this.betweenEmails.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            // 
            // btnSelectRcpt
            // 
            this.btnSelectRcpt.Location = new System.Drawing.Point(552, 330);
            this.btnSelectRcpt.Name = "btnSelectRcpt";
            this.btnSelectRcpt.Size = new System.Drawing.Size(24, 21);
            this.btnSelectRcpt.TabIndex = 6;
            this.btnSelectRcpt.Text = "...";
            this.btnSelectRcpt.UseVisualStyleBackColor = true;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(25, 312);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(101, 13);
            this.label7.TabIndex = 8;
            this.label7.Text = "Recipient file (CSV):";
            // 
            // Recipients
            // 
            this.Recipients.Location = new System.Drawing.Point(25, 331);
            this.Recipients.Name = "Recipients";
            this.Recipients.Size = new System.Drawing.Size(521, 20);
            this.Recipients.TabIndex = 5;
            // 
            // From
            // 
            this.From.Location = new System.Drawing.Point(25, 177);
            this.From.Name = "From";
            this.From.Size = new System.Drawing.Size(194, 20);
            this.From.TabIndex = 1;
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
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(596, 420);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.From);
            this.Controls.Add(this.btnSelectRcpt);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.Recipients);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.btnSelectBody);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.Subject);
            this.Controls.Add(this.BodyText);
            this.Controls.Add(this.Send);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Mailer";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.betweenBatches)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.betweenEmails)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox Password;
        private System.Windows.Forms.TextBox UserName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button Send;
        private System.Windows.Forms.TextBox BodyText;
        private System.Windows.Forms.TextBox Subject;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button btnSelectBody;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.NumericUpDown betweenBatches;
        private System.Windows.Forms.NumericUpDown betweenEmails;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button btnSelectRcpt;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox Recipients;
        private System.Windows.Forms.TextBox From;
        private System.Windows.Forms.Label label8;
    }
}

