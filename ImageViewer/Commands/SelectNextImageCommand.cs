using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using MVVMBase;
using ImageViewer.ViewModel;
using System.Windows.Forms;

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
            return true;
		}
		public static void SelectNextImageExecute(object o)
		{
		}
		#endregion

	}
}
