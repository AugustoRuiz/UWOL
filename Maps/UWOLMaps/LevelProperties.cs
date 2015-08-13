using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace UWOLMaps
{
    public partial class LevelProperties : UserControl
    {
        public event EventHandler ObjectChanged;

        public LevelProperties()
        {
            InitializeComponent();
            loadCombos();

            if (!this.DesignMode)
            {
                cboInk.MeasureItem += new MeasureItemEventHandler(cboMeasureItem);
                cboPaper.MeasureItem += new MeasureItemEventHandler(cboMeasureItem);

                cboInk.DrawItem += new DrawItemEventHandler(cboDrawItem);
                cboPaper.DrawItem += new DrawItemEventHandler(cboDrawItem);

                cboTileFondo.MeasureItem += new MeasureItemEventHandler(cboTileFondo_MeasureItem);
                cboTileFondo.DrawItem += new DrawItemEventHandler(cboTileFondo_DrawItem);
            }
        }

        private Level level;

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden), Browsable(false)]
        public Level Level
        {
            get { return level; }
            set
            {
                level = value;
                UpdateBinding();
            }
        }

        private Dictionary<string, Image> imageList;

        public Dictionary<string, Image> ImageList
        {
            get { return imageList; }
            set { imageList = value; }
        }

        private Version version;

        public Version Version
        {
            get { return version; }
            set { version = value; }
        }

        public object SelectedObject
        {
            get { return properties.SelectedObject; }
            set
            {
                lblTipo.Text = "";
                properties.SelectedObject = value;

                if (value is Moneda) lblTipo.Text = "Moneda";
                if (value is Enemigo) lblTipo.Text = "Enemigo";
                if (value is Plataforma) lblTipo.Text = "Plataforma";
            }
        }


        void cboDrawItem(object sender, DrawItemEventArgs e)
        {
            ColorZX col;
            SolidBrush br;
            SolidBrush textBr;
            ComboBox cbo = sender as ComboBox;

            e.DrawBackground();

            if (e.Index != -1)
            {
                col = (ColorZX)((ComboBox)sender).Items[e.Index];
                br = new SolidBrush(Utils.ColorZXToColor(col));

                if ((e.State & (DrawItemState.Selected | DrawItemState.HotLight)) != 0)
                {
                    textBr = new SolidBrush(Color.FromKnownColor(KnownColor.HighlightText));
                }
                else
                {
                    textBr = new SolidBrush(Color.FromKnownColor(KnownColor.WindowText));
                }

                e.Graphics.FillRectangle(br, e.Bounds.X + 1, e.Bounds.Y + 1, cbo.ItemHeight - 1, cbo.ItemHeight - 1);
                e.Graphics.DrawRectangle(Pens.Black, e.Bounds.X, e.Bounds.Y, cbo.ItemHeight, cbo.ItemHeight);
                e.Graphics.DrawString(col.ToString(), cbo.Font, textBr, cbo.ItemHeight + 3, e.Bounds.Y + 1);

                br.Dispose();
                textBr.Dispose();
            }
        }

        void cboMeasureItem(object sender, MeasureItemEventArgs e)
        {
            ComboBox cbo = sender as ComboBox;

            if (e.Index != -1)
            {
                e.ItemHeight = cbo.ItemHeight;
                e.ItemWidth = cbo.ItemHeight + 6 +
                     (int)e.Graphics.MeasureString(((ColorZX)cbo.Items[e.Index]).ToString(), cbo.Font).Width;
            }
        }

        void cboTileFondo_DrawItem(object sender, DrawItemEventArgs e)
        {
            ComboBox cbo = sender as ComboBox;

            e.DrawBackground();

            if (!this.DesignMode)
            {
                if (e.Index != -1)
                {
                    TilesFondo tile = (TilesFondo)cbo.Items[e.Index];

                    if (this.imageList != null)
                    {
                        e.Graphics.SmoothingMode = SmoothingMode.None;
                        e.Graphics.InterpolationMode = InterpolationMode.NearestNeighbor;
                        e.Graphics.DrawImage(this.imageList[this.version.CPU.ToString() + tile.ToString()], e.Bounds.X + 2, e.Bounds.Y + 2, 32, 32);
                        e.Graphics.DrawRectangle(Pens.Black, e.Bounds.X + 1, e.Bounds.Y + 1, 32, 32);
                    }
                    else
                    {
                        SolidBrush textBr;


                        if ((e.State & (DrawItemState.Selected | DrawItemState.HotLight)) != 0)
                        {
                            textBr = new SolidBrush(Color.FromKnownColor(KnownColor.HighlightText));
                        }
                        else
                        {
                            textBr = new SolidBrush(Color.FromKnownColor(KnownColor.WindowText));
                        }
                        e.Graphics.DrawString(tile.ToString(), cbo.Font, textBr, e.Bounds.X + 1, e.Bounds.Y + 1);
                        textBr.Dispose();
                    }
                }
            }
        }

        void cboTileFondo_MeasureItem(object sender, MeasureItemEventArgs e)
        {
            e.ItemHeight = 34;
            e.ItemWidth = 34;
        }

        private void loadCombos()
        {
            // Load color combos.
            foreach (ColorZX col in System.Enum.GetValues(typeof(ColorZX)))
            {
                cboInk.Items.Add(col);
                cboPaper.Items.Add(col);
            }
            // Load tile combos.
            foreach (TilesFondo tile in System.Enum.GetValues(typeof(TilesFondo)))
            {
                cboTileFondo.Items.Add(tile);
            }
        }

        private void UpdateBinding()
        {
            this.levelBindingSource.DataSource = this.level;
        }

        private void cboTileFondo_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.level.TileFondo = (TilesFondo)cboTileFondo.Items[cboTileFondo.SelectedIndex];
        }

        private void cboPaper_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.level.PaperColor = (ColorZX)cboPaper.Items[cboPaper.SelectedIndex];
        }

        private void cboInk_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.level.InkColor = (ColorZX)cboInk.Items[cboInk.SelectedIndex];
        }

        private void numPantalla_ValueChanged(object sender, EventArgs e)
        {
            this.level.NumPantalla = (byte)numPantalla.Value;
        }

        private void properties_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            this.OnObjectChanged(EventArgs.Empty);
        }

        private void OnObjectChanged(EventArgs e)
        {
            if (this.ObjectChanged != null)
            {
                this.ObjectChanged(this, e);
            }
        }
    }
}
