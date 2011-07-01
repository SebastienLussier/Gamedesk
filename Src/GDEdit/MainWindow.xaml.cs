using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using AvalonDock;
using System.Windows.Interop;
using System.Threading;
using System.Globalization;
using System.Windows.Forms;
using System.Windows.Forms.Integration;
using System.ComponentModel;
using System.Reflection;
using Microsoft.CSharp;
using System.CodeDom.Compiler;
using System.Reflection.Emit;
using System.Linq.Expressions;
using System.Runtime.InteropServices;

using GameEdit;

namespace GDEdit
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public static class CompositionTargetEx
        {
            private static TimeSpan _last = TimeSpan.Zero;
            private static event EventHandler<RenderingEventArgs> _FrameUpdating;
            public static event EventHandler<RenderingEventArgs> FrameUpdating
            {
                add
                {
                    if (_FrameUpdating == null)
                        CompositionTarget.Rendering += CompositionTarget_Rendering;

                    _FrameUpdating += value;
                }

                remove
                {
                    _FrameUpdating -= value;
                    if (_FrameUpdating == null)
                        CompositionTarget.Rendering -= CompositionTarget_Rendering;
                }
            }

            static void CompositionTarget_Rendering(object sender, EventArgs e)
            {
                RenderingEventArgs args = (RenderingEventArgs)e;
                if (args.RenderingTime == _last) return;

                _last = args.RenderingTime;
                _FrameUpdating(sender, args);
            }
        }

        public class SimpleTest
        {
            public float a { get; set; }
            public float b { get; set; }
            public float c { get; set; }
        }

        public class SimpleTestA : SimpleTest
        {
            public float d { get; set; }
        }

        public class SimpleTestB : SimpleTest
        {
            public float e { get; set; }
        }

        public MainWindow()
        {
            Thread.CurrentThread.CurrentCulture = CultureInfo.CreateSpecificCulture("en-US");
            Thread.CurrentThread.CurrentUICulture = new CultureInfo("en-US");

            InitializeComponent();

            Uri uri = new Uri("DataTemplates.xaml", System.UriKind.Relative);
            ResourceDictionary resourceDico = (ResourceDictionary)System.Windows.Application.LoadComponent(uri);
            Resources.MergedDictionaries.Add(resourceDico);

            EditorLink.Instance.Init(this.renderControl.Handle, Resources);

            this.renderControl.Paint += new PaintEventHandler(simpleOpenGlControl_Paint);
            CompositionTargetEx.FrameUpdating += new EventHandler<RenderingEventArgs>(CompositionTargetEx_FrameUpdating);

            List<ObjectLink> entityList = new List<ObjectLink>();
            entityList.Add(EditorLink.Instance.SpawnEntity(-10.0f, 0, 0));
            entityList.Add(EditorLink.Instance.SpawnEntity(10.0f, 0, 0));

            SimpleTest[] obj = new SimpleTest[2];
            obj[0] = new SimpleTestA();
            obj[1] = new SimpleTestB();
            myPropertyGrid.SelectedObjects = entityList.ToArray();
            propertygrid2.SelectedObjects = entityList.ToArray();
           // o.PropertyChanged += new PropertyChangedEventHandler(o_PropertyChanged);
        }

        void o_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            myPropertyGrid.SelectedObject = sender;
        }

        void CompositionTargetEx_FrameUpdating(object sender, RenderingEventArgs e)
        {
            renderControl.Invalidate();
        }
        
        void simpleOpenGlControl_Paint(object sender, PaintEventArgs e)
        {
            EditorLink.Instance.Tick();
        }

        private void OnMouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            //GameEdit.Bidon.OnViewportMouseDown(e.Button, e.X, e.Y);
        }
    }
}
