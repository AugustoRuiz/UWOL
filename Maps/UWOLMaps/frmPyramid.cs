using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace UWOLMaps
{
    public partial class frmPyramid : Form
    {
        public frmPyramid()
        {
            InitializeComponent();
        }

        public Levels Levels
        {
            get { return this.pyramidViewer1.Levels; }
            set { this.pyramidViewer1.Levels = value; }
        }

        public Version Version
        {
            get { return this.pyramidViewer1.Version; }
            set { this.pyramidViewer1.Version = value; }
        }
    }
}