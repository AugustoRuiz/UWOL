using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Drawing.Imaging;
using System.Reflection;
using System.Diagnostics;
using UWOLMaps.CodeGenerators;

namespace UWOLMaps
{
    [Serializable]
    public partial class frmEditor : Form
    {
        private Level currentLevel = new Level();
        private AppState currentStatus;

        private TipoEnemigo tipoEnemigo = TipoEnemigo.Franky;
        private TilePlataforma tipoPlataforma = TilePlataforma.Plat0;

        private bool isDrawing = false;

        private bool initializing = false;

        private Version currentVersion = Version.DameVersion(CPUVersion.ZX);

        private Plataforma plataformaActual = null;

        private byte tileXIni = 0, tileYIni = 0;

        private object seleccionado;

        private Dictionary<string, Image> tiles = new Dictionary<string, Image>();

        private Levels levels = new Levels();

        private ColorZX lastInk, lastPaper;

        public frmEditor()
        {
            initializing = true;
            InitializeComponent();
            tiles = Utils.getTiles();

            currentLevel.InkColor = ColorZX.Blanco;
            currentLevel.PaperColor = ColorZX.Negro;

            updateTiles();

            initializeView();
            loadVersionCombo();
            levels.Add(currentLevel);
            this.levelSelector.Levels = levels;
            initializing = false;
        }

        private void updateTiles()
        {
            if (lastInk != currentLevel.InkColor || lastPaper != currentLevel.PaperColor)
            {
                Utils.updateZXTiles(tiles, currentLevel);
            }
            lastInk = currentLevel.InkColor;
            lastPaper = currentLevel.PaperColor;
        }

        private void initializeView()
        {
            currentVersion = Version.DameVersion((CPUVersion)System.Enum.Parse(typeof(CPUVersion), Properties.Settings.Default.VersionPorDefecto));

            this.currentLevelView.GridColor = Properties.Settings.Default.ColorRejilla;
            colorDlg.Color = this.currentLevelView.GridColor;
            this.currentLevelView.ShowGrid = Properties.Settings.Default.MostrarRejilla;
            mostrarRejillaToolStripMenuItem.Checked = this.currentLevelView.ShowGrid;
            this.currentLevelView.ShowArrows = Properties.Settings.Default.MostrarDesplazamientos;
            mostrarDesplazamientosToolStripMenuItem.Checked = this.currentLevelView.ShowArrows;

            this.currentLevelProp.Level = currentLevel;
            this.currentLevelView.Level = currentLevel;
            this.currentLevel.PropertyChanged += new PropertyChangedEventHandler(currentLevel_PropertyChanged);
            
            updateTiles();
            
            this.currentLevelProp.ImageList = tiles;
            this.currentLevelView.ImageList = tiles;

            updateView();
        }

        void currentLevel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            updateTiles();
            updateView();
        }

        private void binarioToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CodeView frmCode = new CodeView();

            frmCode.Levels = this.levels;
            frmCode.Version = this.currentVersion;
            frmCode.Show();
        }

        private void guardarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            sfd.Filter = "UWOL Maps|*.uwolmap|All Files|*.*";
            sfd.Title = "Selecciona el fichero a guardar";

            if (sfd.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    this.levels.Save(sfd.FileName);
                }
                catch (Exception exc)
                {
                    MessageBox.Show(string.Format("Error:\n{0}", exc.ToString()), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void abrirToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    levels = Levels.Load(ofd.FileName);
                    this.levelSelector.Levels = levels;

                    if (levels.Count > 0)
                        currentLevel = levels[0];

                    initializeView();
                }
                catch (Exception exc)
                {
                    MessageBox.Show(string.Format("Error:\n{0}", exc.ToString()), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void importarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    Levels tmpLevels = Levels.Load(ofd.FileName);

                    foreach (Level tmpLevel in tmpLevels)
                    {
                        levels.Add(tmpLevel);
                    }

                    initializeView();
                    this.levelSelector.RedrawAll();
                }
                catch (Exception exc)
                {
                    MessageBox.Show(string.Format("Error:\n{0}", exc.ToString()), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void salirToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void currentLevelProp_ObjectChanged(object sender, EventArgs e)
        {
            updateTiles();
            updateView();
        }

        private void updateView()
        {
            this.currentLevel.Ordena();

            this.currentLevelView.Version = this.currentVersion;
            this.currentLevelProp.Version = this.currentVersion;
            this.levelSelector.Version = this.currentVersion;

            this.currentLevelView.Invalidate();
            this.currentLevelProp.Invalidate();
            this.levelSelector.UpdateCurrentLevel();

            this.lblStatus.Text = string.Format("Plataformas: {0}, Enemigos: {1}, Monedas: {2}",
               this.currentLevel.Plataformas.Count,
               this.currentLevel.Enemigos.Count,
               this.currentLevel.Monedas.Count);

            enableToolsTripButtons();
        }

        private void enableToolsTripButtons()
        {
            if (this.currentLevel.Enemigos.Count == currentVersion.MaxEnemigos)
            {
                //fantyToolStripButton.Enabled = false;
                frankyToolStripButton.Enabled = false;
                vampyToolStripButton.Enabled = false;
                wolfyToolStripButton.Enabled = false;
                if (currentStatus == AppState.AddingEnemy)
                {
                    //fantyToolStripButton.Checked = false;
                    frankyToolStripButton.Checked = false;
                    vampyToolStripButton.Checked = false;
                    wolfyToolStripButton.Checked = false;

                    flechaToolStripButton.Checked = true;
                    currentStatus = AppState.Normal;
                }
            }
            else
            {
                //fantyToolStripButton.Enabled = true;
                frankyToolStripButton.Enabled = true;
                vampyToolStripButton.Enabled = true;
                wolfyToolStripButton.Enabled = true;
            }

            if (currentLevel.Monedas.Count == currentVersion.MaxMonedas)
            {
                coinToolStripButton.Enabled = false;
                if (currentStatus == AppState.AddingCoin)
                {
                    coinToolStripButton.Checked = false;
                    flechaToolStripButton.Checked = true;
                    currentStatus = AppState.Normal;
                }
            }
            else
            {
                coinToolStripButton.Enabled = true;
            }

            if (!(this.isDrawing) && this.currentLevel.Plataformas.Count == currentVersion.MaxPlataformas)
            {
                tipo0ToolStripButton.Enabled = false;
                tipo1ToolStripButton.Enabled = false;
                tipo2ToolStripButton.Enabled = false;
                tipo3ToolStripButton.Enabled = false;
                tipo4ToolStripButton.Enabled = false;
                tipo5ToolStripButton.Enabled = false;
                tipo6ToolStripButton.Enabled = false;
                tipo7ToolStripButton.Enabled = false;

                if (currentStatus == AppState.AddingPlatform)
                {
                    tipo0ToolStripButton.Checked = false;
                    tipo1ToolStripButton.Checked = false;
                    tipo2ToolStripButton.Checked = false;
                    tipo3ToolStripButton.Checked = false;
                    tipo4ToolStripButton.Checked = false;
                    tipo5ToolStripButton.Checked = false;
                    tipo6ToolStripButton.Checked = false;
                    tipo7ToolStripButton.Checked = false;
                    flechaToolStripButton.Checked = true;
                    currentStatus = AppState.Normal;
                }
            }
            else
            {
                tipo0ToolStripButton.Enabled = true;
                tipo1ToolStripButton.Enabled = true;
                tipo2ToolStripButton.Enabled = true;
                tipo3ToolStripButton.Enabled = true;
                tipo4ToolStripButton.Enabled = true;
                tipo5ToolStripButton.Enabled = true;
                tipo6ToolStripButton.Enabled = true;
                tipo7ToolStripButton.Enabled = true;
            }

            borrarToolStripButton.Enabled = (this.currentLevelProp.SelectedObject != null);
        }

        private void loadVersionCombo()
        {
            versionCboBox.Items.Clear();

            foreach (CPUVersion value in System.Enum.GetValues(typeof(CPUVersion)))
            {
                versionCboBox.Items.Add(value);
            }

            versionCboBox.SelectedIndex = versionCboBox.FindStringExact(currentVersion.CPU.ToString());
        }

        private void versionCboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (versionCboBox.SelectedIndex != -1)
            {
                if (!initializing)
                {
                    currentVersion = Version.DameVersion((CPUVersion)versionCboBox.SelectedItem);
                    Properties.Settings.Default.VersionPorDefecto = versionCboBox.Text;
                    Properties.Settings.Default.Save();
                }
                
                levelSelector.Invalidate();

                this.updateView();
            }
        }


        private void flechaToolStripButton_Click(object sender, EventArgs e)
        {
            setNormalStatus();
        }

        private void setNormalStatus()
        {
            this.currentStatus = AppState.Normal;

            this.flechaToolStripButton.Checked = true;
            this.vampyToolStripButton.Checked = false;
            //this.fantyToolStripButton.Checked = false;
            this.wolfyToolStripButton.Checked = false;
            this.frankyToolStripButton.Checked = false;
            this.coinToolStripButton.Checked = false;
            uncheckPlatforms();
        }

        private void addCoin(object sender, EventArgs e)
        {
            this.currentStatus = AppState.AddingCoin;

            this.flechaToolStripButton.Checked = false;
            this.vampyToolStripButton.Checked = false;
            //this.fantyToolStripButton.Checked = false;
            this.wolfyToolStripButton.Checked = false;
            this.frankyToolStripButton.Checked = false;
            uncheckPlatforms();
        }

        private void addFranky(object sender, EventArgs e)
        {
            this.currentStatus = AppState.AddingEnemy;
            tipoEnemigo = TipoEnemigo.Franky;

            this.flechaToolStripButton.Checked = false;
            this.vampyToolStripButton.Checked = false;
            //this.fantyToolStripButton.Checked = false;
            this.wolfyToolStripButton.Checked = false;
            this.coinToolStripButton.Checked = false;
            uncheckPlatforms();
        }

        private void addVampy(object sender, EventArgs e)
        {
            this.currentStatus = AppState.AddingEnemy;
            tipoEnemigo = TipoEnemigo.Vampy;

            this.flechaToolStripButton.Checked = false;
            this.frankyToolStripButton.Checked = false;
            //this.fantyToolStripButton.Checked = false;
            this.wolfyToolStripButton.Checked = false;
            this.coinToolStripButton.Checked = false;
            uncheckPlatforms();
        }

        private void addWolfy(object sender, EventArgs e)
        {
            this.currentStatus = AppState.AddingEnemy;
            tipoEnemigo = TipoEnemigo.Wolfy;

            this.flechaToolStripButton.Checked = false;
            this.vampyToolStripButton.Checked = false;
            //this.fantyToolStripButton.Checked = false;
            this.frankyToolStripButton.Checked = false;
            this.coinToolStripButton.Checked = false;
            uncheckPlatforms();
        }

        private void uncheckPlatforms()
        {
            tipo0ToolStripButton.Checked = false;
            tipo1ToolStripButton.Checked = false;
            tipo2ToolStripButton.Checked = false;
            tipo3ToolStripButton.Checked = false;
            tipo4ToolStripButton.Checked = false;
            tipo5ToolStripButton.Checked = false;
            tipo6ToolStripButton.Checked = false;
            tipo7ToolStripButton.Checked = false;
        }

        private void addPlatform(object sender, EventArgs e)
        {
            ToolStripButton item = sender as ToolStripButton;
            tipoPlataforma = (TilePlataforma)byte.Parse(item.Tag.ToString());

            this.flechaToolStripButton.Checked = false;
            this.vampyToolStripButton.Checked = false;
            //this.fantyToolStripButton.Checked = false;
            this.wolfyToolStripButton.Checked = false;
            this.frankyToolStripButton.Checked = false;
            this.coinToolStripButton.Checked = false;

            uncheckPlatforms();
            item.Checked = true;

            this.currentStatus = AppState.AddingPlatform;
        }

        private void currentLevelView_MouseDown(object sender, MouseEventArgs e)
        {
            byte tileX = (byte)(e.X / 32);
            byte tileY = (byte)(e.Y / 32);

            switch (this.currentStatus)
            {
                case AppState.AddingPlatform:
                    this.isDrawing = true;

                    plataformaActual = new Plataforma();
                    plataformaActual.TipoPlataforma = tipoPlataforma;
                    plataformaActual.X = tileX;
                    plataformaActual.Y = tileY;
                    plataformaActual.Longitud = 1;
                    plataformaActual.Direccion = Direccion.Horizontal;

                    this.currentLevel.Plataformas.Add(plataformaActual);

                    tileXIni = tileX;
                    tileYIni = tileY;

                    updateView();
                    break;
                case AppState.Normal:
                    this.seleccionado = getObjectAt(tileX, tileY);
                    this.currentLevelProp.SelectedObject = this.seleccionado;

                    tileXIni = tileX;
                    tileYIni = tileY;
                    break;
                default:
                    this.isDrawing = false;
                    break;
            }
        }

        private void currentLevelView_MouseMove(object sender, MouseEventArgs e)
        {
            byte tileX = (byte)(e.X / 32);
            byte tileY = (byte)(e.Y / 32);

            if (e.Button == MouseButtons.Left)
            {
                // Moviendo el ratón con el botón izquierdo...
                switch (this.currentStatus)
                {
                    case AppState.Normal:
                        if (seleccionado != null)
                        {
                            updateSeleccionado(tileX, tileY);
                        }
                        break;
                }
            }
            if (this.isDrawing)
            {
                updatePlataformaDibujada(tileX, tileY);
            }
        }

        private void updateSeleccionado(byte tileX, byte tileY)
        {
            bool actualizar = false;

            actualizar |= updateMoneda(tileX, tileY, actualizar);
            actualizar |= updateEnemigo(tileX, tileY, actualizar);
            actualizar |= updatePlataforma(tileX, tileY, actualizar);

            if (actualizar)
            {
                updateView();
            }
        }

        private bool updatePlataforma(byte tileX, byte tileY, bool actualizar)
        {
            Plataforma plataforma = seleccionado as Plataforma;
            if (plataforma != null)
            {
                if (plataforma.Direccion == Direccion.Horizontal)
                {
                    if (plataforma.Y != tileY)
                    {
                        plataforma.Y = tileY;
                        actualizar = true;
                    }
                }
                else
                {
                    if (plataforma.X != tileX)
                    {
                        plataforma.X = tileX;
                        actualizar = true;
                    }
                }
            }
            return actualizar;
        }

        private bool updateEnemigo(byte tileX, byte tileY, bool actualizar)
        {
            Enemigo enemigo = seleccionado as Enemigo;
            if (enemigo != null)
            {
                if (enemigo.TileVert != tileY)
                {
                    enemigo.TileVert = tileY;
                    actualizar = true;
                }
            }
            return actualizar;
        }

        private bool updateMoneda(byte tileX, byte tileY, bool actualizar)
        {
            Moneda moneda = seleccionado as Moneda;
            if (moneda != null)
            {
                if (moneda.X != tileX)
                {
                    moneda.X = tileX;
                    actualizar = true;
                }
                if (moneda.Y != tileY)
                {
                    moneda.Y = tileY;
                    actualizar = true;
                }
            }
            return actualizar;
        }

        private void updatePlataformaDibujada(byte tileX, byte tileY)
        {
            byte tileXMod, tileYMod;
            // Arrastrando para colocar una plataforma...
            if (Math.Abs(tileX - tileXIni) > Math.Abs(tileY - tileYIni))
            {
                tileXMod = tileX;
                tileYMod = tileYIni;
                plataformaActual.X = Math.Min(tileXMod, tileXIni);
                plataformaActual.Direccion = Direccion.Horizontal;
                plataformaActual.Longitud = (byte)(Math.Abs(tileX - tileXIni) + 1);
            }
            else
            {
                tileXMod = tileXIni;
                tileYMod = tileY;
                plataformaActual.Y = Math.Min(tileYMod, tileYIni);
                plataformaActual.Direccion = Direccion.Vertical;
                plataformaActual.Longitud = (byte)(Math.Abs(tileY - tileYIni) + 1);
            }
            updateView();
        }

        private void currentLevelView_MouseUp(object sender, MouseEventArgs e)
        {
            byte tileX = (byte)(e.X / 32);
            byte tileY = (byte)(e.Y / 32);

            switch (this.currentStatus)
            {
                case AppState.Normal:
                    this.currentLevelProp.SelectedObject = getObjectAt(tileX, tileY);
                    enableToolsTripButtons();
                    break;
                case AppState.AddingPlatform:
                    if (this.isDrawing && ((e.Button & MouseButtons.Left) == MouseButtons.Left))
                    {
                        // Fin de arrastre para colocar una plataforma...
                        updatePlataformaDibujada(tileX, tileY);
                    }
                    this.isDrawing = false;
                    updateView();
                    break;
                case AppState.AddingCoin:
                    Moneda moneda = new Moneda();

                    moneda.X = tileX;
                    moneda.Y = tileY;

                    this.currentLevel.Monedas.Add(moneda);
                    this.currentLevelProp.SelectedObject = moneda;

                    this.updateView();
                    break;
                case AppState.AddingEnemy:
                    Enemigo enemigo = new Enemigo();

                    enemigo.TileDer = tileX;
                    enemigo.TileIzq = tileX;
                    enemigo.TileVert = tileY;
                    enemigo.TipoEnemigo = tipoEnemigo;
                    enemigo.Velocidad = Velocidad.Lento;

                    this.currentLevel.Enemigos.Add(enemigo);
                    this.currentLevelProp.SelectedObject = enemigo;

                    this.updateView();
                    break;
            }
        }

        private object getObjectAt(byte tileX, byte tileY)
        {
            object selectedObject = null;

            foreach (Plataforma obj in this.currentLevel.Plataformas)
            {
                if (obj != null && obj.IsInTile(tileX, tileY))
                {
                    selectedObject = obj;
                }
            }

            if (selectedObject == null)
            {
                foreach (Moneda coin in this.currentLevel.Monedas)
                {
                    if (coin != null && coin.IsInTile(tileX, tileY))
                    {
                        selectedObject = coin;
                    }
                }
            }

            if (selectedObject == null)
            {
                foreach (Enemigo enem in this.currentLevel.Enemigos)
                {
                    if (enem != null && enem.IsInTile(tileX, tileY))
                    {
                        selectedObject = enem;
                    }
                }
            }

            return selectedObject;
        }

        private void borrarToolStripButton_Click(object sender, EventArgs e)
        {
            object seleccionado = this.currentLevelProp.SelectedObject;

            Moneda moneda = seleccionado as Moneda;
            if (moneda != null)
            {
                this.currentLevel.Monedas.Remove(moneda);
            }

            Enemigo enemigo = seleccionado as Enemigo;
            if (enemigo != null)
            {
                this.currentLevel.Enemigos.Remove(enemigo);
            }

            Plataforma plataforma = seleccionado as Plataforma;
            if (plataforma != null)
            {
                this.currentLevel.Plataformas.Remove(plataforma);
            }

            this.currentLevelProp.SelectedObject = null;
            updateView();
        }

        private void snapShotButton_Click(object sender, EventArgs e)
        {
            sfd.Filter = "PNG Files|*.png|All Files|*.*";
            sfd.Title = "Guardar preview";
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                this.currentLevelView.SavePreview(sfd.FileName);
            }
        }

        private void frmEditor_FormClosed(object sender, FormClosedEventArgs e)
        {
            foreach (Bitmap bmp in tiles.Values)
            {
                bmp.Dispose();
            }
            tiles.Clear();
        }

        private void mostrarSplashAlIniciarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.MostrarSplash = mostrarSplashAlIniciarToolStripMenuItem.Checked;
            Properties.Settings.Default.Save();
        }

        private void colorDeLaRejillaToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (colorDlg.ShowDialog() == DialogResult.OK)
            {
                Properties.Settings.Default.ColorRejilla = colorDlg.Color;
                Properties.Settings.Default.Save();
                this.currentLevelView.GridColor = colorDlg.Color;
            }
        }

        private void levelSelector_SelectedLevelChanged(object sender, EventArgs e)
        {
            currentLevel = levelSelector.SelectedLevel;
            initializeView();
        }

        private void mostrarRejillaToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.MostrarRejilla = mostrarRejillaToolStripMenuItem.Checked;
            currentLevelView.ShowGrid = mostrarRejillaToolStripMenuItem.Checked;
            Properties.Settings.Default.Save();
            updateView();
        }

        private void mostrarDesplazamientosToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.MostrarDesplazamientos = mostrarDesplazamientosToolStripMenuItem.Checked;
            currentLevelView.ShowArrows = mostrarDesplazamientosToolStripMenuItem.Checked;
            Properties.Settings.Default.Save();
            updateView();
        }

        private void guardarTodasLasVersionesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.levels != null)
            {
                if (sfd.ShowDialog() == DialogResult.OK)
                {
                    string basePath = System.IO.Path.GetDirectoryName(sfd.FileName);

                    foreach (CPUVersion value in System.Enum.GetValues(typeof(CPUVersion)))
                    {
                        string fileName = Path.GetFileNameWithoutExtension(sfd.FileName);
                        CodeGenerators.ICodeGenerator generator;

                        generator = CodeGenerators.CodeGeneratorFactory.CreateGenerator(Version.DameVersion(value));

                        FileStream stream = new FileStream(System.IO.Path.Combine(basePath, generator.GetFileName(fileName)), FileMode.Create, FileAccess.Write);
                        generator.CreateCode(levels, stream);

                        stream.Close();
                    }
                }
            }
        }

        private const int FIRST_BYTE_IN_Z80 = 0x4E89;

        private void cmdProbarNiveles_Click(object sender, EventArgs e)
        {
            string exeDir = Path.GetDirectoryName(Application.ExecutablePath);
            string filePath = Path.Combine(exeDir, "uwol.sna");
            string destPath = Path.Combine(exeDir, "tmp.sna");

            File.Copy(filePath, destPath, true);

            //string filePath = Path.Combine(exeDir, "game\\data\\rooms.dat");

            CodeGenerators.ICodeGenerator generator;
            generator = new ZXbinCodeGenerator();

            FileStream fileStream = new FileStream(destPath, FileMode.Open, FileAccess.ReadWrite);
            
            byte[] original = new byte[fileStream.Length];
            fileStream.Read(original, 0, (int)fileStream.Length);

            fileStream.Position = FIRST_BYTE_IN_Z80;
            
            generator.CreateCode(levels, fileStream);

            fileStream.Close();

            launch(destPath);
        }

        private static void launch(string path)
        {
            Process gameProcess;

            gameProcess = new Process();
            gameProcess.StartInfo.FileName = path;
            gameProcess.StartInfo.WorkingDirectory = Path.GetDirectoryName(path);
            gameProcess.Start();
        }

        private static void launchUWOL(string exeDir)
        {
            Process gameProcess;

            gameProcess = new Process();
            gameProcess.StartInfo.FileName = Path.Combine(exeDir, "game\\UWOL.exe");
            gameProcess.StartInfo.Arguments = "/w";
            gameProcess.StartInfo.WorkingDirectory = Path.Combine(exeDir, "game");
            gameProcess.Start();
        }

        private void cmdTestLevel_Click(object sender, EventArgs e)
        {
            string exeDir = Path.GetDirectoryName(Application.ExecutablePath);
            string filePath = Path.Combine(exeDir, "uwol.sna");
            string destPath = Path.Combine(exeDir, "tmp.sna");

            File.Copy(filePath, destPath, true);

            //string filePath = Path.Combine(exeDir, "game\\data\\rooms.dat");

            CodeGenerators.ICodeGenerator generator;
            generator = new ZXbinCodeGenerator();

            FileStream fileStream = new FileStream(destPath, FileMode.Open, FileAccess.ReadWrite);

            byte[] original = new byte[fileStream.Length];
            fileStream.Read(original, 0, (int)fileStream.Length);

            fileStream.Position = FIRST_BYTE_IN_Z80;

            Levels currentLevelList = new Levels();
            currentLevelList.Add(currentLevel);

            generator.CreateCode(currentLevelList, fileStream);

            fileStream.Close();

            currentLevelList.Clear();
            currentLevelList = null;

            launch(destPath);
        }

        private void pirámideToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frmPyramid frm = new frmPyramid();
            frm.Levels = levels;
            frm.Version = this.currentVersion;
            frm.Show();
        }
    }
}