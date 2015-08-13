using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace UWOLMaps.CodeGenerators
{
    public class ZXbinCodeGenerator : ICodeGenerator
    {
        #region ICodeGenerator Members

        public void CreateCode(Levels levels, System.IO.Stream stream)
        {
            BinaryWriter writer = new BinaryWriter(stream);

            for (int levelNum = 0; levelNum < levels.Count && levelNum < 41; levelNum++)
            {
                Level lvl = levels[levelNum];

                byte nextByte;

                nextByte = (byte)(((byte)lvl.TileFondo << 6) |
                              ((byte)lvl.PaperColor << 3) |
                              ((byte)lvl.InkColor));

                writer.Write(nextByte);
                int i = 0;
                for (i = 0; i < lvl.Plataformas.Count; i++)
                {
                    Plataforma plat = lvl.Plataformas[i];
                    nextByte = (byte)(((byte)plat.Longitud) << 4 | ((byte)plat.TipoPlataforma) << 1 | ((byte)plat.Direccion));
                    writer.Write(nextByte);
                    nextByte = (byte)(((byte)plat.X) << 4 | ((byte)plat.Y));
                    writer.Write(nextByte);
                }
                for (; i < Version.DameVersion(CPUVersion.ZX).MaxPlataformas; i++)
                {
                    writer.Write((byte)0);
                    writer.Write((byte)0);
                }

                for (i = 0; i < lvl.Enemigos.Count; i++)
                {
                    Enemigo enem = lvl.Enemigos[i];
                    nextByte = (byte)(((byte)enem.TileVert) << 4 | ((byte)enem.TipoEnemigo) << 1 | ((byte)enem.Velocidad));
                    writer.Write(nextByte);
                    nextByte = (byte)(((byte)enem.TileIzq) << 4 | ((byte)enem.TileDer));
                    writer.Write(nextByte);
                }
                for (; i < Version.DameVersion(CPUVersion.ZX).MaxEnemigos; i++)
                {
                    writer.Write((byte)0);
                    writer.Write((byte)0);
                }
                for (i = 0; i < lvl.Monedas.Count; i++)
                {
                    Moneda moneda = lvl.Monedas[i];
                    nextByte = (byte)(((byte)moneda.X) << 4 | ((byte)moneda.Y));
                    writer.Write(nextByte);
                }
                for (; i < Version.DameVersion(CPUVersion.ZX).MaxMonedas; i++)
                {
                    writer.Write((byte)0);
                }
            }
        }

        public string GetFileName(string baseName)
        {
            return string.Format("PC{0}.dat", baseName);
        }

        #endregion

    }
}
