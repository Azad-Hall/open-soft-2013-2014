using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ImageViewer.ViewModel;
using MVVMBase;
using System.Windows.Input;

namespace ImageViewer.Commands
{
	public partial class ApplicationCommands
	{
		private static ICommand _selectLastImageCommand;
		public static ICommand SelectLastImageCommand
		{
			get
			{
				if (_selectLastImageCommand == null)
				{
					_selectLastImageCommand = new RelayCommand(SelectLastImageExecute, SelectLastImageCanExecute);
				}
				return _selectLastImageCommand;
			}
		}
		public static bool SelectLastImageCanExecute(object o)
		{
			if (o != null)
			{
				var vm = o as IImageListViewModel;

				if (vm.ImageListCollection== null || vm.ImageListCollection.Count <= 1 || vm.SelectedImageEntity == vm.ImageListCollection.Last())
				{
					return false;
				}
				else
				{
					return true;
				}


			}
			else
			{
				return false;
			}
		}
		public static void SelectLastImageExecute(object o)
		{
			var vm = o as IImageListViewModel;
			vm.SelectedImageEntity = vm.ImageListCollection.Last();
		}
	}
}
