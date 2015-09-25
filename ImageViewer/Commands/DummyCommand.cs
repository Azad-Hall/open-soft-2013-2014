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
		private static ICommand _dummyCommand;
    public static ICommand DummyCommand
    {
      get{
        if(_dummyCommand==null)
        {
          _dummyCommand = new RelayCommand(DummyCommandExecute,DummyCommandCanExecute);
        }
        return _dummyCommand;
      }
    }
    public static bool DummyCommandCanExecute(object o){
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
    public static void DummyCommandExecute(object o)
    {
      // implement command
    }
	}
}
