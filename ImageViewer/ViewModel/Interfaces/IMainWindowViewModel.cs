using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;
using ImageViewer.Model;

namespace ImageViewer.ViewModel
{
	public interface IMainWindowViewModel
	{
		IImageListViewModel imageListViewModel { get; }
		ObservableCollection<ImageEntity> DroppedFiles { get; }
	}
}
