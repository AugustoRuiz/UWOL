namespace UWOLMaps
{
    partial class LevelProperties
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
           this.cboInk = new System.Windows.Forms.ComboBox();
           this.cboPaper = new System.Windows.Forms.ComboBox();
           this.label1 = new System.Windows.Forms.Label();
           this.label2 = new System.Windows.Forms.Label();
           this.cboTileFondo = new System.Windows.Forms.ComboBox();
           this.label3 = new System.Windows.Forms.Label();
           this.numPantalla = new System.Windows.Forms.NumericUpDown();
           this.label4 = new System.Windows.Forms.Label();
           this.properties = new System.Windows.Forms.PropertyGrid();
           this.lblTipo = new System.Windows.Forms.Label();
           this.levelBindingSource = new System.Windows.Forms.BindingSource(this.components);
           ((System.ComponentModel.ISupportInitialize)(this.numPantalla)).BeginInit();
           ((System.ComponentModel.ISupportInitialize)(this.levelBindingSource)).BeginInit();
           this.SuspendLayout();
           // 
           // cboInk
           // 
           this.cboInk.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                       | System.Windows.Forms.AnchorStyles.Right)));
           this.cboInk.DataBindings.Add(new System.Windows.Forms.Binding("SelectedItem", this.levelBindingSource, "InkColor", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
           this.cboInk.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
           this.cboInk.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
           this.cboInk.FormattingEnabled = true;
           this.cboInk.Location = new System.Drawing.Point(84, 26);
           this.cboInk.Name = "cboInk";
           this.cboInk.Size = new System.Drawing.Size(226, 21);
           this.cboInk.TabIndex = 0;
           this.cboInk.SelectedIndexChanged += new System.EventHandler(this.cboInk_SelectedIndexChanged);
           // 
           // cboPaper
           // 
           this.cboPaper.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                       | System.Windows.Forms.AnchorStyles.Right)));
           this.cboPaper.DataBindings.Add(new System.Windows.Forms.Binding("SelectedItem", this.levelBindingSource, "PaperColor", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
           this.cboPaper.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
           this.cboPaper.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
           this.cboPaper.FormattingEnabled = true;
           this.cboPaper.Location = new System.Drawing.Point(84, 53);
           this.cboPaper.Name = "cboPaper";
           this.cboPaper.Size = new System.Drawing.Size(226, 21);
           this.cboPaper.TabIndex = 1;
           this.cboPaper.SelectedIndexChanged += new System.EventHandler(this.cboPaper_SelectedIndexChanged);
           // 
           // label1
           // 
           this.label1.AutoSize = true;
           this.label1.Location = new System.Drawing.Point(3, 29);
           this.label1.Name = "label1";
           this.label1.Size = new System.Drawing.Size(25, 13);
           this.label1.TabIndex = 2;
           this.label1.Text = "Ink:";
           // 
           // label2
           // 
           this.label2.AutoSize = true;
           this.label2.Location = new System.Drawing.Point(3, 56);
           this.label2.Name = "label2";
           this.label2.Size = new System.Drawing.Size(38, 13);
           this.label2.TabIndex = 3;
           this.label2.Text = "Paper:";
           // 
           // cboTileFondo
           // 
           this.cboTileFondo.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                       | System.Windows.Forms.AnchorStyles.Right)));
           this.cboTileFondo.DataBindings.Add(new System.Windows.Forms.Binding("SelectedItem", this.levelBindingSource, "TileFondo", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
           this.cboTileFondo.DisplayMember = "TileFondo";
           this.cboTileFondo.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
           this.cboTileFondo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
           this.cboTileFondo.FormattingEnabled = true;
           this.cboTileFondo.ItemHeight = 34;
           this.cboTileFondo.Location = new System.Drawing.Point(84, 80);
           this.cboTileFondo.Name = "cboTileFondo";
           this.cboTileFondo.Size = new System.Drawing.Size(226, 40);
           this.cboTileFondo.TabIndex = 4;
           this.cboTileFondo.ValueMember = "TileFondo";
           this.cboTileFondo.SelectedIndexChanged += new System.EventHandler(this.cboTileFondo_SelectedIndexChanged);
           // 
           // label3
           // 
           this.label3.AutoSize = true;
           this.label3.Location = new System.Drawing.Point(3, 83);
           this.label3.Name = "label3";
           this.label3.Size = new System.Drawing.Size(75, 13);
           this.label3.TabIndex = 5;
           this.label3.Text = "Tile de Fondo:";
           // 
           // numPantalla
           // 
           this.numPantalla.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                       | System.Windows.Forms.AnchorStyles.Right)));
           this.numPantalla.DataBindings.Add(new System.Windows.Forms.Binding("Value", this.levelBindingSource, "NumPantalla", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
           this.numPantalla.Location = new System.Drawing.Point(84, 0);
           this.numPantalla.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
           this.numPantalla.Name = "numPantalla";
           this.numPantalla.Size = new System.Drawing.Size(226, 20);
           this.numPantalla.TabIndex = 6;
           this.numPantalla.ValueChanged += new System.EventHandler(this.numPantalla_ValueChanged);
           // 
           // label4
           // 
           this.label4.AutoSize = true;
           this.label4.Location = new System.Drawing.Point(3, 2);
           this.label4.Name = "label4";
           this.label4.Size = new System.Drawing.Size(76, 13);
           this.label4.TabIndex = 7;
           this.label4.Text = "Num. Pantalla:";
           // 
           // properties
           // 
           this.properties.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                       | System.Windows.Forms.AnchorStyles.Left)
                       | System.Windows.Forms.AnchorStyles.Right)));
           this.properties.CommandsVisibleIfAvailable = false;
           this.properties.Location = new System.Drawing.Point(0, 139);
           this.properties.Name = "properties";
           this.properties.Size = new System.Drawing.Size(310, 208);
           this.properties.TabIndex = 8;
           this.properties.ToolbarVisible = false;
           this.properties.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.properties_PropertyValueChanged);
           // 
           // lblTipo
           // 
           this.lblTipo.AutoSize = true;
           this.lblTipo.Location = new System.Drawing.Point(3, 123);
           this.lblTipo.Name = "lblTipo";
           this.lblTipo.Size = new System.Drawing.Size(0, 13);
           this.lblTipo.TabIndex = 9;
           // 
           // levelBindingSource
           // 
           this.levelBindingSource.DataSource = typeof(UWOLMaps.Level);
           // 
           // LevelProperties
           // 
           this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
           this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
           this.Controls.Add(this.lblTipo);
           this.Controls.Add(this.properties);
           this.Controls.Add(this.label4);
           this.Controls.Add(this.numPantalla);
           this.Controls.Add(this.label3);
           this.Controls.Add(this.cboTileFondo);
           this.Controls.Add(this.label2);
           this.Controls.Add(this.label1);
           this.Controls.Add(this.cboPaper);
           this.Controls.Add(this.cboInk);
           this.Name = "LevelProperties";
           this.Size = new System.Drawing.Size(310, 347);
           ((System.ComponentModel.ISupportInitialize)(this.numPantalla)).EndInit();
           ((System.ComponentModel.ISupportInitialize)(this.levelBindingSource)).EndInit();
           this.ResumeLayout(false);
           this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.BindingSource levelBindingSource;
        private System.Windows.Forms.ComboBox cboInk;
        private System.Windows.Forms.ComboBox cboPaper;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cboTileFondo;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numPantalla;
        private System.Windows.Forms.Label label4;
       private System.Windows.Forms.PropertyGrid properties;
       private System.Windows.Forms.Label lblTipo;
    }
}
