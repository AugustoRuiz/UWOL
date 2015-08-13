using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace UWOLMaps
{
    public partial class CodeView : Form
    {
        public CodeView()
        {
            InitializeComponent();
            loadVersionCombo();
        }

        private Levels levels;

        public Levels Levels
        {
            get { return levels; }
            set
            {
                levels = value;
                if (levels != null)
                {
                    levels.ListChanged += new ListChangedEventHandler(levels_ListChanged);
                }
            }
        }

        void levels_ListChanged(object sender, ListChangedEventArgs e)
        {
            updateView();
        }

        private Version version;

        public Version Version
        {
            set
            {
                switch (value.CPU)
                {
                    case CPUVersion.ZX:
                        this.Text = "Vista del código - Spectrum ZX";
                        break;
                    case CPUVersion.MSX:
                        this.Text = "Vista del código - MSX";
                        break;
                    case CPUVersion.CPC:
                        this.Text = "Vista del código - CPC";
                        break;
                    case CPUVersion.PC:
                        this.Text = "Vista del código - PC";
                        break;
                }
                version = value;
                versionCbo.SelectedIndex = versionCbo.FindStringExact(version.CPU.ToString());
                updateView();
            }
        }

        private void updateView()
        {
            if (this.levels != null)
            {
                if (this.version.CPU != CPUVersion.MSX)
                {
                    MemoryStream stream = new MemoryStream();

                    CodeGenerators.ICodeGenerator generator;

                    generator = CodeGenerators.CodeGeneratorFactory.CreateGenerator(this.version);
                    generator.CreateCode(levels, stream);

                    StreamReader codigo = new StreamReader(stream);
                    stream.Position = 0;

                    this.rtbCode.Text = codigo.ReadToEnd();

                    codigo.Close();
                    stream.Dispose();
                }
                else
                {
                    StringBuilder sb = new StringBuilder();

                    foreach (Level lvl in this.levels)
                    {
                        sb.AppendLine(lvl.ToString(this.version));
                    }
                    this.rtbCode.Text = sb.ToString();
                }
            }
        }

        private void loadVersionCombo()
        {
            versionCbo.Items.Clear();

            foreach (CPUVersion value in System.Enum.GetValues(typeof(CPUVersion)))
            {
                versionCbo.Items.Add(value);
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // TODO...
        }

        private void versionCbo_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.Version = Version.DameVersion((CPUVersion)versionCbo.SelectedItem);
        }

    }

}