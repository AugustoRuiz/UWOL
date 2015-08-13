using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace UWOLMaps
{
    public class PyramidViewer : Panel
    {
        private Levels _levels;
        private Dictionary<string, Image> _imageList;
        private ColorZX lastInk, lastPaper;
        private Version version;
        private LevelViewer viewer;

        public Version Version
        {
            get { return version; }
            set
            {
                version = value;
                this.viewer.Version = version;
                this.drawLevels();
            }
        }

        public Levels Levels
        {
            get { return _levels; }
            set
            {
                if (!Levels.Equals(_levels, value))
                {
                    _levels = value;
                    drawLevels();
                }
            }
        }

        public PyramidViewer()
        {
            this.DoubleBuffered = true;
            this.AutoScroll = true;
            this.viewer = new LevelViewer();
            _imageList = Utils.getTiles();
        }

        private void drawLevels()
        {
            if (_levels != null && version != null)
            {
                this.Controls.Clear();
                int levelIdx = 1;
                int tileWidth, tileHeight;

                tileWidth = 100;
                tileHeight = 100;
                viewer.Size = new Size(384, 320);

                foreach (Level l in _levels)
                {
                    PictureBox pb = addLevelToPanel(l, tileWidth, tileHeight);
                    int row, col;
                    getRowCol(levelIdx, out row, out col);
                    
                    pb.Top = tileHeight * row;
                    pb.Left = (int)((tileWidth * 5.0f) - (((row/2.0f) - col + 1) * tileWidth));
                    this.Controls.Add(pb);
                    levelIdx++;
                }
            }
        }

        private PictureBox addLevelToPanel(Level level, int cellWidth, int cellHeight)
        {
            PictureBox levelCell;

            levelCell = new PictureBox();

            if (version.CPU == CPUVersion.ZX)
            {
                updateTiles(level);
            }
            
            viewer.ImageList = _imageList;
            levelCell.Size = new Size(cellWidth - 1, cellWidth - 1);
            levelCell.SizeMode = PictureBoxSizeMode.StretchImage;
            viewer.Level = level;
            levelCell.Image = viewer.GetPreview();
            levelCell.Visible = true;

            return levelCell;
        }

        private void updateTiles(Level currentLevel)
        {
            if (lastInk != currentLevel.InkColor || lastPaper != currentLevel.PaperColor)
            {
                Utils.updateZXTiles(_imageList, currentLevel);
            }
            lastInk = currentLevel.InkColor;
            lastPaper = currentLevel.PaperColor;
        }

        private void getRowCol(int levelNumber, out int row, out int col)
        {
            row = 0;

            for (int i = 1; i <= 10; i++)
            {
                if (levelNumber <= ((i + 1) * i) / 2)
                    break;
                row++;
            }

            col = levelNumber - ((row + 1) * row) / 2 - 1;
        }
    }
}
