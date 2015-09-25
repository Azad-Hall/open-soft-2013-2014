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
		private static ICommand _mouseMoveCommand;
		#endregion

		#region Properties
		public static ICommand MouseMoveCommand
		{
			get
			{
				if (_mouseMoveCommand == null)
				{
					_mouseMoveCommand = new RelayCommand(MouseMoveCommandExecute);
				}
				return _mouseMoveCommand;
			}
		}
		#endregion

		#region Methods
		/// <summary>
		/// handle mouse mouse
		/// </summary>
		/// <param name="o">IImageListViewModel</param>
		public static void MouseMoveCommandExecute(object o)
		{
			// do somethings
			var vm = o as IImageListViewModel;
			vm.Status = "Mouse is moving " + DateTime.Now.ToString();
		}
		#endregion
	}
}
