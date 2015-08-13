namespace UWOLMaps
{
    partial class LevelSelector
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LevelSelector));
            this.layoutPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.addToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.removeToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.moveUpButton = new System.Windows.Forms.ToolStripButton();
            this.moveDownButton = new System.Windows.Forms.ToolStripButton();
            this.viewer = new UWOLMaps.LevelViewer();
            this.levelsBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.levelsBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // layoutPanel
            // 
            this.layoutPanel.AutoScroll = true;
            this.layoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.layoutPanel.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.layoutPanel.Location = new System.Drawing.Point(0, 25);
            this.layoutPanel.Name = "layoutPanel";
            this.layoutPanel.Padding = new System.Windows.Forms.Padding(1);
            this.layoutPanel.Size = new System.Drawing.Size(191, 353);
            this.layoutPanel.TabIndex = 0;
            this.layoutPanel.WrapContents = false;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripButton,
            this.removeToolStripButton,
            this.toolStripSeparator1,
            this.moveUpButton,
            this.moveDownButton});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(191, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // addToolStripButton
            // 
            this.addToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.addToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("addToolStripButton.Image")));
            this.addToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addToolStripButton.Name = "addToolStripButton";
            this.addToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.addToolStripButton.Text = "Añadir Nivel";
            this.addToolStripButton.Click += new System.EventHandler(this.addToolStripButton_Click);
            // 
            // removeToolStripButton
            // 
            this.removeToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.removeToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("removeToolStripButton.Image")));
            this.removeToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.removeToolStripButton.Name = "removeToolStripButton";
            this.removeToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.removeToolStripButton.Text = "Borrar Nivel";
            this.removeToolStripButton.Click += new System.EventHandler(this.removeToolStripButton_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // moveUpButton
            // 
            this.moveUpButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.moveUpButton.Image = ((System.Drawing.Image)(resources.GetObject("moveUpButton.Image")));
            this.moveUpButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.moveUpButton.Name = "moveUpButton";
            this.moveUpButton.Size = new System.Drawing.Size(23, 22);
            this.moveUpButton.Text = "moveUpButton";
            this.moveUpButton.Click += new System.EventHandler(this.moveUpButton_Click);
            // 
            // moveDownButton
            // 
            this.moveDownButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.moveDownButton.Image = ((System.Drawing.Image)(resources.GetObject("moveDownButton.Image")));
            this.moveDownButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.moveDownButton.Name = "moveDownButton";
            this.moveDownButton.Size = new System.Drawing.Size(23, 22);
            this.moveDownButton.Text = "moveDownButton";
            this.moveDownButton.Click += new System.EventHandler(this.moveDownButton_Click);
            // 
            // viewer
            // 
            this.viewer.GridColor = System.Drawing.Color.Empty;
            this.viewer.ImageList = null;
            this.viewer.Location = new System.Drawing.Point(0, 0);
            this.viewer.Name = "viewer";
            this.viewer.ShowArrows = true;
            this.viewer.ShowGrid = false;
            this.viewer.Size = new System.Drawing.Size(191, 100);
            this.viewer.TabIndex = 1;
            this.viewer.TileMap = null;
            this.viewer.Version = null;
            this.viewer.Visible = false;
            this.viewer.ZoomFactor = 2;
            // 
            // levelsBindingSource
            // 
            this.levelsBindingSource.DataSource = typeof(UWOLMaps.Levels);
            // 
            // LevelSelector
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.layoutPanel);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.viewer);
            this.Name = "LevelSelector";
            this.Size = new System.Drawing.Size(191, 378);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.levelsBindingSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.FlowLayoutPanel layoutPanel;
        private System.Windows.Forms.BindingSource levelsBindingSource;
        private LevelViewer viewer;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton addToolStripButton;
        private System.Windows.Forms.ToolStripButton removeToolStripButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton moveUpButton;
        private System.Windows.Forms.ToolStripButton moveDownButton;
    }
}
