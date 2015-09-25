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

namespace ImageViewer
{
	/// <summary>
	/// Logique d'interaction pour MainWindow.xaml
	/// </summary>
	/// 
	public partial class MainWindow : Window
	{
		public string[] parameters { get; set; }
		public MainWindow()
		{
			InitializeComponent();
			

		}
		protected override void OnInitialized(EventArgs e)
		{
			base.OnInitialized(e);
			// Variable declaration
			ImageListViewModel imageListViewModel;
			MainWindowViewModel mainWindowViewModel;
			// get the command line args.
			parameters = System.Environment.GetCommandLineArgs();
			var StringList = new ObservableCollection<ImageEntity>();
			// populate Stringlist
			if (parameters != null)
			{
				foreach (var parameter in parameters)
				{
					if (Uri.IsWellFormedUriString(parameter, UriKind.Absolute))
					{
						StringList.Add(new ImageEntity { ImageUrl = new Uri(parameter) });
						Debug.WriteLine(parameter);
					}
				}
			}
			imageListViewModel = new ImageListViewModel(StringList);
			mainWindowViewModel = new MainWindowViewModel(imageListViewModel);
			this.DataContext = mainWindowViewModel;
		}
		private void Window_Drop(object sender,DragEventArgs e)
		{
		//  BackgroundWorker backgroundWorker = new BackgroundWorker();
		//  backgroundWorker.DoWork += ExecuteThread;
		//  backgroundWorker.RunWorkerAsync(new WorkerHelper { sender = sender, e = e });
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
