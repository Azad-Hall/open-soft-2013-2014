using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;
using ImageViewer.Model;
using System.Windows.Forms;
using System.Windows.Threading;

namespace ImageViewer.ViewModel
{
	public interface IImageListViewModel
	{
		 ObservableCollection<ImageEntity> ImageListCollection { get; }

		 ImageEntity SelectedImageEntity { get; set; }
		 string Status { get; set; }
		 DispatcherTimer Clock { get; }
		 bool IsClockTicking { get; set; }
	}
}
