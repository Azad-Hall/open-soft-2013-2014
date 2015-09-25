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
		private static ICommand _selectNextImageCommand;
		#endregion
		#region Properties
		public static ICommand SelectNextImageCommand
		{
			get
			{
				if (_selectNextImageCommand == null)
				{
					_selectNextImageCommand = new RelayCommand(SelectNextImageExecute, SelectNextImageCanExecute);
				}
				return _selectNextImageCommand;
			}
		}
		#endregion
		#region Methods
		public static bool SelectNextImageCanExecute(object o) 
		{
			if (o != null)
			{
				var vm = o as IImageListViewModel;
				return vm.ImageListCollection.IndexOf(vm.SelectedImageEntity) < vm.ImageListCollection.Count - 1;
			}
			else
			{
				return false;
			}
		}
		public static void SelectNextImageExecute(object o)
		{
			if (o != null)
			{
				var vm = o as IImageListViewModel;
				vm.SelectedImageEntity = vm.ImageListCollection[vm.ImageListCollection.IndexOf(vm.SelectedImageEntity) + 1];
			}
		}
		#endregion

	}
}
