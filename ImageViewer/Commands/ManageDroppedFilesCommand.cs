using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using System.Windows.Forms;
using MVVMBase;

using System.Diagnostics;
using System.IO;
using ImageViewer.ViewModel;
using System.Collections.ObjectModel;
using ImageViewer.Model;
using System.Threading;

using System.ComponentModel;
using System.Windows.Threading;

namespace ImageViewer.Commands
{
	/// <summary>Manage image opening </summary>
	public partial class ApplicationCommands
	{
		private static ICommand _manageDroppedFilesCommand;

		public static ICommand ManageDroppedFilesCommand
		{
			get
			{
				if (_manageDroppedFilesCommand == null)
				{
					_manageDroppedFilesCommand = new RelayCommand(
								(o) => ManageDroppedFilesExecute(o), (o) => ManageDroppedFilesCanExecute(o));
				}
				return _manageDroppedFilesCommand;
			}
		}

		private static bool ManageDroppedFilesCanExecute(object o)
		{
			return true;
		}

		///<summary>manage dropped files</summary> 
		///<param name="o">IMainWindowViewModel</param>
		public static void ManageDroppedFilesExecute(object o)
		{
			// get the main window view model
			var mwvm = o as IMainWindowViewModel;
			// get the last image rank in the image list collection
			var ImageNumber = mwvm.imageListViewModel.ImageListCollection.Count;
			// if some files have been dropped
			if (mwvm.DroppedFiles != null)
			{
				// put each image file in the image list collection
				foreach (var i in mwvm.DroppedFiles)
				{
							// create a new image item async.
					
							var work = Dispatcher.FromThread(mwvm.imageListViewModel.CurrentDispatcher.Thread).BeginInvoke(new Action<ImageEntity>(mwvm.imageListViewModel.ImageListCollection.Add), DispatcherPriority.ApplicationIdle, i );
							work.Completed += delegate
							{
								if (mwvm.imageListViewModel.SelectedImageEntity == null)
								{
									mwvm.imageListViewModel.SelectedImageEntity = mwvm.imageListViewModel.ImageListCollection[0];
								}
							};
				}
			}
			// if the imageListCollection was empty , select the first image
			// and display it in the image display
			if (ImageNumber == 0 && mwvm.imageListViewModel.ImageListCollection.Count > 0)
			{
				mwvm.imageListViewModel.SelectedImageEntity = mwvm.imageListViewModel.ImageListCollection[0];
			}
			// clear the temporary file buffer
			mwvm.DroppedFiles.Clear();
		}
	}
}

