using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Policy;

namespace ImageViewer.Model
{
	public class ImageEntity : MVVMBase.ViewModelBase
	{
		#region Fields
		private Uri _imageUrl;
		private Guid _id;
		#endregion
		#region Properties
		public Uri ImageUrl
		{
			get
			{
				if (_imageUrl == null) _imageUrl = new Uri(string.Empty,UriKind.RelativeOrAbsolute);
				return _imageUrl;
			}
			set
			{
				_imageUrl = value;
				OnPropertyChanged("ImageUrl");
			}
		}
		public Guid Id
		{
			get
			{
				return _id;
			}
		}
		#endregion
		#region Methods
		public ImageEntity()
		{
			_id = new Guid();
		}
		#endregion
	}
}
