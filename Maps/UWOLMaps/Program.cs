using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;

namespace UWOLMaps
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            string logPath = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "UWOLMaps.log");

            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                if (Properties.Settings.Default.MostrarSplash)
                {
                    frmSplash splash = new frmSplash();
                    splash.Show();
                    Thread showSplash = new Thread(new ParameterizedThreadStart(showHideSplash));
                    showSplash.Start(splash);
                }

                Application.Run(new frmEditor());
            }
            catch (Exception ex)
            {
                System.IO.File.AppendAllText(logPath, ex.ToString() + "\\n");
                throw ex;
            }
        }

        static void Application_ThreadException(object sender, ThreadExceptionEventArgs e)
        {
            IntPtr pEP = System.Runtime.InteropServices.Marshal.GetExceptionPointers();
            ErrorDumper.CreateDump(
                    System.Diagnostics.Process.GetCurrentProcess().Id,
                    System.IO.Path.Combine(Environment.GetFolderPath( Environment.SpecialFolder.MyDocuments),"UWOLMaps.dmp"),
                    MinidumpType.MiniDumpNormal,
                    Process.GetCurrentProcess().Id,
                    pEP);
        }

        static void showHideSplash(object obj)
        {
            frmSplash form = obj as frmSplash;
            if (obj != null)
            {
                for (float opacity = 0.0f; opacity < 1.0f; opacity += 0.05f)
                {
                    setOpacity(form, opacity);
                    Thread.Sleep(20);
                }
                Thread.Sleep(1500);
                for (float opacity = 1.0f; opacity > 0.0f; opacity -= 0.05f)
                {
                    setOpacity(form, opacity);
                    Thread.Sleep(20);
                }
                closeSplash(form);
            }
        }

        private delegate void closeSplashDlg(frmSplash form);

        private static void closeSplash(frmSplash form)
        {
            if (!form.IsDisposed)
            {
                if (form.InvokeRequired)
                {
                    form.Invoke(new closeSplashDlg(closeSplash), new object[] { form });
                }
                else
                {
                    form.Close();
                }
            }
        }

        private delegate void setOpacityDlg(frmSplash form, float opacity);

        private static void setOpacity(frmSplash form, float opacity)
        {
            if (!form.IsDisposed)
            {
                if (form.InvokeRequired)
                {
                    form.Invoke(new setOpacityDlg(setOpacity), new object[] { form, opacity });
                }
                else
                {
                    form.Opacity = opacity;
                }
            }
        }
    }
}