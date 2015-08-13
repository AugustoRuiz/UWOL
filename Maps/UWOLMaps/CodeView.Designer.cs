namespace UWOLMaps
{
   partial class CodeView
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
          System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CodeView));
          this.rtbCode = new System.Windows.Forms.RichTextBox();
          this.menuStrip1 = new System.Windows.Forms.MenuStrip();
          this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
          this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
          this.versionCbo = new System.Windows.Forms.ToolStripComboBox();
          this.menuStrip1.SuspendLayout();
          this.SuspendLayout();
          // 
          // rtbCode
          // 
          this.rtbCode.Dock = System.Windows.Forms.DockStyle.Fill;
          this.rtbCode.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
          this.rtbCode.Location = new System.Drawing.Point(0, 25);
          this.rtbCode.Name = "rtbCode";
          this.rtbCode.Size = new System.Drawing.Size(623, 447);
          this.rtbCode.TabIndex = 0;
          this.rtbCode.Text = "";
          // 
          // menuStrip1
          // 
          this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.versionCbo});
          this.menuStrip1.Location = new System.Drawing.Point(0, 0);
          this.menuStrip1.Name = "menuStrip1";
          this.menuStrip1.Size = new System.Drawing.Size(623, 25);
          this.menuStrip1.TabIndex = 1;
          this.menuStrip1.Text = "menuStrip1";
          // 
          // fileToolStripMenuItem
          // 
          this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveToolStripMenuItem});
          this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
          this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 21);
          this.fileToolStripMenuItem.Text = "&File";
          // 
          // saveToolStripMenuItem
          // 
          this.saveToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("saveToolStripMenuItem.Image")));
          this.saveToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
          this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
          this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
          this.saveToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
          this.saveToolStripMenuItem.Text = "&Save";
          this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
          // 
          // versionCbo
          // 
          this.versionCbo.Name = "versionCbo";
          this.versionCbo.Size = new System.Drawing.Size(121, 21);
          this.versionCbo.SelectedIndexChanged += new System.EventHandler(this.versionCbo_SelectedIndexChanged);
          // 
          // CodeView
          // 
          this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
          this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
          this.ClientSize = new System.Drawing.Size(623, 472);
          this.Controls.Add(this.rtbCode);
          this.Controls.Add(this.menuStrip1);
          this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
          this.MainMenuStrip = this.menuStrip1;
          this.Name = "CodeView";
          this.Text = "Vista del código";
          this.menuStrip1.ResumeLayout(false);
          this.menuStrip1.PerformLayout();
          this.ResumeLayout(false);
          this.PerformLayout();

      }

      #endregion

      private System.Windows.Forms.RichTextBox rtbCode;
       private System.Windows.Forms.MenuStrip menuStrip1;
       private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
       private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
       private System.Windows.Forms.ToolStripComboBox versionCbo;
   }
}