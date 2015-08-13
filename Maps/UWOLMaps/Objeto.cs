using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.Serialization;
using System.ComponentModel;

namespace UWOLMaps
{
   [Serializable()]
   public class Objeto
   {
      internal const string NullString = "defb @00000000, @00000000";

      private byte x;

      [Category("Posición"), Description("Posición de la plataforma / del objeto (en tiles) en el eje X.")]
      public byte X
      {
         get { return x; }
         set { x = value; }
      }

      private byte y;

      [Category("Posición"), Description("Posición de la plataforma / del objeto (en tiles) en el eje Y.")]
      public byte Y
      {
         get { return y; }
         set { y = value; }
      }

      private byte longitud;

      [Category("Posición"), Description("Número de tiles que ocupa la plataforma / el objeto.")]
      public byte Longitud
      {
         get { return longitud; }
         set { longitud = value; }
      }

      private TilePlataforma tipoPlataforma;

      [Category("Apariencia"), Description("Tipo de plataforma / objeto."),
      DisplayName("Tipo de Plataforma")]
      public TilePlataforma TipoPlataforma
      {
         get { return tipoPlataforma; }
         set { tipoPlataforma = value; }
      }

      private Direccion direccion;

      [Category("Posición"), Description("Dirección de la plataforma / objeto.")]
      public Direccion Direccion
      {
         get { return direccion; }
         set { direccion = value; }
      }

      public override string ToString()
      {
         return string.Format("defb @{0}{1}{2}, @{3}{4}",
                              Utils.ToBinary4(this.longitud),
                              Utils.ToBinary3((byte)this.tipoPlataforma),
                              Utils.ToBinary1((byte)this.direccion),
                              Utils.ToBinary4(this.x),
                              Utils.ToBinary4(this.y));
      }

      public bool IsInTile(byte tileX, byte tileY)
      {
         bool result = false;

         if (this.direccion == Direccion.Horizontal)
         {
            result = ((tileX >= this.x) && (tileX < this.x + this.longitud) && (this.y == tileY));
         }
         else
         {
            result = ((tileY >= this.y) && (tileY < this.y + this.longitud) && (this.x == tileX));
         }

         return result;
      }
   }
}

//1   2    Descriptor de objeto, 2 bytes: XXXXYYYYLLLLTTTD 
//         XXXX: 4 bits, coordenada X en tiles de la esquina sup.izq (0 a 15) 
//         YYYY: 4 bits, coordenada Y en tiles de la esquina sup.izq (0 a 15) 
//         LLLL: 4 bits, longitud del objeto en tiles (0 a 15) 
//         TTT: 3 bits, número de objeto (0 a 7) 
//         D: 1 bit, dirección: 0 horz, 1 vert. 

//         P.ej. una plataforma de 6 tiles de ancho en (2, 2) del tipo 2, sería: 
//         0010001001100100 
//         [ 2][ 2][ 6][2]0 

//        ;LLLLTTTD   XXXXYYYY    
//   defb @11001100, @00000000    ; Objetos 
//   defb @00111100, @00010011 
//   defb @01011100, @01100100 
//   defb @00101100, @00110110 
//   defb @00011100, @01011000 
//   defb @11001100, @00001001 
//   defb @00000000, @00000000 
//   defb @00000000, @00000000 
//   defb @00000000, @00000000 
//   defb @00000000, @00000000 
