using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.IO;
using System.Xml.Serialization;

namespace UWOLMaps
{
    [Serializable()]
    public class Level : INotifyPropertyChanged
    {
        public Level()
        {
            this.enemigos = new List<Enemigo>(Utils.MAX_ENEMIES);
            this.monedas = new List<Moneda>(Utils.MAX_COINS);
            this.plataformas = new List<Plataforma>(Utils.MAX_PLATFORMS);
        }

        private byte numPantalla;

        public byte NumPantalla
        {
            get { return numPantalla; }
            set
            {
                if (numPantalla != value)
                {
                    numPantalla = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("NumPantalla"));
                }
            }
        }

        private TilesFondo tileFondo;

        public TilesFondo TileFondo
        {
            get { return tileFondo; }
            set
            {
                if (tileFondo != value)
                {
                    tileFondo = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("TileFondo"));
                }
            }
        }

        private ColorZX paperColor;

        public ColorZX PaperColor
        {
            get { return paperColor; }
            set
            {
                if (paperColor != value)
                {
                    paperColor = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("PaperColor"));
                }
            }
        }

        private ColorZX inkColor;

        public ColorZX InkColor
        {
            get { return inkColor; }
            set
            {
                if (inkColor != value)
                {
                    inkColor = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("InkColor"));
                }
            }
        }

        private List<Plataforma> plataformas;

        public List<Plataforma> Plataformas
        {
            get { return plataformas; }
            set
            {
                if (plataformas != value)
                {
                    plataformas = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("Plataformas"));
                }
            }
        }

        private List<Moneda> monedas;

        public List<Moneda> Monedas
        {
            get { return monedas; }
            set
            {
                if (monedas != value)
                {
                    monedas = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("Monedas"));
                }
            }
        }

        private List<Enemigo> enemigos;

        public List<Enemigo> Enemigos
        {
            get { return enemigos; }
            set
            {
                if (enemigos != value)
                {
                    enemigos = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("Enemigos"));
                }
            }
        }

        public void Ordena()
        {
            this.enemigos.Sort();
            this.monedas.Sort();
            this.plataformas.Sort();
        }

        public static Level Load(string fileName)
        {
            Level loadedLevel;
            XmlSerializer formatter = new XmlSerializer(typeof(Level));
            Stream objNewFileStream = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.Read);
            loadedLevel = (Level)formatter.Deserialize(objNewFileStream);
            objNewFileStream.Close();

            return loadedLevel;
        }

        public void Save(string fileName)
        {
            XmlSerializer formatter = new XmlSerializer(typeof(Level));
            Stream objFileStream = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.None);
            formatter.Serialize(objFileStream, this);
            objFileStream.Close();
        }

        #region INotifyPropertyChanged Members

        protected virtual void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, e);
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        internal string ToString(Version version)
        {
            StringBuilder result = new StringBuilder();
            int i = 0;

            // Descriptor.
            result.AppendFormat(";.Pantalla {0}\n", this.numPantalla);

            if (version.CPU == CPUVersion.MSX)
            {
                result.AppendFormat("   {0} {1} ; FONDO\n", version.TokenDef, (byte)this.tileFondo);
            }
            else
            {
                result.Append("        ;TTPPPIII\n");
                result.AppendFormat("   {3} {4}{0}{1}{2}{5}            ; Descriptor\n",
                   Utils.ToBinary2((byte)this.tileFondo),
                   Utils.ToBinary3((byte)this.paperColor),
                   Utils.ToBinary3((byte)this.inkColor),
                   version.TokenDef,
                   version.PrefijoDatoBin,
                   version.EndOfByte);
            }

            // Plataformas.
            result.Append("\n        ;Objetos\n");
            result.Append("        ;datos \"al reves\", por eso del \"endian\"\n");
            result.Append("        ;LLLLTTTD   XXXXYYYY\n");
            for (i = 0; i < this.plataformas.Count; i++)
            {
                result.AppendFormat("   {0}\n", this.plataformas[i].ToString(version));
            }
            if (version.CPU != CPUVersion.MSX)
            {
                Plataforma plataformaVacia = new Plataforma();

                for (; i < version.MaxPlataformas; i++)
                {
                    result.AppendFormat("   {0}\n", plataformaVacia.ToString(version));
                }
            }

            result.AppendLine(version.EndOfSection);

            // Moviles.
            result.Append("\n        ;Moviles\n");
            result.Append("        ;YYYYTTTD   AAAABBBB\n");
            for (i = 0; i < this.enemigos.Count; i++)
            {
                result.AppendFormat("   {0}\n", this.enemigos[i].ToString(version));
            }
            if (version.CPU != CPUVersion.MSX)
            {
                Enemigo enemigoVacio = new Enemigo();

                for (; i < version.MaxEnemigos; i++)
                {
                    result.AppendFormat("   {0}\n", enemigoVacio.ToString(version));
                }
            }
            result.AppendLine(version.EndOfSection);

            // Monedas.
            result.Append("\n        ;Monedas\n");
            result.Append("        ;XXXXYYYY\n");
            for (i = 0; i < this.monedas.Count; i++)
            {
                result.AppendFormat("   {0}\n", this.monedas[i].ToString(version));
            }
            if (version.CPU != CPUVersion.MSX)
            {
                Moneda monedaVacia = new Moneda();

                for (; i < version.MaxMonedas; i++)
                {
                    result.AppendFormat("   {0}\n", monedaVacia.ToString(version));
                }
            }
            result.AppendLine(version.EndOfSection);

            return result.ToString();
        }
    }
}

//typedef struct 
//{ 
//   uchar descriptor; 
//   unsigned int obj[10]; 
//   unsigned int movil[3]; 
//   unsigned char coin[10]; 
//} FASE; 

//extern FASE fases[]; 

//// Definición de las pantallas del juego según esta documentación: 
///* 
//B:  L:   C: 
//0   1    Byte descriptor de fase: TTPPPIII 
//         TT : 2 bits, número de tile de fondo (0 a 3) 
//         PPP: 3 bits, color de PAPER (0 a 8) 
//         III: 3 bits, color de INK (0 a 8) 

//1   2    Descriptor de objeto, 2 bytes: XXXXYYYYLLLLTTTD 
//         XXXX: 4 bits, coordenada X en tiles de la esquina sup.izq (0 a 15) 
//         YYYY: 4 bits, coordenada Y en tiles de la esquina sup.izq (0 a 15) 
//         LLLL: 4 bits, longitud del objeto en tiles (0 a 15) 
//         TTT: 3 bits, número de objeto (0 a 7) 
//         D: 1 bit, dirección: 0 horz, 1 vert. 

//         P.ej. una plataforma de 6 tiles de ancho en (2, 2) del tipo 2, sería: 
//         0010001001100100 
//         [ 2][ 2][ 6][2]0 

//... x 10 
//21   2   Descriptor de enemigo, 2 bytes: AAAABBBBYYYYRTTV 
//         AAAA: 4 bits, nº tile horizontal límite izquierdo (0 a 15) 
//         BBBB: 4 bits, nº tile horizontal límite derecho (0 a 15) 
//         YYYY: 4 bits, nº tile vertical (0 a 15) 
//         R: 1 bit, reservado (por ahora no sirve pa ná). 
//         TT: 2 bits, tipo de enemigo (0 a 3); 1 = franky, 2 = vampy, 3 = fanty. 
//         V: 1 bit, velocidad: 0 lento (de 1 en 1 pixels), 1 rápido (de 2 en 2 píxels). 
//... x 3 
//27   1   Descriptor de moneda, 1 byte: XXXXYYYY 
//         XXXX: 4 bits, coordenada X en tiles (0 a 15) 
//         YYYY: 4 bits, coordenada Y en tiles (0 a 15) 
//... x 10 
//*/ 
//#asm 
//._fases 
//; Pantalla 0: 
//        ;TTPPPIII 
//   defb @00000101            ; Descriptor 

//        ;datos "al reves", por eso del "endian" 
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

//        ;YYYYTTTD   AAAABBBB 
//   defb @00100010, @00010011   ; Moviles 
//   defb @00110101, @01101010 
//   defb @10000110, @01101011 

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

//; Pantalla 1: 
//;... 
//#endasm 