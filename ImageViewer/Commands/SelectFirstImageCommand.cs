using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using MVVMBase;
using ImageViewer.ViewModel;

namespace ImageViewer.Commands
{
	public static partial class ApplicationCommands
	{
		private static ICommand _selectFirstImageCommand;
		public static ICommand SelectFirstImageCommand
		{
			get
			{
				if (_selectFirstImageCommand == null)
				{
					_selectFirstImageCommand = new RelayCommand(SelectFirstImageExecute, SelectFirstImageCanExecute);
				}
				return _selectFirstImageCommand;
			}
		}
		public static bool SelectFirstImageCanExecute(object o)
		{
			
			if (o != null)
			{
				var vm = o as IImageListViewModel;
				if (vm.ImageListCollection==null || vm.ImageListCollection.Count <= 1 || vm.SelectedImageEntity == vm.ImageListCollection[0])
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

		public static void SelectFirstImageExecute(object o)
		{
			var vm = o as IImageListViewModel;
			vm.SelectedImageEntity = vm.ImageListCollection[0];
		}
	}
}
