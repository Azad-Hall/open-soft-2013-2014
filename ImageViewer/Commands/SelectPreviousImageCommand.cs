using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using MVVMBase;
using ImageViewer.ViewModel;
using System.IO;
using System.IO.Pipes;
using System.Threading;
using System.Diagnostics;

namespace ImageViewer.Commands
{
    public class ProgramPipe
    {
        public void ThreadStartServer(object o)
        {
            // Create a name pipe
            using (NamedPipeServerStream pipeStream = new NamedPipeServerStream("CommPipe"))
            {
                Console.WriteLine("[Server] Pipe created {0}", pipeStream.GetHashCode());

                // Wait for a connection
                pipeStream.WaitForConnection();
                MainWindow.obj.Name += "\n[Server] Pipe connection established";
                using (StreamReader sr = new StreamReader(pipeStream))
                {
                    string temp;
                    // We read a line from the pipe and print it together with the current time
                    var ViewModel = o as IImageListViewModel; 
                    while ((temp = sr.ReadLine()) != "End Connection")
                    {
                        if (temp != null)
                            MainWindow.obj.Name += "\n" + temp;
                    }
                    MainWindow.obj.Name += "\n[Server] Connection terminated";
                }
            }
        }

        public static void RunMain(object o)
        {
            ProgramPipe Server = new ProgramPipe();

            Thread ServerThread = new Thread(() => Server.ThreadStartServer(o));
            ServerThread.Start();

        }
    }

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
            return true;
		}
		public static void SelectPreviousImageExecute(object o)
		{
            //ProgramPipe.RunMain(o);
            var vm = o as IImageListViewModel;
            //string args = "";
            
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(@"abc.txt"))
            {
                foreach (var imageentity in vm.ImageListCollection)
                {
                    file.WriteLine(imageentity.ImageUrl.ToString().Substring(8));
                }
            }
            
            var process = System.Diagnostics.Process.Start("OpenSoft.exe");
            process.WaitForExit();
            Console.WriteLine(Path.Combine(Environment.CurrentDirectory, "polygons.jpg"));
            vm.ImageListCollection.Add(new Model.ImageEntity { ImageUrl = new Uri(Path.Combine(Environment.CurrentDirectory, "polygons.jpg") )});
            vm.SelectedImageEntity = vm.ImageListCollection.Last();
        }
		#endregion

	}
}
