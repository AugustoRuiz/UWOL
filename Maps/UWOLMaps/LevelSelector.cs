using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace UWOLMaps
{
    public partial class LevelSelector : UserControl
    {
        public LevelSelector()
        {
            InitializeComponent();
            this.DoubleBuffered = true;
            imageList = Utils.getTiles();
        }

        private Levels levels;
        private Dictionary<string, Image> imageList;
        private Version version;
        private PictureBox selectedPBox;
        private Level selectedLevel;
        public event EventHandler SelectedLevelChanged;
        
        private ColorZX lastInk, lastPaper;

        public Levels Levels
        {
            get { return levels; }
            set
            {
                if (!Levels.Equals(levels, value))
                {
                    levels = value;
                    drawLevels();
                    selectFirstLevel();
                    enableToolStripButtons();
                }
            }
        }

        public Version Version
        {
            get { return version; }
            set
            {
                 if (version == null || (!Version.Equals(version.CPU, value.CPU)))
                 {
                     version = value;
                     this.viewer.Version = value;
                     drawLevels();
                 }
            }
        }

        public Level SelectedLevel
        {
            get { return selectedLevel; }
            set
            {
                if (!Level.Equals(selectedLevel, value))
                {
                    selectedLevel = value;
                    OnSelectedLevelChanged(EventArgs.Empty);
                }
            }
        }

        private void drawLevels()
        {
            if (levels != null)
            {
                viewer.Size = new Size(384, 320);

                this.layoutPanel.SuspendLayout();
                this.layoutPanel.Controls.Clear();

                foreach (Level level in levels)
                {
                    addLevelToPanel(level);
                }
                
                this.layoutPanel.ResumeLayout(true);

                enableToolStripButtons();
            }
            this.layoutPanel.Invalidate();
        }

        private PictureBox addLevelToPanel(Level level)
        {
            PictureBox levelCell;

            levelCell = new PictureBox();
            levelCell.Click += new EventHandler(levelCell_Click);
            levelCell.Tag = level;

            if (version.CPU == CPUVersion.ZX)
            {
                updateTiles(level);
            }
            
            viewer.ImageList = imageList;

            if (level.Equals(selectedLevel))
            {
                levelCell.BorderStyle = BorderStyle.Fixed3D;
            }
            else
            {
                levelCell.BorderStyle = BorderStyle.None;
            }
            int cellWidth = this.ClientRectangle.Width - layoutPanel.Padding.Left - layoutPanel.Padding.Right - 4 - SystemInformation.VerticalScrollBarWidth;
            levelCell.Size = new Size(cellWidth, cellWidth);
            levelCell.SizeMode = PictureBoxSizeMode.StretchImage;
            viewer.Level = level;
            levelCell.Image = viewer.GetPreview();
            levelCell.Visible = true;
            layoutPanel.Controls.Add(levelCell);

            return levelCell;
        }

        void levelCell_Click(object sender, EventArgs e)
        {
            selectedPBox = sender as PictureBox;
            
            removeBorders();
            selectedPBox.BorderStyle = BorderStyle.Fixed3D;

            Level clickedLevel = selectedPBox.Tag as Level;

            if (clickedLevel != selectedLevel)
            {
                selectedLevel = clickedLevel;
                OnSelectedLevelChanged(EventArgs.Empty);
                enableToolStripButtons();
            }
        }

        private void removeBorders()
        {
            foreach (Control c in layoutPanel.Controls)
            {
                PictureBox pBox = c as PictureBox;
                if (pBox != null)
                {
                    pBox.BorderStyle = BorderStyle.None;
                }
            }
        }

        protected virtual void OnSelectedLevelChanged(EventArgs e)
        {
            if (this.SelectedLevelChanged != null)
            {
                this.SelectedLevelChanged(this, e);
            }
        }

        internal void UpdateCurrentLevel()
        {
            if (selectedLevel != null && selectedPBox != null)
            {
                if (version.CPU == CPUVersion.ZX)
                {
                    updateTiles(selectedLevel);
                }

                viewer.ImageList = imageList;

                viewer.Level = selectedLevel;
                selectedPBox.Image = viewer.GetPreview();
                selectedPBox.Invalidate();
            }
        }

        internal void RedrawAll()
        {
            this.drawLevels();
        }

        private void addToolStripButton_Click(object sender, EventArgs e)
        {
            Level newLevel = new Level();
            newLevel.NumPantalla = (byte)this.levels.Count;
            newLevel.InkColor = ColorZX.Blanco;
            newLevel.PaperColor = ColorZX.Negro;

            this.levels.Add(newLevel);
            removeBorders();

            selectedPBox = this.addLevelToPanel(newLevel);
            this.SelectedLevel = newLevel;

            UpdateCurrentLevel();
            enableToolStripButtons();
        }

        private void removeToolStripButton_Click(object sender, EventArgs e)
        {
            if (this.selectedLevel != null)
            {
                this.levels.Remove(selectedLevel);
                removeLevelFromPanel(selectedLevel);
                if (this.levels.Count > 0)
                {
                    this.SelectedLevel = this.levels[0];
                }
                UpdateCurrentLevel();
            }
            enableToolStripButtons();
        }

        private void removeLevelFromPanel(Level selectedLevel)
        {
            PictureBox pBoxToRemove = null;
            foreach (PictureBox pBox in layoutPanel.Controls)
            {
                if (selectedLevel.Equals(pBox.Tag))
                {
                    pBoxToRemove = pBox;
                    break;
                }
            }
            if (pBoxToRemove != null)
            {
                layoutPanel.Controls.Remove(pBoxToRemove);
            }
        }

        private void selectFirstLevel()
        {
            removeBorders();
            if (levels != null && levels.Count > 0)
            {
                selectedLevel = levels[0];
                selectedPBox = layoutPanel.Controls[0] as PictureBox;
                selectedPBox.BorderStyle = BorderStyle.Fixed3D;
            }
        }

        private void enableToolStripButtons()
        {
            this.removeToolStripButton.Enabled = ((this.levels != null) && (this.levels.Count > 1));
            this.addToolStripButton.Enabled = ((this.levels != null) && (this.levels.Count < byte.MaxValue));
            this.moveDownButton.Enabled = ((this.levels != null) && (this.levels.IndexOf(this.selectedLevel) < (this.levels.Count - 1)));
            this.moveUpButton.Enabled = ((this.levels != null) && (this.levels.IndexOf(this.selectedLevel) > 0));
        }

        private void moveUpButton_Click(object sender, EventArgs e)
        {
            int index = this.levels.IndexOf(this.selectedLevel) - 1;
            this.levels.Remove(this.selectedLevel);
            this.levels.Insert(index, this.selectedLevel);
            drawLevels();
        }

        private void moveDownButton_Click(object sender, EventArgs e)
        {
            int index = this.levels.IndexOf(this.selectedLevel) + 1;
            this.levels.Remove(this.selectedLevel);
            this.levels.Insert(index, this.selectedLevel);
            drawLevels();
        }

        private void updateTiles(Level currentLevel)
        {
            if (lastInk != currentLevel.InkColor || lastPaper != currentLevel.PaperColor)
            {
                Utils.updateZXTiles(imageList, currentLevel);
            }
            lastInk = currentLevel.InkColor;
            lastPaper = currentLevel.PaperColor;
        }
    }
}
