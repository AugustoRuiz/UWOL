using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace UWOLMaps
{
   public class Version
   {
      private CPUVersion cpu;

      public CPUVersion CPU
      {
         get { return cpu; }
         set { cpu = value; }
      }

      private int maxEnemigos;

      public int MaxEnemigos
      {
         get { return maxEnemigos; }
         set { maxEnemigos = value; }
      }

      private int maxMonedas;

      public int MaxMonedas
      {
         get { return maxMonedas; }
         set { maxMonedas = value; }
      }

      private int maxPlataformas;

      public int MaxPlataformas
      {
         get { return maxPlataformas; }
         set { maxPlataformas = value; }
      }

      public string TokenDef
      {
         get
         {
            string token = null;
            switch (cpu)
            { 
               case CPUVersion.ZX:
                  token = "defb";
                  break;
               case CPUVersion.MSX:
                  token = "db";
                  break;
               case CPUVersion.CPC:
                  token = "defb";
                  break;
            }
            return token;
         }
      }

      public string PrefijoDatoBin
      {
         get
         {
            string prefijo = null;
            switch (cpu)
            {
               case CPUVersion.ZX:
                  prefijo = "@";
                  break;
               case CPUVersion.MSX:
                  prefijo = "";
                  break;
               case CPUVersion.CPC:
                  prefijo = "@";
                  break;
            }
            return prefijo;
         }
      }

      public string EndOfSection
      {
         get
         {
            string token = null;
            switch (cpu)
            {
               case CPUVersion.ZX:
                  token = "";
                  break;
               case CPUVersion.MSX:
                  token = "\n   DB   $FF";
                  break;
               case CPUVersion.CPC:
                  token = "";
                  break;
            }
            return token;
         }
      }

      public string EndOfByte
      {
         get
         {
            string token = null;
            switch (cpu)
            {
               case CPUVersion.ZX:
                  token = "";
                  break;
               case CPUVersion.MSX:
                  token = "B";
                  break;
               case CPUVersion.CPC:
                  token = "";
                  break;
            }
            return token;
         }
      }

      public static Version DameVersion(CPUVersion version)
      {
         Version nuevaVersion = new Version();

         nuevaVersion.cpu = version;

         switch (version)
         {
            case CPUVersion.ZX:
               nuevaVersion.maxEnemigos = 3;
               nuevaVersion.maxMonedas = 10;
               nuevaVersion.maxPlataformas = 10;
               break;
            case CPUVersion.MSX:
               nuevaVersion.maxEnemigos = 3;
               nuevaVersion.maxMonedas = 10;
               nuevaVersion.maxPlataformas = 10;
               break;
            case CPUVersion.CPC:
               nuevaVersion.maxEnemigos = 3;
               nuevaVersion.maxMonedas = 10;
               nuevaVersion.maxPlataformas = 10;
               break;
           case CPUVersion.PC:
               nuevaVersion.maxEnemigos = 3;
               nuevaVersion.maxMonedas = 10;
               nuevaVersion.maxPlataformas = 10;
               break;
         }

         return nuevaVersion;
      }

   }
}
