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
using ImageViewer.ViewModel;
using System.Collections.ObjectModel;
using ImageViewer.Model;
using System.Diagnostics;
using System.Threading;
using System.ComponentModel;
using System.Windows.Threading;

namespace ImageViewer
{
	/// <summary>
	/// Logique d'interaction pour MainWindow.xaml
	/// </summary>
	/// 

    public class StatusText : INotifyPropertyChanged
    {
        private string _name = "Drag and Drop Images, then click Join Images";
        public string Name
        {
            get
            {
                return _name;
            }
            set
            {
                _name = value;
                OnPropertyChanged("Name");
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public void OnPropertyChanged(string PropertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(PropertyName));
        }
    }
	public partial class MainWindow : Window
	{
        public static StatusText obj;
		public string[] parameters { get; set; }
		public MainWindow()
		{
            obj = new StatusText();
			InitializeComponent();  
            txtName.DataContext = obj;
            new SplashWindow().ShowDialog();
		}
		protected override void OnInitialized(EventArgs e)
		{
			base.OnInitialized(e);
			// Variable declaration
			IImageListViewModel imageListViewModel;
			IMainWindowViewModel mainWindowViewModel;
			var StringList = new ObservableCollection<ImageEntity>();
			//// get the command line args.
			//parameters = System.Environment.GetCommandLineArgs();
			//
			//// populate Stringlist
			//if (parameters != null)
			//{
			//  foreach (var parameter in parameters)
			//  {
			//    if (Uri.IsWellFormedUriString(parameter, UriKind.Absolute))
			//    {
			//      StringList.Add(new ImageEntity { ImageUrl = new Uri(parameter) });
			//      Debug.WriteLine(parameter);
			//    }
			//  }
			//}
			imageListViewModel = new ImageListViewModel(StringList, Dispatcher.CurrentDispatcher);
			mainWindowViewModel = new MainWindowViewModel(imageListViewModel);
			this.DataContext = mainWindowViewModel;
		}
		private void Window_Drop(object sender, DragEventArgs e)
		{
			var w = sender as FrameworkElement;
			var dc = w.DataContext as IMainWindowViewModel;
			if (dc != null)
			{
				if (e.Data.GetDataPresent(DataFormats.FileDrop))
				{
					string[] droppedFilePaths =
					e.Data.GetData(DataFormats.FileDrop, true) as string[];

					foreach (string droppedFilePath in droppedFilePaths)
					{
						dc.DroppedFiles.Add(new ImageEntity { ImageUrl = new Uri(droppedFilePath, UriKind.RelativeOrAbsolute) });
					}
					Commands.ApplicationCommands.ManageDroppedFilesCommand.Execute(dc);
				}
			}
		}
	}
}
