using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;

namespace GDEdit
{
    public class RenderControl : System.Windows.Forms.UserControl
    {
        private IContainer components; // Required for designer support

        #region CreateParams
        /// <summary>
        ///     Overrides the control's class style parameters.
        /// </summary>
        protected override CreateParams CreateParams
        {
            get
            {
                Int32 CS_VREDRAW = 0x1;
                Int32 CS_HREDRAW = 0x2;
                Int32 CS_OWNDC = 0x20;
                CreateParams cp = base.CreateParams;
                cp.ClassStyle = cp.ClassStyle | CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
                return cp;
            }
        }
        #endregion CreateParams

        #region InitializeComponent()
        /// <summary>
        ///     Required for designer support.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            // 
            // SimpleOpenGlControl
            // 
            this.BackColor = System.Drawing.Color.Black;
            this.Size = new System.Drawing.Size(50, 50);
        }
        #endregion InitializeComponent()

        #region InitializeStyles()
        /// <summary>
        ///     Initializes the control's styles.
        /// </summary>
        private void InitializeStyles()
        {
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetStyle(ControlStyles.DoubleBuffer, false);
            this.SetStyle(ControlStyles.Opaque, true);
            this.SetStyle(ControlStyles.ResizeRedraw, true);
            this.SetStyle(ControlStyles.UserPaint, true);
        }
        #endregion InitializeStyles()


        public RenderControl()
        {
            InitializeStyles();
            InitializeComponent();
        }

        #region Draw()
        /// <summary>
        ///     Sends an see cref="UserControl.Invalidate"  command to this control, thus
        ///     forcing a redraw to occur.
        /// </summary>
        public void Draw()
        {
            this.Invalidate();
        }
        #endregion Draw()

        #region OnPaint(PaintEventArgs e)
        protected override void OnPaint(PaintEventArgs e)
        {
            if (this.DesignMode)
            {
                e.Graphics.Clear(this.BackColor);
                e.Graphics.Flush();
                return;
            }

            base.OnPaint(e);
        }
        #endregion OnPaint(PaintEventArgs e)

        #region OnPaintBackground(PaintEventArgs e)
        /// <summary>
        ///     Paints the background.
        /// </summary>
        /// <param name="e"></param>
        protected override void OnPaintBackground(PaintEventArgs e)
        {
        }
        #endregion OnPaintBackground(PaintEventArgs e)
    }
}
