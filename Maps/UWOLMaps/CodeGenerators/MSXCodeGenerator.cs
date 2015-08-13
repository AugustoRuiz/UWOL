using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace UWOLMaps.CodeGenerators
{
    public class MSXCodeGenerator : ICodeGenerator
    {
        #region ICodeGenerator Members

        public void CreateCode(Levels levels, System.IO.Stream stream)
        {
            foreach (Level nivel in levels)
            {
                int i;
                nivel.Ordena();

                stream.WriteByte((byte)nivel.TileFondo);
                
                // Plataformas
                for (i = 0; i < nivel.Plataformas.Count; i++)
                {
                    byte platByte1, platByte2;
                    
                    platByte1 = (byte) 
                                (
                                   (nivel.Plataformas[i].Longitud << 4) | 
                                   ((byte)nivel.Plataformas[i].TipoPlataforma << 1) | 
                                   (nivel.Plataformas[i].Direccion == Direccion.Horizontal ? 0 : 1)
                                );

                    platByte2 = (byte)
                                (
                                   (nivel.Plataformas[i].X << 4) | (nivel.Plataformas[i].Y)
                                );
                    
                    stream.WriteByte(platByte1);
                    stream.WriteByte(platByte2);
                }
                stream.WriteByte(0xFF);

                // Moviles
                for (i = 0; i < nivel.Enemigos.Count; i++)
                {
                    byte enemByte1, enemByte2;
                    
                    enemByte1 = (byte)
                                (
                                    (nivel.Enemigos[i].TileVert << 4) |
                                    ((byte)nivel.Enemigos[i].TipoEnemigo << 1) |
                                    (nivel.Enemigos[i].Velocidad == Velocidad.Lento? 0 : 1)
                                );

                    enemByte2 = (byte)
                                (
                                    (nivel.Enemigos[i].TileIzq << 4) |
                                    (nivel.Enemigos[i].TileDer)
                                );
                    
                    stream.WriteByte(enemByte1);
                    stream.WriteByte(enemByte2);
                }
                stream.WriteByte(0xFF);

                // Monedas
                for (i = 0; i < nivel.Monedas.Count; i++)
                {
                    byte coinByte;

                    coinByte = (byte)
                               (
                                    (nivel.Monedas[i].X << 4) |
                                    (nivel.Monedas[i].Y)
                               );

                    stream.WriteByte(coinByte);
                }
                stream.WriteByte(0xFF);
            }
            stream.WriteByte(0xFF);
        }

        public string GetFileName(string baseName)
        {
            return string.Format("MSX{0}.bin", baseName);
        }

        #endregion
    }
}
