using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.ComponentModel;

namespace UWOLMaps
{
   public class LevelViewer : Panel
   {

      public LevelViewer()
      {
         this.DoubleBuffered = true;
      }

      private int zoomFactor = 2;

      public int ZoomFactor
      {
         get { return zoomFactor; }
         set { zoomFactor = value; }
      }

      private Image tileMap;

      public Image TileMap
      {
         get { return tileMap; }
         set { tileMap = value; }
      }

      private Level level;

      [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden), Browsable(false)]
      public Level Level
      {
         get { return level; }
         set { level = value; }
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

      private Color gridColor;

      public Color GridColor
      {
         get { return gridColor; }
         set
         {
            gridColor = value;
            this.Invalidate();
         }
      }

      private bool showGrid;

      public bool ShowGrid
      {
         get { return showGrid; }
         set { showGrid = value; }
      }

      private bool showArrows;

      public bool ShowArrows
      {
         get { return showArrows; }
         set { showArrows = value; }
      }


      protected override void OnPaintBackground(PaintEventArgs e)
      {
         // base.OnPaintBackground(e);
         e.Graphics.FillRectangle(Brushes.Black, this.ClientRectangle);
      }

      protected override void OnPaint(PaintEventArgs e)
      {
         drawLevel(e.Graphics, true);
      }

      private void drawLevel(Graphics graphics, bool grid)
      {
         graphics.InterpolationMode = InterpolationMode.NearestNeighbor;

         if (level != null && imageList != null)
         {
            TilesFondo tilFondo = level.TileFondo;

            int tilWidth = 32;
            int tilHeight = 32;
            Rectangle srcRect = new Rectangle(0, 0, 16, 16);
            Rectangle destRect = new Rectangle(0, 0, 33, 33);

            for (int iFila = 0; iFila < 10; iFila++)
            {
               for (int iCol = 0; iCol < 12; iCol++)
               {
                  graphics.DrawImage(this.imageList[version.CPU.ToString() + tilFondo.ToString()], destRect, srcRect, GraphicsUnit.Pixel);
                  destRect.X += tilWidth;
               }
               destRect.X = 0;
               destRect.Y += tilHeight;
            }

            drawPlatforms(graphics, tilWidth, tilHeight, ref srcRect, ref destRect);
            drawCoins(graphics, tilWidth, tilHeight, ref srcRect, ref destRect);
            drawEnemies(graphics, tilWidth, tilHeight, ref srcRect, ref destRect);

            if (grid && this.showGrid)
            {
               drawGrid(graphics, tilWidth, tilHeight);
            }
         }
      }

      private void drawGrid(Graphics graphics, int tilWidth, int tilHeight)
      {
         int x, y;
         SolidBrush br = new SolidBrush(this.gridColor);
         Pen currentPen = new Pen(br, 1);
         currentPen.DashStyle = DashStyle.Dash;

         y = 0;
         for (int iFila = 0; iFila < 11; iFila++)
         {
            graphics.DrawLine(currentPen, 0, y, this.Width, y);
            y += tilHeight;
         }
         x = 0;
         for (int iCol = 0; iCol < 12; iCol++)
         {
            graphics.DrawLine(currentPen, x, 0, x, this.Height);
            x += tilWidth;
         }
      }

      private void drawPlatforms(Graphics graphics, int tilWidth, int tilHeight, ref Rectangle srcRect, ref Rectangle destRect)
      {
         Rectangle shadowRect = new Rectangle(0, 0, 33, 33);

         foreach (Plataforma obj in this.level.Plataformas)
         {
            if (obj != null)
            {
               destRect = new Rectangle(obj.X * tilWidth, obj.Y * tilHeight, 33, 33);

               if (obj.X == 0)
               {
                  shadowRect.X = 0;
                  shadowRect.Y = destRect.Y + 16;
                  graphics.DrawImage(this.imageList[version.CPU.ToString() + "s" + this.level.TileFondo.ToString()], shadowRect, srcRect, GraphicsUnit.Pixel);
               }

               for (int iPos = 0; iPos < obj.Longitud; iPos++)
               {
                  shadowRect.X = destRect.X + 16;
                  shadowRect.Y = destRect.Y + 16;

                  graphics.DrawImage(this.imageList[version.CPU.ToString() + "s" + this.level.TileFondo.ToString()], shadowRect, srcRect, GraphicsUnit.Pixel);
                  graphics.DrawImage(this.imageList[version.CPU.ToString() + obj.TipoPlataforma.ToString()], destRect, srcRect, GraphicsUnit.Pixel);

                  if (obj.Direccion == Direccion.Horizontal)
                  {
                     destRect.X += tilWidth;
                  }
                  else
                  {
                     destRect.Y += tilHeight;
                  }
               }
            }
         }
      }

      private void drawCoins(Graphics graphics, int tilWidth, int tilHeight, ref Rectangle srcRect, ref Rectangle destRect)
      {
         foreach (Moneda moneda in this.level.Monedas)
         {
            if (moneda != null)
            {
               destRect = new Rectangle(moneda.X * tilWidth, moneda.Y * tilHeight, 33, 33);
               if (version.CPU == CPUVersion.ZX)
               {
                   graphics.DrawImage(this.ImageList[version.CPU.ToString() + "Moneda" + level.TileFondo.ToString()], destRect, srcRect, GraphicsUnit.Pixel);
               }
               else
               {
                   graphics.DrawImage(this.ImageList[version.CPU.ToString() + "Moneda"], destRect, srcRect, GraphicsUnit.Pixel);
               }
            }
         }
      }

      private void drawEnemies(Graphics graphics, int tilWidth, int tilHeight, ref Rectangle srcRect, ref Rectangle destRect)
      {
         Pen arrowPen = new Pen(Color.FromArgb(192, 255, 32, 32), 5.0f);

         arrowPen.EndCap = LineCap.ArrowAnchor;
         arrowPen.StartCap = LineCap.ArrowAnchor;

         foreach (Enemigo enemigo in this.level.Enemigos)
         {
            if (enemigo != null)
            {
               if (this.showArrows)
               {
                  int arrowY = enemigo.TileVert * tilHeight + (tilHeight / 2);
                  int arrowX1 = enemigo.TileIzq * tilWidth + (tilWidth / 2);
                  int arrowX2 = enemigo.TileDer * tilWidth + (tilWidth / 2);
                  graphics.DrawLine(arrowPen, arrowX1, arrowY, arrowX2, arrowY);
               }
               destRect = new Rectangle((enemigo.TileIzq + ((enemigo.TileDer - enemigo.TileIzq) / 2)) * tilWidth, enemigo.TileVert * tilHeight, 33, 33);
               graphics.DrawImage(this.ImageList[version.CPU.ToString() + enemigo.TipoEnemigo.ToString()], destRect, srcRect, GraphicsUnit.Pixel);
            }
         }

         arrowPen.Dispose();
      }

      internal Image GetPreview()
      {
         Bitmap previewBmp = new Bitmap(this.Width, this.Height);
         Graphics graphics = Graphics.FromImage(previewBmp);
         this.drawLevel(graphics, false);
         return previewBmp;
      }

      internal void SavePreview(string fileName)
      {
         Bitmap previewBmp = new Bitmap(this.Width, this.Height);
         Graphics graphics = Graphics.FromImage(previewBmp);
         this.drawLevel(graphics, false);
         previewBmp.Save(fileName);
      }
   }
}
