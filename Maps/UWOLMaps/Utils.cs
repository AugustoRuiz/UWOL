using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.Reflection;
using System.IO;

namespace UWOLMaps
{

    [Serializable]
    public enum AppState
    {
        Normal,
        AddingEnemy,
        AddingPlatform,
        AddingCoin
    }

    [Serializable]
    public enum TipoEnemigo : byte
    {
        Wolfy = 0,
        Franky = 1,
        Vampy = 2//,
        //Fanty = 3
    }

    [Serializable]
    public enum Velocidad : byte
    {
        Lento = 0,
        Rapido = 1
    }

    [Serializable]
    public enum Direccion : byte
    {
        Horizontal = 0,
        Vertical = 1
    }

    [Serializable]
    public enum TilesFondo : byte
    {
        Tile0 = 0,
        Tile1 = 1,
        Tile2 = 2,
        Tile3 = 3
    }

    [Serializable]
    public enum TilePlataforma : byte
    {
        Plat0 = 0,
        Plat1 = 1,
        Plat2 = 2,
        Plat3 = 3,
        Plat4 = 4,
        Plat5 = 5,
        Plat6 = 6,
        Plat7 = 7
    }

    [Serializable]
    public enum ColorZX : byte
    {
        Negro = 0,
        Azul = 1,
        Rojo = 2,
        Magenta = 3,
        Verde = 4,
        Cyan = 5,
        Amarillo = 6,
        Blanco = 7
    }

    // Ya sé que con este enumerado habrá polémica por el orden... Que quede claro que 
    // yo soy CPCero!!!!
    public enum CPUVersion : byte
    {
        ZX = 0,
        MSX = 1,
        CPC = 2,
        PC = 3
    }

    internal class Utils
    {
        internal static string logPath = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "UWOLMaps.log");

        internal const int MAX_COINS = 10;
        internal const int MAX_ENEMIES = 3;
        internal const int MAX_PLATFORMS = 10;

        internal static string[] fileNamesBN = {"Tile0","Tile1","Tile2","Tile3",
                                                "sTile0","sTile1","sTile2","sTile3",
                                                "MonedaTile0","MonedaTile1","MonedaTile2","MonedaTile3",
                                                "Franky","Fanty","Vampy","Wolfy"};


        internal static string ToBinary4(byte number)
        {
            string result;

            switch (number)
            {
                case 0:
                    result = "0000";
                    break;
                case 1:
                    result = "0001";
                    break;
                case 2:
                    result = "0010";
                    break;
                case 3:
                    result = "0011";
                    break;
                case 4:
                    result = "0100";
                    break;
                case 5:
                    result = "0101";
                    break;
                case 6:
                    result = "0110";
                    break;
                case 7:
                    result = "0111";
                    break;
                case 8:
                    result = "1000";
                    break;
                case 9:
                    result = "1001";
                    break;
                case 10:
                    result = "1010";
                    break;
                case 11:
                    result = "1011";
                    break;
                case 12:
                    result = "1100";
                    break;
                case 13:
                    result = "1101";
                    break;
                case 14:
                    result = "1110";
                    break;
                case 15:
                    result = "1111";
                    break;
                default:
                    throw new ArgumentException(string.Format("Value {0} cannot be translated to 4 bits", number));
            }
            return result;
        }

        internal static string ToBinary3(byte number)
        {
            string result;

            switch (number)
            {
                case 0:
                    result = "000";
                    break;
                case 1:
                    result = "001";
                    break;
                case 2:
                    result = "010";
                    break;
                case 3:
                    result = "011";
                    break;
                case 4:
                    result = "100";
                    break;
                case 5:
                    result = "101";
                    break;
                case 6:
                    result = "110";
                    break;
                case 7:
                    result = "111";
                    break;
                default:
                    throw new ArgumentException(string.Format("Value {0} cannot be translated to 3 bits", number));
            }
            return result;
        }

        internal static string ToBinary2(byte number)
        {
            string result;

            switch (number)
            {
                case 0:
                    result = "00";
                    break;
                case 1:
                    result = "01";
                    break;
                case 2:
                    result = "10";
                    break;
                case 3:
                    result = "11";
                    break;
                default:
                    throw new ArgumentException(string.Format("Value {0} cannot be translated to 2 bits", number));
            }
            return result;
        }

        internal static string ToBinary1(byte number)
        {
            string result;

            switch (number)
            {
                case 0:
                    result = "0";
                    break;
                case 1:
                    result = "1";
                    break;
                default:
                    throw new ArgumentException(string.Format("Value {0} cannot be translated to 1 bit", number));
            }
            return result;
        }

        internal static Color ColorZXToColor(ColorZX color)
        {
            Color result = Color.FromArgb(0, 0, 0);

            switch (color)
            {
                case ColorZX.Negro:
                    result = Color.FromArgb(0, 0, 0);
                    break;
                case ColorZX.Azul:
                    result = Color.FromArgb(0, 0, 200);
                    break;
                case ColorZX.Rojo:
                    result = Color.FromArgb(200, 0, 0);
                    break;
                case ColorZX.Magenta:
                    result = Color.FromArgb(200, 0, 200);
                    break;
                case ColorZX.Verde:
                    result = Color.FromArgb(0, 196, 0);
                    break;
                case ColorZX.Cyan:
                    result = Color.FromArgb(0, 196, 200);
                    break;
                case ColorZX.Amarillo:
                    result = Color.FromArgb(200, 196, 0);
                    break;
                case ColorZX.Blanco:
                    result = Color.FromArgb(200, 196, 200);
                    break;
            }
            return result;
        }

        internal static void updateZXTiles(Dictionary<string, Image> tiles, Level currentLevel)
        {
            Color paperColor = Utils.ColorZXToColor(currentLevel.PaperColor);
            Color inkColor = Utils.ColorZXToColor(currentLevel.InkColor);

            Console.WriteLine("updateZXTiles");

            foreach (string tileName in fileNamesBN)
            {
                System.IO.File.AppendAllText(Utils.logPath, string.Format("Fetching tile: {0}{1}", tileName, Environment.NewLine));
                Image originalImage = tiles[tileName];

                string currentKey = string.Format("ZX{0}", tileName);

                if (tiles.ContainsKey(currentKey))
                    tiles[currentKey].Dispose();

                System.IO.File.AppendAllText(logPath, string.Format("Storing tile: {0}{1}", currentKey, Environment.NewLine));
                tiles[currentKey] = createTile(originalImage, paperColor, inkColor);
            }
        }

        internal static Image createTile(Image originalImage, Color paperColor, Color inkColor)
        {
            int imgHeight = originalImage.Height;
            int imgWidth = originalImage.Width;

            Image imgCopy = new Bitmap(imgWidth, imgHeight, PixelFormat.Format32bppArgb);
            Graphics g = Graphics.FromImage(imgCopy);
            g.DrawImage(originalImage, 0, 0, imgWidth, imgHeight);
            g.Dispose();

            Image newTile = new Bitmap(imgWidth, imgHeight, PixelFormat.Format32bppArgb);
            int paper = paperColor.ToArgb();
            int ink = inkColor.ToArgb();

            BitmapData bmDataSrc = ((Bitmap)imgCopy).LockBits(new Rectangle(0, 0, imgWidth, imgHeight), ImageLockMode.ReadOnly, imgCopy.PixelFormat);
            BitmapData bmDataDst = ((Bitmap)newTile).LockBits(new Rectangle(0, 0, imgWidth, imgHeight), ImageLockMode.WriteOnly, newTile.PixelFormat);

            IntPtr bmSrcPtr = bmDataSrc.Scan0;
            IntPtr bmDstPtr = bmDataDst.Scan0;

            IntPtr readPtr = bmSrcPtr;
            IntPtr writePtr = bmDstPtr;

            int offsetSrc = 0;
            int offsetDst = 0;

            for (int line = 0; line < imgHeight; line++)
            {
                for (int col = 0; col < imgWidth; col++)
                {
                    int colOffset = col << 2;
                    int iSrcColor = System.Runtime.InteropServices.Marshal.ReadInt32(readPtr, offsetSrc + colOffset);

                    if (iSrcColor != 0)
                    {
                        System.Runtime.InteropServices.Marshal.WriteInt32(writePtr, offsetDst + colOffset, iSrcColor == -1 ? ink : paper);
                    }
                }
                // Avanzar punteros a la siguiente línea.
                offsetSrc += bmDataSrc.Stride;
                offsetDst += bmDataDst.Stride;
                readPtr = bmSrcPtr;
                writePtr = bmDstPtr;
            }

            ((Bitmap)imgCopy).UnlockBits(bmDataSrc);
            ((Bitmap)newTile).UnlockBits(bmDataDst);

            return newTile;
        }

        internal static Dictionary<string, Image> getTiles()
        {
            Dictionary<string, Image> tiles = new Dictionary<string, Image>();

            string[] fileNames = {"Tile0","Tile1","Tile2","Tile3",
                               "sTile0","sTile1","sTile2","sTile3",
                               "Plat0","Plat1","Plat2","Plat3",
                               "Plat4","Plat5","Plat6","Plat7",
                               "Moneda","Franky","Fanty","Vampy","Wolfy"};

            try
            {
                // Aquí añade los tiles de las versiones, si bien, algunas de las de ZX van a 
                // ser reemplazadas cuando se cambien los combos de Ink y Pen.
                foreach (string fileName in fileNames)
                {
                    System.IO.File.AppendAllText(logPath, string.Format("Loading tile: {0}{1}", fileName, Environment.NewLine));
                    foreach (string versionName in System.Enum.GetNames(typeof(CPUVersion)))
                    {
                        string currentName = string.Format("{0}{1}", versionName, fileName);
                        string realFile = string.Format("{1}\\images\\{0}.png", currentName, GetAppPath());
                        System.IO.File.AppendAllText(logPath, string.Format("Loading file for version {2}: {0}{1}", realFile, Environment.NewLine, versionName));
                        Image tile = Image.FromFile(realFile);
                        tiles.Add(currentName, tile);
                    }
                }
                // Cargamos los tiles en blanco y negro...
                foreach (string fileName in Utils.fileNamesBN)
                {
                    string realFile = string.Format("{1}\\images\\{0}.png", fileName, GetAppPath());
                    System.IO.File.AppendAllText(logPath, string.Format("Loading speccy tile: {0}{1}", realFile, Environment.NewLine));
                    Image tile = Image.FromFile(realFile);
                    tiles.Add(fileName, tile);
                }
            }
            catch (Exception ex)
            {
                System.IO.File.AppendAllText(logPath, ex.ToString() + "\\n");
                throw ex;
            }

            return tiles;
        }

        internal static string GetAppPath()
        {
            return Path.GetDirectoryName(Application.ExecutablePath);
        }

    }
}
