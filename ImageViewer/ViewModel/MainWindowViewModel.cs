using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ImageViewer.ViewModel;
using MVVMBase;
using System.Collections.ObjectModel;
using ImageViewer.Model;
using System.Windows.Threading;

namespace ImageViewer.ViewModel
{
    //public class StatusText : INotifyPropertyChanged
    //{
    //    private string _name = "Drag and Drop Images, then click Join Images";
    //    public string Name
    //    {
    //        get
    //        {
    //            return _name;
    //        }
    //        set
    //        {
    //            _name = value;
    //            OnPropertyChanged("Name");
    //        }
    //    }

    //    public event PropertyChangedEventHandler PropertyChanged;

    //    public void OnPropertyChanged(string PropertyName)
    //    {
    //        if (PropertyChanged != null)
    //            PropertyChanged(this, new PropertyChangedEventArgs(PropertyName));
    //    }
    //}
	class MainWindowViewModel : ViewModelBase, IMainWindowViewModel
	{
		#region Fields
		private readonly IImageListViewModel _imageListViewModel;
		private ObservableCollection<ImageEntity> _droppedFiles;
		#endregion

		#region Properties
		public IImageListViewModel imageListViewModel
		{
			get
			{
				return _imageListViewModel;
			}
		}
		public ObservableCollection<ImageEntity> DroppedFiles
		{
			get
			{
				if (_droppedFiles == null)
				{
					_droppedFiles = new ObservableCollection<ImageEntity>();
				}
				return _droppedFiles;
			}
		}
		public Dispatcher CurrentDispatcher { get; set; }
		#endregion

		#region Constructor
		public MainWindowViewModel():this(new ImageListViewModel()){}
		public MainWindowViewModel(IImageListViewModel imageListViewModel)
		{
			_imageListViewModel = imageListViewModel;
		}
		#endregion

	}
}
