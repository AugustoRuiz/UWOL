namespace UWOLMaps
{
    partial class frmPyramid
    {
        /// <summary>
        /// Variable del diseñador requerida.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén utilizando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben eliminar; false en caso contrario, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido del método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.pyramidViewer1 = new UWOLMaps.PyramidViewer();
            this.SuspendLayout();
            // 
            // pyramidViewer1
            // 
            this.pyramidViewer1.AutoScroll = true;
            this.pyramidViewer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pyramidViewer1.Levels = null;
            this.pyramidViewer1.Location = new System.Drawing.Point(0, 0);
            this.pyramidViewer1.Name = "pyramidViewer1";
            this.pyramidViewer1.Size = new System.Drawing.Size(738, 473);
            this.pyramidViewer1.TabIndex = 0;
            this.pyramidViewer1.Version = null;
            // 
            // frmPyramid
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(738, 473);
            this.Controls.Add(this.pyramidViewer1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "frmPyramid";
            this.Text = "frmPyramid";
            this.ResumeLayout(false);

        }

        #endregion

        private PyramidViewer pyramidViewer1;
    }
}