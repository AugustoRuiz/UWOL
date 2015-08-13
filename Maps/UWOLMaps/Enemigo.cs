using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace UWOLMaps
{
   [Serializable()]
   public class Enemigo : IComparable
   {
      private byte tileIzq;

      [Category("Posición"), Description("Tile izquierdo del recorrido del enemigo."), 
      DisplayName("Tile Izquierdo")]
      public byte TileIzq
      {
         get { return tileIzq; }
         set
         {
            if (value > tileDer)
            {
               tileDer = value;
            }
            tileIzq = value;
         }
      }

      private byte tileDer;

      [Category("Posición"), Description("Tile derecho del recorrido del enemigo."),
      DisplayName("Tile Derecho")]
      public byte TileDer
      {
         get { return tileDer; }
         set
         {
            if (value < tileIzq)
            {
               tileIzq = value;
            }
            tileDer = value;
         }
      }

      private byte tileVert;

      [Category("Posición"), Description("Tile vertical del enemigo."),
      DisplayName("Tile Vertical")]
      public byte TileVert
      {
         get { return tileVert; }
         set { tileVert = value; }
      }

      private TipoEnemigo tipoEnemigo;

      [Category("Apariencia"), Description("Tipo de enemigo."),
      DisplayName("Tipo de enemigo")]
      public TipoEnemigo TipoEnemigo
      {
         get { return tipoEnemigo; }
         set { tipoEnemigo = value; }
      }

      private Velocidad velocidad;

      [Category("Comportamiento"), Description("Velocidad. Lento = 1px, Rápido = 2px."),
      DisplayName("Velocidad")]
      public Velocidad Velocidad
      {
         get { return velocidad; }
         set { velocidad = value; }
      }

      public string ToString(Version version)
      {
         return string.Format("{5} {6}{0}{1}{2}{7}, {6}{3}{4}{7}",
                              Utils.ToBinary4(this.tileVert),
                              Utils.ToBinary3((byte)this.TipoEnemigo),
                              Utils.ToBinary1((byte)this.Velocidad),
                              Utils.ToBinary4(this.tileIzq),
                              Utils.ToBinary4(this.tileDer),
                              version.TokenDef,
                              version.PrefijoDatoBin,
                              version.EndOfByte);
      }

      public bool IsInTile(byte tileX, byte tileY)
      {
         return ((tileX >= tileIzq) && (tileX <= TileDer) && (tileY == tileVert));
      }

      #region IComparable Members

      public int CompareTo(object obj)
      {
         Enemigo objAComparar = obj as Enemigo;

         if (objAComparar == null)
         {
            throw new ArgumentException(string.Format("No se puede comparar con un objeto de tipo {0}", obj.GetType().FullName));
         }
         if (this.tileVert < objAComparar.tileVert || 
            (this.tileVert == objAComparar.tileVert && this.tileIzq < objAComparar.tileIzq) || 
            (this.tileVert == objAComparar.tileVert && this.tileIzq == objAComparar.tileIzq && this.tileDer < objAComparar.tileDer))
         {
            return -1;
         }
         if (this.tileVert == objAComparar.tileVert && 
             this.tileIzq == objAComparar.tileIzq && 
             this.tileDer == objAComparar.tileDer)
         {
            return 0;
         }
         return 1;
      }

      #endregion
   }
}

//21   2   Descriptor de enemigo, 2 bytes: AAAABBBBYYYYRTTV 
//         AAAA: 4 bits, nº tile horizontal límite izquierdo (0 a 15) 
//         BBBB: 4 bits, nº tile horizontal límite derecho (0 a 15) 
//         YYYY: 4 bits, nº tile vertical (0 a 15) 
//         R: 1 bit, reservado (por ahora no sirve pa ná). 
//         TT: 2 bits, tipo de enemigo (0 a 3); 1 = franky, 2 = vampy, 3 = fanty. 
//         V: 1 bit, velocidad: 0 lento (de 1 en 1 pixels), 1 rápido (de 2 en 2 píxels). 

//        ;YYYYTTTD   AAAABBBB 
//   defb @00100010, @00010011   ; Moviles 
//   defb @00110101, @01101010 
//   defb @10000110, @01101011 