using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ImageViewer.ViewModel;
using MVVMBase;
using System.Collections.ObjectModel;
using ImageViewer.Model;

namespace ImageViewer.ViewModel
{
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
