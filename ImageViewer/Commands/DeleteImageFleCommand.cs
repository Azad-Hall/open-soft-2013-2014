using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MVVMBase;
using System.Windows.Input;
using System.Collections.ObjectModel;
using ImageViewer.Model;
using ImageViewer.ViewModel;
using System.Diagnostics;

namespace ImageViewer.Commands
{
	public partial class ApplicationCommands
	{
		static ICommand _deleteImageFileCommand;
		public static ICommand DeleteImageFileCommand
		{
			get
			{
				if (_deleteImageFileCommand == null)
				{
					_deleteImageFileCommand = new RelayCommand(
					p => DeleteFileCommandExecute(p),
					p => DeleteFileCommandCanExecute(p)
						);
				}
				return _deleteImageFileCommand;
			}
		}

		static private bool DeleteFileCommandCanExecute(object p)
		{
			if (p != null)
			{
				return ((IImageListViewModel)p).SelectedImageEntity != null;
			}
			else
			{
				return false;
			}
		}

		static private void DeleteFileCommandExecute(object p)
		{
			Debug.WriteLine(p);
			var k = (IImageListViewModel)p;
			var index = k.ImageListCollection.IndexOf(k.SelectedImageEntity);
			Debug.WriteLine(k);
			Debug.WriteLine(k.ImageListCollection.ToString());
			Debug.WriteLine(k.SelectedImageEntity.ToString());
			if (k.ImageListCollection.Contains(k.SelectedImageEntity))
			{
				k.ImageListCollection.Remove(k.SelectedImageEntity);
			}
			if (index+1 < k.ImageListCollection.Count && k.ImageListCollection[index] != null )
			k.SelectedImageEntity= 	k.ImageListCollection[index];

		}
	}
}