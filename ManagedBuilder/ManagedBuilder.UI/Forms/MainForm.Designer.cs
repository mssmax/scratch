// -----------------------------------------------------------------------
// <copyright file="MainForm.Designer.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
namespace ManagedBuilder.UI.Forms
{
    partial class MainForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.lstProjects = new System.Windows.Forms.ListBox();
            this.projectsSource = new System.Windows.Forms.BindingSource(this.components);
            this.buildFileSource = new System.Windows.Forms.BindingSource(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.txtName = new System.Windows.Forms.TextBox();
            this.txtPath = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.txtSccPath = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.txtBuildOpt = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.txtFramework = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.lstDependencies = new System.Windows.Forms.CheckedListBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tabDetails = new System.Windows.Forms.TabPage();
            this.btnChooseConfig = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.txtDestPath = new System.Windows.Forms.TextBox();
            this.outputInformationBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.btnDeleteBinPlaceOptions = new System.Windows.Forms.Button();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.btnAddBinPlaceOpt = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnAddDirectPath = new System.Windows.Forms.Button();
            this.btnEditDirectPath = new System.Windows.Forms.Button();
            this.txtDirectOutputPath = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.cbBinPlaceDestPath = new System.Windows.Forms.ComboBox();
            this.lstOutputs = new System.Windows.Forms.ListBox();
            this.pathListBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.btnAddOutputFile = new System.Windows.Forms.Button();
            this.btnAddOutputPath = new System.Windows.Forms.Button();
            this.btnDeleteOutputEntry = new System.Windows.Forms.Button();
            this.txtConfigName = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.txtPlatform = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.btnChooseProjectFile = new System.Windows.Forms.Button();
            this.tabDependencies = new System.Windows.Forms.TabPage();
            this.tabChildDeps = new System.Windows.Forms.TabPage();
            this.lstDependants = new System.Windows.Forms.CheckedListBox();
            this.projectDependencySource = new System.Windows.Forms.BindingSource(this.components);
            this.toolStrip = new System.Windows.Forms.ToolStrip();
            this.btnNew = new System.Windows.Forms.ToolStripButton();
            this.btnLoad = new System.Windows.Forms.ToolStripButton();
            this.btnSave = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.btnAddProject = new System.Windows.Forms.ToolStripButton();
            this.btnRemoveProject = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.txtFilter = new System.Windows.Forms.ToolStripTextBox();
            this.btnResetFilter = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.btnShowDuplicates = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.buildProjectButton = new System.Windows.Forms.ToolStripSplitButton();
            this.buildToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.buildWithoutDependenciesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newProjectFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.recentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.clearToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.projectsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.importOneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importManyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.detailsViewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dependenciesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.childDependenciesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.projectListToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.filterToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem5 = new System.Windows.Forms.ToolStripSeparator();
            this.previousProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.nextProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.buildToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewBuildOrderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.binPlaceFileEditorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.tslblDuplicateEntries = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblModifying = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblSaving = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblSaved = new System.Windows.Forms.ToolStripStatusLabel();
            this.projectChildDepsSource = new System.Windows.Forms.BindingSource(this.components);
            this.filterTimer = new System.Windows.Forms.Timer(this.components);
            this.savedLabelTimer = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.projectsSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.buildFileSource)).BeginInit();
            this.tabControl.SuspendLayout();
            this.tabDetails.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.outputInformationBindingSource)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pathListBindingSource)).BeginInit();
            this.tabDependencies.SuspendLayout();
            this.tabChildDeps.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.projectDependencySource)).BeginInit();
            this.toolStrip.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.statusStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.projectChildDepsSource)).BeginInit();
            this.SuspendLayout();
            // 
            // lstProjects
            // 
            this.lstProjects.DataSource = this.projectsSource;
            this.lstProjects.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lstProjects.FormattingEnabled = true;
            this.lstProjects.IntegralHeight = false;
            this.lstProjects.Location = new System.Drawing.Point(0, 0);
            this.lstProjects.Name = "lstProjects";
            this.lstProjects.Size = new System.Drawing.Size(273, 659);
            this.lstProjects.TabIndex = 0;
            this.lstProjects.KeyUp += new System.Windows.Forms.KeyEventHandler(this.lstProjects_KeyUp);
            // 
            // projectsSource
            // 
            this.projectsSource.DataMember = "Projects";
            this.projectsSource.DataSource = this.buildFileSource;
            this.projectsSource.Sort = "";
            this.projectsSource.CurrentItemChanged += new System.EventHandler(this.projectsSource_CurrentItemChanged);
            this.projectsSource.ListChanged += new System.ComponentModel.ListChangedEventHandler(this.projectsSource_ListChanged);
            // 
            // buildFileSource
            // 
            this.buildFileSource.DataSource = typeof(ManagedBuilder.UI.Adapters.BuildFileAdapter);
            this.buildFileSource.CurrentItemChanged += new System.EventHandler(this.buildFileSource_CurrentItemChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "&Name";
            // 
            // txtName
            // 
            this.txtName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtName.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.projectsSource, "Name", true));
            this.txtName.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtName.Location = new System.Drawing.Point(9, 28);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(520, 20);
            this.txtName.TabIndex = 4;
            // 
            // txtPath
            // 
            this.txtPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtPath.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.projectsSource, "Path", true));
            this.txtPath.Location = new System.Drawing.Point(9, 68);
            this.txtPath.Name = "txtPath";
            this.txtPath.Size = new System.Drawing.Size(482, 20);
            this.txtPath.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 51);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(59, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "&Project File";
            // 
            // txtSccPath
            // 
            this.txtSccPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtSccPath.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.projectsSource, "SccPath", true));
            this.txtSccPath.Location = new System.Drawing.Point(9, 107);
            this.txtSccPath.Name = "txtSccPath";
            this.txtSccPath.Size = new System.Drawing.Size(520, 20);
            this.txtSccPath.TabIndex = 9;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 91);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(102, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "&Source Control Path";
            // 
            // txtBuildOpt
            // 
            this.txtBuildOpt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtBuildOpt.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.projectsSource, "BuildOpt", true));
            this.txtBuildOpt.Location = new System.Drawing.Point(9, 185);
            this.txtBuildOpt.Multiline = true;
            this.txtBuildOpt.Name = "txtBuildOpt";
            this.txtBuildOpt.Size = new System.Drawing.Size(519, 52);
            this.txtBuildOpt.TabIndex = 17;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 169);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(69, 13);
            this.label4.TabIndex = 16;
            this.label4.Text = "&Build Options";
            // 
            // txtFramework
            // 
            this.txtFramework.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.projectsSource, "Framework", true));
            this.txtFramework.Location = new System.Drawing.Point(9, 146);
            this.txtFramework.Name = "txtFramework";
            this.txtFramework.Size = new System.Drawing.Size(69, 20);
            this.txtFramework.TabIndex = 11;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 130);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(59, 13);
            this.label5.TabIndex = 10;
            this.label5.Text = "&Framework";
            // 
            // lstDependencies
            // 
            this.lstDependencies.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lstDependencies.ColumnWidth = 250;
            this.lstDependencies.FormattingEnabled = true;
            this.lstDependencies.IntegralHeight = false;
            this.lstDependencies.Location = new System.Drawing.Point(6, 6);
            this.lstDependencies.MultiColumn = true;
            this.lstDependencies.Name = "lstDependencies";
            this.lstDependencies.Size = new System.Drawing.Size(490, 621);
            this.lstDependencies.TabIndex = 8;
            this.lstDependencies.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.lstDependencies_ItemCheck);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 94);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(94, 13);
            this.label6.TabIndex = 6;
            this.label6.Text = "Source &Directories";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 55);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(127, 13);
            this.label7.TabIndex = 4;
            this.label7.Text = "&Relative Destination Path";
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabDetails);
            this.tabControl.Controls.Add(this.tabDependencies);
            this.tabControl.Controls.Add(this.tabChildDeps);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(545, 659);
            this.tabControl.TabIndex = 17;
            this.tabControl.SizeChanged += new System.EventHandler(this.tabControl1_SizeChanged);
            // 
            // tabDetails
            // 
            this.tabDetails.BackColor = System.Drawing.SystemColors.Control;
            this.tabDetails.Controls.Add(this.btnChooseConfig);
            this.tabDetails.Controls.Add(this.groupBox1);
            this.tabDetails.Controls.Add(this.txtConfigName);
            this.tabDetails.Controls.Add(this.label10);
            this.tabDetails.Controls.Add(this.txtPlatform);
            this.tabDetails.Controls.Add(this.label9);
            this.tabDetails.Controls.Add(this.btnChooseProjectFile);
            this.tabDetails.Controls.Add(this.label1);
            this.tabDetails.Controls.Add(this.txtName);
            this.tabDetails.Controls.Add(this.label2);
            this.tabDetails.Controls.Add(this.txtPath);
            this.tabDetails.Controls.Add(this.label3);
            this.tabDetails.Controls.Add(this.txtSccPath);
            this.tabDetails.Controls.Add(this.label4);
            this.tabDetails.Controls.Add(this.txtFramework);
            this.tabDetails.Controls.Add(this.txtBuildOpt);
            this.tabDetails.Controls.Add(this.label5);
            this.tabDetails.Location = new System.Drawing.Point(4, 22);
            this.tabDetails.Name = "tabDetails";
            this.tabDetails.Padding = new System.Windows.Forms.Padding(3);
            this.tabDetails.Size = new System.Drawing.Size(537, 633);
            this.tabDetails.TabIndex = 0;
            this.tabDetails.Text = "Details";
            // 
            // btnChooseConfig
            // 
            this.btnChooseConfig.Location = new System.Drawing.Point(497, 146);
            this.btnChooseConfig.Name = "btnChooseConfig";
            this.btnChooseConfig.Size = new System.Drawing.Size(32, 21);
            this.btnChooseConfig.TabIndex = 19;
            this.btnChooseConfig.Text = "...";
            this.btnChooseConfig.UseVisualStyleBackColor = true;
            this.btnChooseConfig.Click += new System.EventHandler(this.btnChooseConfig_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.txtDestPath);
            this.groupBox1.Controls.Add(this.btnDeleteBinPlaceOptions);
            this.groupBox1.Controls.Add(this.btnAddBinPlaceOpt);
            this.groupBox1.Controls.Add(this.groupBox2);
            this.groupBox1.Controls.Add(this.cbBinPlaceDestPath);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.lstOutputs);
            this.groupBox1.Controls.Add(this.btnAddOutputFile);
            this.groupBox1.Controls.Add(this.btnAddOutputPath);
            this.groupBox1.Controls.Add(this.btnDeleteOutputEntry);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Location = new System.Drawing.Point(8, 243);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(3, 10, 3, 3);
            this.groupBox1.Size = new System.Drawing.Size(520, 339);
            this.groupBox1.TabIndex = 18;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Binary Placement Options";
            // 
            // txtDestPath
            // 
            this.txtDestPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtDestPath.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.outputInformationBindingSource, "DestPath", true));
            this.txtDestPath.Location = new System.Drawing.Point(9, 71);
            this.txtDestPath.Name = "txtDestPath";
            this.txtDestPath.Size = new System.Drawing.Size(505, 20);
            this.txtDestPath.TabIndex = 5;
            // 
            // outputInformationBindingSource
            // 
            this.outputInformationBindingSource.DataMember = "OutputInformation";
            this.outputInformationBindingSource.DataSource = this.projectsSource;
            // 
            // btnDeleteBinPlaceOptions
            // 
            this.btnDeleteBinPlaceOptions.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnDeleteBinPlaceOptions.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.btnDeleteBinPlaceOptions.ImageIndex = 1;
            this.btnDeleteBinPlaceOptions.ImageList = this.imageList;
            this.btnDeleteBinPlaceOptions.Location = new System.Drawing.Point(486, 21);
            this.btnDeleteBinPlaceOptions.Name = "btnDeleteBinPlaceOptions";
            this.btnDeleteBinPlaceOptions.Size = new System.Drawing.Size(28, 28);
            this.btnDeleteBinPlaceOptions.TabIndex = 3;
            this.btnDeleteBinPlaceOptions.TabStop = false;
            this.btnDeleteBinPlaceOptions.UseVisualStyleBackColor = true;
            this.btnDeleteBinPlaceOptions.Click += new System.EventHandler(this.btnDeleteBinPlaceOptions_Click);
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList.Images.SetKeyName(0, "Plus");
            this.imageList.Images.SetKeyName(1, "Delete");
            this.imageList.Images.SetKeyName(2, "Edit");
            this.imageList.Images.SetKeyName(3, "AddFile");
            this.imageList.Images.SetKeyName(4, "AddFolder");
            // 
            // btnAddBinPlaceOpt
            // 
            this.btnAddBinPlaceOpt.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnAddBinPlaceOpt.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.btnAddBinPlaceOpt.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnAddBinPlaceOpt.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnAddBinPlaceOpt.ImageIndex = 0;
            this.btnAddBinPlaceOpt.ImageList = this.imageList;
            this.btnAddBinPlaceOpt.Location = new System.Drawing.Point(332, 21);
            this.btnAddBinPlaceOpt.Name = "btnAddBinPlaceOpt";
            this.btnAddBinPlaceOpt.Size = new System.Drawing.Size(148, 28);
            this.btnAddBinPlaceOpt.TabIndex = 2;
            this.btnAddBinPlaceOpt.Text = "Add Placement Options";
            this.btnAddBinPlaceOpt.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnAddBinPlaceOpt.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnAddBinPlaceOpt.UseVisualStyleBackColor = true;
            this.btnAddBinPlaceOpt.Click += new System.EventHandler(this.btnAddBinPlaceOpt_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.btnAddDirectPath);
            this.groupBox2.Controls.Add(this.btnEditDirectPath);
            this.groupBox2.Controls.Add(this.txtDirectOutputPath);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Location = new System.Drawing.Point(9, 224);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(505, 106);
            this.groupBox2.TabIndex = 8;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Direct Output Path Editor";
            // 
            // btnAddDirectPath
            // 
            this.btnAddDirectPath.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.btnAddDirectPath.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnAddDirectPath.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnAddDirectPath.ImageIndex = 0;
            this.btnAddDirectPath.ImageList = this.imageList;
            this.btnAddDirectPath.Location = new System.Drawing.Point(9, 68);
            this.btnAddDirectPath.Name = "btnAddDirectPath";
            this.btnAddDirectPath.Size = new System.Drawing.Size(139, 28);
            this.btnAddDirectPath.TabIndex = 3;
            this.btnAddDirectPath.Text = "Add Source Path";
            this.btnAddDirectPath.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnAddDirectPath.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnAddDirectPath.UseVisualStyleBackColor = true;
            this.btnAddDirectPath.Click += new System.EventHandler(this.btnAddDirectPath_Click);
            // 
            // btnEditDirectPath
            // 
            this.btnEditDirectPath.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.btnEditDirectPath.Font = new System.Drawing.Font("Wingdings 2", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(2)));
            this.btnEditDirectPath.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.btnEditDirectPath.ImageIndex = 2;
            this.btnEditDirectPath.ImageList = this.imageList;
            this.btnEditDirectPath.Location = new System.Drawing.Point(154, 69);
            this.btnEditDirectPath.Name = "btnEditDirectPath";
            this.btnEditDirectPath.Size = new System.Drawing.Size(28, 28);
            this.btnEditDirectPath.TabIndex = 4;
            this.btnEditDirectPath.UseVisualStyleBackColor = true;
            this.btnEditDirectPath.Click += new System.EventHandler(this.btnEditDirectPath_Click);
            // 
            // txtDirectOutputPath
            // 
            this.txtDirectOutputPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtDirectOutputPath.Location = new System.Drawing.Point(9, 42);
            this.txtDirectOutputPath.Name = "txtDirectOutputPath";
            this.txtDirectOutputPath.Size = new System.Drawing.Size(490, 20);
            this.txtDirectOutputPath.TabIndex = 2;
            this.txtDirectOutputPath.KeyUp += new System.Windows.Forms.KeyEventHandler(this.txtDirectOutputPath_KeyUp);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 26);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(64, 13);
            this.label8.TabIndex = 1;
            this.label8.Text = "O&utput Path";
            this.label8.Click += new System.EventHandler(this.label8_Click);
            // 
            // cbBinPlaceDestPath
            // 
            this.cbBinPlaceDestPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.cbBinPlaceDestPath.DataSource = this.outputInformationBindingSource;
            this.cbBinPlaceDestPath.DisplayMember = "DestPath";
            this.cbBinPlaceDestPath.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbBinPlaceDestPath.FormattingEnabled = true;
            this.cbBinPlaceDestPath.Location = new System.Drawing.Point(9, 26);
            this.cbBinPlaceDestPath.Name = "cbBinPlaceDestPath";
            this.cbBinPlaceDestPath.Size = new System.Drawing.Size(317, 21);
            this.cbBinPlaceDestPath.TabIndex = 1;
            // 
            // lstOutputs
            // 
            this.lstOutputs.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lstOutputs.DataSource = this.pathListBindingSource;
            this.lstOutputs.FormattingEnabled = true;
            this.lstOutputs.Location = new System.Drawing.Point(9, 110);
            this.lstOutputs.Name = "lstOutputs";
            this.lstOutputs.Size = new System.Drawing.Size(471, 108);
            this.lstOutputs.TabIndex = 7;
            this.lstOutputs.KeyDown += new System.Windows.Forms.KeyEventHandler(this.lstOutputs_KeyDown);
            // 
            // pathListBindingSource
            // 
            this.pathListBindingSource.DataMember = "PathList";
            this.pathListBindingSource.DataSource = this.outputInformationBindingSource;
            // 
            // btnAddOutputFile
            // 
            this.btnAddOutputFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnAddOutputFile.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.btnAddOutputFile.ImageIndex = 3;
            this.btnAddOutputFile.ImageList = this.imageList;
            this.btnAddOutputFile.Location = new System.Drawing.Point(486, 110);
            this.btnAddOutputFile.Name = "btnAddOutputFile";
            this.btnAddOutputFile.Size = new System.Drawing.Size(28, 28);
            this.btnAddOutputFile.TabIndex = 0;
            this.btnAddOutputFile.TabStop = false;
            this.btnAddOutputFile.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
            this.btnAddOutputFile.UseVisualStyleBackColor = true;
            this.btnAddOutputFile.Click += new System.EventHandler(this.btnAddOutputFile_Click);
            // 
            // btnAddOutputPath
            // 
            this.btnAddOutputPath.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnAddOutputPath.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.btnAddOutputPath.ImageIndex = 4;
            this.btnAddOutputPath.ImageList = this.imageList;
            this.btnAddOutputPath.Location = new System.Drawing.Point(486, 144);
            this.btnAddOutputPath.Name = "btnAddOutputPath";
            this.btnAddOutputPath.Size = new System.Drawing.Size(28, 28);
            this.btnAddOutputPath.TabIndex = 0;
            this.btnAddOutputPath.TabStop = false;
            this.btnAddOutputPath.UseVisualStyleBackColor = true;
            this.btnAddOutputPath.Click += new System.EventHandler(this.btnAddOutputPath_Click);
            // 
            // btnDeleteOutputEntry
            // 
            this.btnDeleteOutputEntry.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnDeleteOutputEntry.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.btnDeleteOutputEntry.ImageIndex = 1;
            this.btnDeleteOutputEntry.ImageList = this.imageList;
            this.btnDeleteOutputEntry.Location = new System.Drawing.Point(486, 178);
            this.btnDeleteOutputEntry.Name = "btnDeleteOutputEntry";
            this.btnDeleteOutputEntry.Size = new System.Drawing.Size(28, 28);
            this.btnDeleteOutputEntry.TabIndex = 0;
            this.btnDeleteOutputEntry.TabStop = false;
            this.btnDeleteOutputEntry.UseVisualStyleBackColor = true;
            this.btnDeleteOutputEntry.Click += new System.EventHandler(this.btnDeleteOutputEntry_Click);
            // 
            // txtConfigName
            // 
            this.txtConfigName.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.projectsSource, "ConfigurationName", true));
            this.txtConfigName.Location = new System.Drawing.Point(187, 146);
            this.txtConfigName.Name = "txtConfigName";
            this.txtConfigName.Size = new System.Drawing.Size(304, 20);
            this.txtConfigName.TabIndex = 15;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(184, 130);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(100, 13);
            this.label10.TabIndex = 14;
            this.label10.Text = "&Configuration Name";
            // 
            // txtPlatform
            // 
            this.txtPlatform.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.projectsSource, "PlatformName", true));
            this.txtPlatform.Location = new System.Drawing.Point(84, 146);
            this.txtPlatform.Name = "txtPlatform";
            this.txtPlatform.Size = new System.Drawing.Size(97, 20);
            this.txtPlatform.TabIndex = 13;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(81, 130);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(79, 13);
            this.label9.TabIndex = 12;
            this.label9.Text = "&Target Platform";
            // 
            // btnChooseProjectFile
            // 
            this.btnChooseProjectFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnChooseProjectFile.Location = new System.Drawing.Point(497, 67);
            this.btnChooseProjectFile.Name = "btnChooseProjectFile";
            this.btnChooseProjectFile.Size = new System.Drawing.Size(32, 21);
            this.btnChooseProjectFile.TabIndex = 7;
            this.btnChooseProjectFile.Text = "...";
            this.btnChooseProjectFile.UseVisualStyleBackColor = true;
            this.btnChooseProjectFile.Click += new System.EventHandler(this.btnChooseProjectFile_Click);
            // 
            // tabDependencies
            // 
            this.tabDependencies.Controls.Add(this.lstDependencies);
            this.tabDependencies.Location = new System.Drawing.Point(4, 22);
            this.tabDependencies.Name = "tabDependencies";
            this.tabDependencies.Padding = new System.Windows.Forms.Padding(3);
            this.tabDependencies.Size = new System.Drawing.Size(537, 633);
            this.tabDependencies.TabIndex = 1;
            this.tabDependencies.Text = "Depends On";
            this.tabDependencies.UseVisualStyleBackColor = true;
            // 
            // tabChildDeps
            // 
            this.tabChildDeps.Controls.Add(this.lstDependants);
            this.tabChildDeps.Location = new System.Drawing.Point(4, 22);
            this.tabChildDeps.Name = "tabChildDeps";
            this.tabChildDeps.Padding = new System.Windows.Forms.Padding(3);
            this.tabChildDeps.Size = new System.Drawing.Size(537, 633);
            this.tabChildDeps.TabIndex = 2;
            this.tabChildDeps.Text = "Child Dependants";
            this.tabChildDeps.UseVisualStyleBackColor = true;
            // 
            // lstDependants
            // 
            this.lstDependants.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lstDependants.ColumnWidth = 250;
            this.lstDependants.FormattingEnabled = true;
            this.lstDependants.IntegralHeight = false;
            this.lstDependants.Location = new System.Drawing.Point(6, 6);
            this.lstDependants.MultiColumn = true;
            this.lstDependants.Name = "lstDependants";
            this.lstDependants.Size = new System.Drawing.Size(490, 621);
            this.lstDependants.TabIndex = 9;
            this.lstDependants.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.lstDependants_ItemCheck);
            // 
            // projectDependencySource
            // 
            this.projectDependencySource.DataMember = "Projects";
            this.projectDependencySource.DataSource = this.buildFileSource;
            this.projectDependencySource.ListChanged += new System.ComponentModel.ListChangedEventHandler(this.projectDependencySource_ListChanged);
            // 
            // toolStrip
            // 
            this.toolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnNew,
            this.btnLoad,
            this.btnSave,
            this.toolStripSeparator1,
            this.btnAddProject,
            this.btnRemoveProject,
            this.toolStripSeparator2,
            this.toolStripLabel1,
            this.txtFilter,
            this.btnResetFilter,
            this.toolStripSeparator3,
            this.btnShowDuplicates,
            this.toolStripSeparator4,
            this.buildProjectButton,
            this.toolStripButton1});
            this.toolStrip.Location = new System.Drawing.Point(0, 24);
            this.toolStrip.Name = "toolStrip";
            this.toolStrip.Size = new System.Drawing.Size(822, 25);
            this.toolStrip.TabIndex = 18;
            this.toolStrip.Text = "toolStrip1";
            // 
            // btnNew
            // 
            this.btnNew.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnNew.Image = ((System.Drawing.Image)(resources.GetObject("btnNew.Image")));
            this.btnNew.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnNew.Name = "btnNew";
            this.btnNew.Size = new System.Drawing.Size(23, 22);
            this.btnNew.Text = "New ...";
            this.btnNew.Click += new System.EventHandler(this.btnNew_Click);
            // 
            // btnLoad
            // 
            this.btnLoad.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnLoad.Image = global::ManagedBuilder.UI.Properties.Resources.openHS;
            this.btnLoad.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(23, 22);
            this.btnLoad.Text = "Open Project File";
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // btnSave
            // 
            this.btnSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnSave.Image = global::ManagedBuilder.UI.Properties.Resources.saveHS;
            this.btnSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(23, 22);
            this.btnSave.Text = "Save Project File";
            this.btnSave.Click += new System.EventHandler(this.bntSave_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // btnAddProject
            // 
            this.btnAddProject.Image = global::ManagedBuilder.UI.Properties.Resources.Plus;
            this.btnAddProject.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAddProject.Name = "btnAddProject";
            this.btnAddProject.Size = new System.Drawing.Size(89, 22);
            this.btnAddProject.Text = "Add Project";
            this.btnAddProject.Click += new System.EventHandler(this.btnAddProject_Click);
            // 
            // btnRemoveProject
            // 
            this.btnRemoveProject.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnRemoveProject.Image = global::ManagedBuilder.UI.Properties.Resources.delete;
            this.btnRemoveProject.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnRemoveProject.Name = "btnRemoveProject";
            this.btnRemoveProject.Size = new System.Drawing.Size(23, 22);
            this.btnRemoveProject.Text = "Remove Project";
            this.btnRemoveProject.Click += new System.EventHandler(this.btnRemoveProject_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(33, 22);
            this.toolStripLabel1.Text = "Filter";
            // 
            // txtFilter
            // 
            this.txtFilter.Name = "txtFilter";
            this.txtFilter.Size = new System.Drawing.Size(200, 25);
            this.txtFilter.KeyUp += new System.Windows.Forms.KeyEventHandler(this.txtFilter_KeyUp);
            this.txtFilter.TextChanged += new System.EventHandler(this.txtFilter_TextChanged);
            // 
            // btnResetFilter
            // 
            this.btnResetFilter.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnResetFilter.Image = global::ManagedBuilder.UI.Properties.Resources.Close;
            this.btnResetFilter.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnResetFilter.Name = "btnResetFilter";
            this.btnResetFilter.Size = new System.Drawing.Size(23, 22);
            this.btnResetFilter.Text = "Reset String Filter";
            this.btnResetFilter.Click += new System.EventHandler(this.btnResetFilter_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // btnShowDuplicates
            // 
            this.btnShowDuplicates.CheckOnClick = true;
            this.btnShowDuplicates.Image = global::ManagedBuilder.UI.Properties.Resources.Filter;
            this.btnShowDuplicates.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnShowDuplicates.Name = "btnShowDuplicates";
            this.btnShowDuplicates.Size = new System.Drawing.Size(114, 22);
            this.btnShowDuplicates.Text = "Show Duplicates";
            this.btnShowDuplicates.Click += new System.EventHandler(this.btnShowDuplicates_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 25);
            // 
            // buildProjectButton
            // 
            this.buildProjectButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.buildProjectButton.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.buildToolStripMenuItem1,
            this.buildWithoutDependenciesToolStripMenuItem});
            this.buildProjectButton.Image = ((System.Drawing.Image)(resources.GetObject("buildProjectButton.Image")));
            this.buildProjectButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buildProjectButton.Name = "buildProjectButton";
            this.buildProjectButton.Size = new System.Drawing.Size(32, 22);
            this.buildProjectButton.Text = "toolStripSplitButton1";
            this.buildProjectButton.ButtonClick += new System.EventHandler(this.buildProject_Click);
            // 
            // buildToolStripMenuItem1
            // 
            this.buildToolStripMenuItem1.Name = "buildToolStripMenuItem1";
            this.buildToolStripMenuItem1.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
                        | System.Windows.Forms.Keys.B)));
            this.buildToolStripMenuItem1.Size = new System.Drawing.Size(222, 22);
            this.buildToolStripMenuItem1.Text = "&Build";
            // 
            // buildWithoutDependenciesToolStripMenuItem
            // 
            this.buildWithoutDependenciesToolStripMenuItem.Name = "buildWithoutDependenciesToolStripMenuItem";
            this.buildWithoutDependenciesToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.buildWithoutDependenciesToolStripMenuItem.Text = "Build without Dependencies";
            this.buildWithoutDependenciesToolStripMenuItem.Click += new System.EventHandler(this.buildWithoutDependenciesToolStripMenuItem_Click);
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton1.Text = "toolStripButton1";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.projectsToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.buildToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(822, 24);
            this.menuStrip1.TabIndex = 19;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newProjectFileToolStripMenuItem,
            this.openToolStripMenuItem,
            this.recentToolStripMenuItem,
            this.toolStripMenuItem3,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.toolStripMenuItem1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newProjectFileToolStripMenuItem
            // 
            this.newProjectFileToolStripMenuItem.Name = "newProjectFileToolStripMenuItem";
            this.newProjectFileToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.newProjectFileToolStripMenuItem.Size = new System.Drawing.Size(202, 22);
            this.newProjectFileToolStripMenuItem.Text = "&New Project File";
            this.newProjectFileToolStripMenuItem.Click += new System.EventHandler(this.btnNew_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openToolStripMenuItem.Size = new System.Drawing.Size(202, 22);
            this.openToolStripMenuItem.Text = "&Open ...";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // recentToolStripMenuItem
            // 
            this.recentToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.clearToolStripMenuItem,
            this.toolStripMenuItem4});
            this.recentToolStripMenuItem.Name = "recentToolStripMenuItem";
            this.recentToolStripMenuItem.Size = new System.Drawing.Size(202, 22);
            this.recentToolStripMenuItem.Text = "&Recent";
            // 
            // clearToolStripMenuItem
            // 
            this.clearToolStripMenuItem.Name = "clearToolStripMenuItem";
            this.clearToolStripMenuItem.Size = new System.Drawing.Size(101, 22);
            this.clearToolStripMenuItem.Text = "&Clear";
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(98, 6);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(199, 6);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(202, 22);
            this.saveToolStripMenuItem.Text = "&Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.bntSave_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(202, 22);
            this.saveAsToolStripMenuItem.Text = "Save &As ...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(199, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(202, 22);
            this.exitToolStripMenuItem.Text = "&Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // projectsToolStripMenuItem
            // 
            this.projectsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripMenuItem,
            this.removeToolStripMenuItem,
            this.toolStripMenuItem2,
            this.importOneToolStripMenuItem,
            this.importManyToolStripMenuItem});
            this.projectsToolStripMenuItem.Name = "projectsToolStripMenuItem";
            this.projectsToolStripMenuItem.Size = new System.Drawing.Size(61, 20);
            this.projectsToolStripMenuItem.Text = "&Projects";
            // 
            // addToolStripMenuItem
            // 
            this.addToolStripMenuItem.Image = global::ManagedBuilder.UI.Properties.Resources.Plus;
            this.addToolStripMenuItem.Name = "addToolStripMenuItem";
            this.addToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.A)));
            this.addToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.addToolStripMenuItem.Text = "&Add ";
            this.addToolStripMenuItem.Click += new System.EventHandler(this.btnAddProject_Click);
            // 
            // removeToolStripMenuItem
            // 
            this.removeToolStripMenuItem.Image = global::ManagedBuilder.UI.Properties.Resources.delete;
            this.removeToolStripMenuItem.Name = "removeToolStripMenuItem";
            this.removeToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.R)));
            this.removeToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.removeToolStripMenuItem.Text = "&Remove";
            this.removeToolStripMenuItem.Click += new System.EventHandler(this.btnRemoveProject_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(155, 6);
            // 
            // importOneToolStripMenuItem
            // 
            this.importOneToolStripMenuItem.Name = "importOneToolStripMenuItem";
            this.importOneToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.importOneToolStripMenuItem.Text = "&Import One";
            this.importOneToolStripMenuItem.Click += new System.EventHandler(this.importOneToolStripMenuItem_Click);
            // 
            // importManyToolStripMenuItem
            // 
            this.importManyToolStripMenuItem.Name = "importManyToolStripMenuItem";
            this.importManyToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.importManyToolStripMenuItem.Text = "Import &Many";
            this.importManyToolStripMenuItem.Click += new System.EventHandler(this.importManyToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.detailsViewToolStripMenuItem,
            this.dependenciesToolStripMenuItem,
            this.childDependenciesToolStripMenuItem,
            this.projectListToolStripMenuItem,
            this.filterToolStripMenuItem,
            this.toolStripMenuItem5,
            this.previousProjectToolStripMenuItem,
            this.nextProjectToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.viewToolStripMenuItem.Text = "&View";
            // 
            // detailsViewToolStripMenuItem
            // 
            this.detailsViewToolStripMenuItem.Name = "detailsViewToolStripMenuItem";
            this.detailsViewToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.D1)));
            this.detailsViewToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.detailsViewToolStripMenuItem.Text = "&Details View";
            this.detailsViewToolStripMenuItem.Click += new System.EventHandler(this.SwitchDetailsView);
            // 
            // dependenciesToolStripMenuItem
            // 
            this.dependenciesToolStripMenuItem.Name = "dependenciesToolStripMenuItem";
            this.dependenciesToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.D2)));
            this.dependenciesToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.dependenciesToolStripMenuItem.Text = "&Dependencies";
            this.dependenciesToolStripMenuItem.Click += new System.EventHandler(this.SwitchDependenciesView);
            // 
            // childDependenciesToolStripMenuItem
            // 
            this.childDependenciesToolStripMenuItem.Name = "childDependenciesToolStripMenuItem";
            this.childDependenciesToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.D3)));
            this.childDependenciesToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.childDependenciesToolStripMenuItem.Text = "&Child Dependencies";
            this.childDependenciesToolStripMenuItem.Click += new System.EventHandler(this.SwitchChildDependsView);
            // 
            // projectListToolStripMenuItem
            // 
            this.projectListToolStripMenuItem.Name = "projectListToolStripMenuItem";
            this.projectListToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.D4)));
            this.projectListToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.projectListToolStripMenuItem.Text = "Project &List";
            this.projectListToolStripMenuItem.Click += new System.EventHandler(this.JumpToProjectList);
            // 
            // filterToolStripMenuItem
            // 
            this.filterToolStripMenuItem.Name = "filterToolStripMenuItem";
            this.filterToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F3;
            this.filterToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.filterToolStripMenuItem.Text = "&Filter";
            this.filterToolStripMenuItem.Click += new System.EventHandler(this.JumpToFilter);
            // 
            // toolStripMenuItem5
            // 
            this.toolStripMenuItem5.Name = "toolStripMenuItem5";
            this.toolStripMenuItem5.Size = new System.Drawing.Size(212, 6);
            // 
            // previousProjectToolStripMenuItem
            // 
            this.previousProjectToolStripMenuItem.Name = "previousProjectToolStripMenuItem";
            this.previousProjectToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.K)));
            this.previousProjectToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.previousProjectToolStripMenuItem.Text = "&Previous Project";
            this.previousProjectToolStripMenuItem.Click += new System.EventHandler(this.MovePrevProjectEvent);
            // 
            // nextProjectToolStripMenuItem
            // 
            this.nextProjectToolStripMenuItem.Name = "nextProjectToolStripMenuItem";
            this.nextProjectToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.J)));
            this.nextProjectToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.nextProjectToolStripMenuItem.Text = "&Next Project";
            this.nextProjectToolStripMenuItem.Click += new System.EventHandler(this.MoveNextProjectEvent);
            // 
            // buildToolStripMenuItem
            // 
            this.buildToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.viewBuildOrderToolStripMenuItem,
            this.binPlaceFileEditorToolStripMenuItem});
            this.buildToolStripMenuItem.Name = "buildToolStripMenuItem";
            this.buildToolStripMenuItem.Size = new System.Drawing.Size(48, 20);
            this.buildToolStripMenuItem.Text = "&Tools";
            // 
            // viewBuildOrderToolStripMenuItem
            // 
            this.viewBuildOrderToolStripMenuItem.Name = "viewBuildOrderToolStripMenuItem";
            this.viewBuildOrderToolStripMenuItem.Size = new System.Drawing.Size(174, 22);
            this.viewBuildOrderToolStripMenuItem.Text = "&View Build Order";
            // 
            // binPlaceFileEditorToolStripMenuItem
            // 
            this.binPlaceFileEditorToolStripMenuItem.Name = "binPlaceFileEditorToolStripMenuItem";
            this.binPlaceFileEditorToolStripMenuItem.Size = new System.Drawing.Size(174, 22);
            this.binPlaceFileEditorToolStripMenuItem.Text = "&BinPlace File Editor";
            this.binPlaceFileEditorToolStripMenuItem.Click += new System.EventHandler(this.binPlaceFileEditorToolStripMenuItem_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 49);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.lstProjects);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.tabControl);
            this.splitContainer1.Size = new System.Drawing.Size(822, 659);
            this.splitContainer1.SplitterDistance = 273;
            this.splitContainer1.TabIndex = 20;
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tslblDuplicateEntries,
            this.lblModifying,
            this.lblSaving,
            this.lblSaved});
            this.statusStrip.Location = new System.Drawing.Point(0, 708);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(822, 22);
            this.statusStrip.TabIndex = 21;
            this.statusStrip.Text = "statusStrip1";
            // 
            // tslblDuplicateEntries
            // 
            this.tslblDuplicateEntries.Image = ((System.Drawing.Image)(resources.GetObject("tslblDuplicateEntries.Image")));
            this.tslblDuplicateEntries.Name = "tslblDuplicateEntries";
            this.tslblDuplicateEntries.Size = new System.Drawing.Size(274, 17);
            this.tslblDuplicateEntries.Text = "There are multiple projects with the same name";
            this.tslblDuplicateEntries.Visible = false;
            // 
            // lblModifying
            // 
            this.lblModifying.Name = "lblModifying";
            this.lblModifying.Size = new System.Drawing.Size(67, 17);
            this.lblModifying.Text = "Modified ...";
            this.lblModifying.Visible = false;
            // 
            // lblSaving
            // 
            this.lblSaving.Name = "lblSaving";
            this.lblSaving.Size = new System.Drawing.Size(54, 17);
            this.lblSaving.Tag = "";
            this.lblSaving.Text = "Saving ...";
            this.lblSaving.Visible = false;
            // 
            // lblSaved
            // 
            this.lblSaved.Name = "lblSaved";
            this.lblSaved.Size = new System.Drawing.Size(58, 17);
            this.lblSaved.Text = "File saved";
            this.lblSaved.Visible = false;
            // 
            // projectChildDepsSource
            // 
            this.projectChildDepsSource.DataMember = "Projects";
            this.projectChildDepsSource.DataSource = this.buildFileSource;
            // 
            // filterTimer
            // 
            this.filterTimer.Interval = 250;
            this.filterTimer.Tick += new System.EventHandler(this.filterTimer_Tick);
            // 
            // savedLabelTimer
            // 
            this.savedLabelTimer.Interval = 2000;
            this.savedLabelTimer.Tick += new System.EventHandler(this.savedLabelTimer_Tick);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(822, 730);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.toolStrip);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "CodeMason";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.projectsSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.buildFileSource)).EndInit();
            this.tabControl.ResumeLayout(false);
            this.tabDetails.ResumeLayout(false);
            this.tabDetails.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.outputInformationBindingSource)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pathListBindingSource)).EndInit();
            this.tabDependencies.ResumeLayout(false);
            this.tabChildDeps.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.projectDependencySource)).EndInit();
            this.toolStrip.ResumeLayout(false);
            this.toolStrip.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.projectChildDepsSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox lstProjects;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.TextBox txtPath;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtSccPath;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtBuildOpt;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtFramework;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckedListBox lstDependencies;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabDetails;
        private System.Windows.Forms.TabPage tabDependencies;
        private System.Windows.Forms.TabPage tabChildDeps;
        private System.Windows.Forms.ToolStrip toolStrip;
        private System.Windows.Forms.ToolStripButton btnLoad;
        private System.Windows.Forms.ToolStripButton btnSave;
        private System.Windows.Forms.ListBox lstOutputs;
        private System.Windows.Forms.CheckedListBox lstDependants;
        private System.Windows.Forms.Button btnAddOutputFile;
        private System.Windows.Forms.Button btnDeleteOutputEntry;
        private System.Windows.Forms.Button btnAddOutputPath;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ToolStripButton btnNew;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newProjectFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem projectsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton btnAddProject;
        private System.Windows.Forms.ToolStripButton btnRemoveProject;
        private System.Windows.Forms.Button btnChooseProjectFile;
        private System.Windows.Forms.Button btnEditDirectPath;
        private System.Windows.Forms.Button btnAddDirectPath;
        private System.Windows.Forms.TextBox txtDirectOutputPath;
        private System.Windows.Forms.ToolStripStatusLabel tslblDuplicateEntries;
        private System.Windows.Forms.BindingSource projectsSource;
        private System.Windows.Forms.BindingSource projectDependencySource;
        private System.Windows.Forms.BindingSource projectChildDepsSource;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem importOneToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importManyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem recentToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem clearToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem buildToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewBuildOrderToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripTextBox txtFilter;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem detailsViewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem dependenciesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem childDependenciesToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem5;
        private System.Windows.Forms.ToolStripMenuItem previousProjectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem nextProjectToolStripMenuItem;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ToolStripMenuItem projectListToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem filterToolStripMenuItem;
        private System.Windows.Forms.TextBox txtConfigName;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox txtPlatform;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox cbBinPlaceDestPath;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btnDeleteBinPlaceOptions;
        private System.Windows.Forms.Button btnAddBinPlaceOpt;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton btnShowDuplicates;
        private System.Windows.Forms.ToolStripButton btnResetFilter;
        private System.Windows.Forms.Timer filterTimer;
        private System.Windows.Forms.BindingSource outputInformationBindingSource;
        private System.Windows.Forms.BindingSource pathListBindingSource;
        private System.Windows.Forms.TextBox txtDestPath;
        private System.Windows.Forms.BindingSource buildFileSource;
        private System.Windows.Forms.ToolStripStatusLabel lblModifying;
        private System.Windows.Forms.ToolStripStatusLabel lblSaving;
        private System.Windows.Forms.ToolStripStatusLabel lblSaved;
        private System.Windows.Forms.Timer savedLabelTimer;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripSplitButton buildProjectButton;
        private System.Windows.Forms.ToolStripMenuItem buildToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem buildWithoutDependenciesToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.ToolStripMenuItem binPlaceFileEditorToolStripMenuItem;
        private System.Windows.Forms.Button btnChooseConfig;
    }
}