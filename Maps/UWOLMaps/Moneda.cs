using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace UWOLMaps
{
   [Serializable()]
   public class Moneda : IComparable
   {
      private byte x;

      [Category("Posición"), Description("Posición de la moneda (en tiles) en el eje X.")]
      public byte X
      {
         get { return x; }
         set { x = value; }
      }

      private byte y;

      [Category("Posición"), Description("Posición de la moneda (en tiles) en el eje Y.")]
      public byte Y
      {
         get { return y; }
         set { y = value; }
      }

      public string ToString(Version version)
      {
         return string.Format("{2} {3}{0}{1}{4}", 
                       Utils.ToBinary4(this.x),
                       Utils.ToBinary4(this.y),
                       version.TokenDef,
                       version.PrefijoDatoBin,
                       version.EndOfByte);
      }

      public bool IsInTile(byte tileX, byte tileY)
      {
         return ((x == tileX) && (y == tileY));
      }

      #region IComparable Members

      public int CompareTo(object obj)
      {
         Moneda objAComparar = obj as Moneda;

         if (objAComparar == null)
         {
            throw new ArgumentException(string.Format("No se puede comparar con un objeto de tipo {0}", obj.GetType().FullName));
         }
         if (this.y < objAComparar.y || 
            (this.y == objAComparar.y && this.x < objAComparar.x))
         {
            return -1;
         }
         if (this.y == objAComparar.y && this.x == objAComparar.x)
         {
            return 0;
         }
         return 1;
      }

      #endregion
   }
}

//27   1   Descriptor de moneda, 1 byte: XXXXYYYY 
//         XXXX: 4 bits, coordenada X en tiles (0 a 15) 
//         YYYY: 4 bits, coordenada Y en tiles (0 a 15) 
//... x 10 

//        ;XXXXYYYY 
//   defb @00010010            ; Monedas 
//   defb @00110010 
//   defb @00110101 
//   defb @10100011 
//   defb @10000111 
//   defb @10010111 
//   defb @10100111 
//   defb @00000000 
//   defb @00000000 
//   defb @00000000 