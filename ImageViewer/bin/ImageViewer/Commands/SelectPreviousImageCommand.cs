using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using MVVMBase;
using ImageViewer.ViewModel;

namespace ImageViewer.Commands
{
	public partial class ApplicationCommands
	{
		#region Fields
		private static ICommand _selectPreviousImageCommand;
		#endregion
		#region Properties
		public static ICommand SelectPreviousImageCommand
		{
			get
			{
				if (_selectPreviousImageCommand == null)
				{
					_selectPreviousImageCommand = new RelayCommand(SelectPreviousImageExecute, SelectPreviousImageCanExecute);
				}
				return _selectPreviousImageCommand;
			}
		}
		#endregion
		#region Methods
		public static bool SelectPreviousImageCanExecute(object o) 
		{
			if (o != null)
			{
				var vm = o as IImageListViewModel;
				return vm.ImageListCollection.IndexOf(vm.SelectedImageEntity) > 0;
			}
			else
			{
				return false;
			}
		}
		public static void SelectPreviousImageExecute(object o)
		{
			var vm = o as IImageListViewModel ;
			vm.SelectedImageEntity = vm.ImageListCollection[vm.ImageListCollection.IndexOf(vm.SelectedImageEntity) - 1];
		}
		#endregion

	}
}
