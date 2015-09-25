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

			var mwvm = o as IMainWindowViewModel;
			var ImageNumber = mwvm.imageListViewModel.ImageListCollection.Count;
			if (mwvm.DroppedFiles != null)
			{
				foreach (var i in mwvm.DroppedFiles)
				{
					{
						mwvm.imageListViewModel.ImageListCollection.Add(i);
					}
				}
				// if the imageListCollection was empty , select the first image
				// and display it in the image display
				if(ImageNumber == 0 && mwvm.imageListViewModel.ImageListCollection.Count > 0 )
				{
					mwvm.imageListViewModel.SelectedImageEntity = mwvm.imageListViewModel.ImageListCollection[0];
				}
				// clear the temporary file buffer
				mwvm.DroppedFiles.Clear();
			}
		}
	}
}
