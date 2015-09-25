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
		private static ICommand _clearImageFilesCommand;
    public static ICommand ClearImageFilesCommand
    {
      get{
        if(_clearImageFilesCommand==null)
        {
          _clearImageFilesCommand = new RelayCommand(ClearImageFilesExecute,ClearImageFilesCanExecute);
        }
        return _clearImageFilesCommand;
      }
    }
    public static bool ClearImageFilesCanExecute(object o){
      if (o != null)
			{
				var vm = o as IImageListViewModel;
				if (vm.ImageListCollection != null & vm.ImageListCollection.Count > 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		
    }
    public static void ClearImageFilesExecute(object o)
    {
      var vm = o as IImageListViewModel ;
			if (vm != null & vm.ImageListCollection != null)
			{
				vm.ImageListCollection.Clear();
			}
    }
	}
}
