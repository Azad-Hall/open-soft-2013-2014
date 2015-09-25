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

namespace ImageViewer.Commands
{
	/// <summary>Manage image opening </summary>
	public partial class ApplicationCommands
	{
		private static ICommand _openImageFileCommand;

		public static ICommand OpenImageFileCommand
		{
			get
			{
				if (_openImageFileCommand == null)
				{
					_openImageFileCommand = new RelayCommand(
							(o) => OpenImageFileExecute(o), (o) => OpenImageFileCanExecute(o));
				}
				return _openImageFileCommand;
			}
		}

		private static bool OpenImageFileCanExecute(object o)
		{
			return true;
		}

		///<summary>Open an image file</summary> 
		private static void OpenImageFileExecute(object o)
		{
			Debug.WriteLine("called");
			var ViewModel = o as IImageListViewModel;
			using (var ofd = new OpenFileDialog())
			{
				ofd.Filter = "Image files (*.jpg)|*.jpg|All Files (*.*)|*.*";
				ofd.RestoreDirectory = true;
				ofd.Multiselect = true;
				if (ofd.ShowDialog() == DialogResult.OK)
				{
					foreach (var File in ofd.FileNames)
					{
						ViewModel.ImageListCollection.Add(new Model.ImageEntity { ImageUrl = new Uri(File) });
					}

				}
			}
		}
	}
}
