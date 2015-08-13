using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace UWOLMaps.CodeGenerators
{
    public class PCCodeGenerator : ICodeGenerator
    {
        #region ICodeGenerator Members

        public void CreateCode(Levels levels, System.IO.Stream stream)
        {
            BinaryWriter writer = new BinaryWriter(stream);

            foreach (Level lvl in levels)
            {
                writer.Write((byte)lvl.TileFondo);
                foreach (Plataforma plat in lvl.Plataformas)
                {
                    writer.Write((byte)plat.Longitud);
                    writer.Write((byte)plat.TipoPlataforma);
                    writer.Write((byte)plat.Direccion);
                    writer.Write((byte)plat.X);
                    writer.Write((byte)plat.Y);
                }
                writer.Write((byte)0xFF);
                foreach (Enemigo enem in lvl.Enemigos)
                {
                    writer.Write((byte)enem.TileVert);
                    writer.Write((byte)enem.TipoEnemigo);
                    writer.Write((byte)enem.Velocidad);
                    writer.Write((byte)enem.TileIzq);
                    writer.Write((byte)enem.TileDer);
                }
                writer.Write((byte)0xFF);
                foreach (Moneda moneda in lvl.Monedas)
                {
                    writer.Write((byte)moneda.X);
                    writer.Write((byte)moneda.Y);
                }
                writer.Write((byte)0xFF);
            }
            writer.Write((byte)0xFF);
        }

        public string GetFileName(string baseName)
        {
            return string.Format("PC{0}.dat", baseName);
        }

        #endregion
    }
}
