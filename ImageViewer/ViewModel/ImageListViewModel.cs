using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;
using ImageViewer.Model;
using MVVMBase;
using System.Diagnostics;
using System.Windows.Forms;
using System.Windows.Threading;

namespace ImageViewer.ViewModel
{
	class ImageListViewModel : ViewModelBase, IImageListViewModel
	{
		#region Fields
		private readonly ObservableCollection<ImageEntity> _imageListCollection;
		private ImageEntity _selectedImageEntity;
		private DispatcherTimer _clock;
		private string _status;
		private double _slideShowSpeed;
		private readonly Dispatcher _currentDispatcher;
		private int _selectedImageEntityIndex;
        private string _status_text = "Drag and Drop Images, then click Join Images";
		#endregion

		#region Properties
        public string StatusText
        {
            get
            {
                return _status_text;
            }
            set
            {
                _status_text = value;
                OnPropertyChanged("StatusText");
            }
        }
		public ObservableCollection<ImageEntity> ImageListCollection
		{
			get { return _imageListCollection; }
		}
		public ImageEntity SelectedImageEntity
		{
			get
			{
				return _selectedImageEntity;
			}
			set
			{
				_selectedImageEntity = value;
				OnPropertyChanged("SelectedImageEntity");
				SelectedImageEntityIndex = ImageListCollection.IndexOf(SelectedImageEntity)+1;
			}
		}
		public string Status
		{
			get
			{
				return _status;
			}
			set
			{
				_status = value;
				OnPropertyChanged("Status");
			}
		}
		public DispatcherTimer Clock
		{
			get
			{
				if (_clock == null) _clock = new DispatcherTimer(DispatcherPriority.Normal, this.CurrentDispatcher);
				return _clock;
			}
		}
		public bool IsClockTicking
		{
			get
			{


				return Clock.IsEnabled;
			}
			set
			{
				Clock.IsEnabled = value;
				OnPropertyChanged("IsClockTicking");
			}
		}
		public double SlideShowSpeed
		{
			get
			{
				return _slideShowSpeed;
			}
			set
			{
				_slideShowSpeed = value;
				Clock.Interval = TimeSpan.FromSeconds(SlideShowSpeed);
				OnPropertyChanged("SlideShowSpeed");
			}
		}
		public int ImageCount
		{
			get
			{
				return ImageListCollection.Count;
			}
		}
		public Dispatcher CurrentDispatcher
		{
			get
			{
				return _currentDispatcher;
			}
		}
		public int SelectedImageEntityIndex
		{
			get
			{
				return _selectedImageEntityIndex;
			}
			set
			{
				_selectedImageEntityIndex = value;
				OnPropertyChanged("SelectedImageEntityIndex");
			}
		}
		#endregion

		#region Constructor
		public ImageListViewModel() : this(new ImageRepository(), Dispatcher.CurrentDispatcher) { }
		public ImageListViewModel(ObservableCollection<ImageEntity> imageRepository, Dispatcher CurrentDispatcher)
		{
			_currentDispatcher = CurrentDispatcher;
			_imageListCollection = imageRepository;
			if (_imageListCollection.Count > 0)
			{
				_selectedImageEntity = _imageListCollection[0];
			}
			SlideShowSpeed = 5;
			Clock.Interval = TimeSpan.FromSeconds(SlideShowSpeed);
			Clock.Tick += Clock_Tick;

		}
		#endregion

		#region Methods
		private void Clock_Tick(object sender, EventArgs e)
		{
			var s = this;
			if (s.ImageListCollection != null && s.ImageListCollection.Count > 1)
			{
				if (s.ImageListCollection.IndexOf(s.SelectedImageEntity) < s.ImageListCollection.Count - 1)
				{
					s.SelectedImageEntity = s.ImageListCollection[s.ImageListCollection.IndexOf(s.SelectedImageEntity) + 1];
				}
				else
				{
					s.SelectedImageEntity = s.ImageListCollection[0];
				}
			}

		}
		#endregion
	}
}
