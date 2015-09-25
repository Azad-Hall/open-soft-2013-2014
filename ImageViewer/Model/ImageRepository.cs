using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;

namespace ImageViewer.Model
{
    class ImageRepository:ObservableCollection<ImageEntity>
    {
        public ImageRepository()
        {
					// design time datas
            Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Hydrangeas.jpg") });
            Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Chrysanthemum.jpg") });
            Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Koala.jpg") });
            Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Lighthouse.jpg") });
						Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Lighthouse.jpg") });
						Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Hydrangeas.jpg") });
						Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Chrysanthemum.jpg") });
						Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Koala.jpg") });
						Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Lighthouse.jpg") });
						Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Lighthouse.jpg") });
						Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Hydrangeas.jpg") });
						Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Chrysanthemum.jpg") });
						Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Koala.jpg") });
						Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Lighthouse.jpg") });
						Add(new ImageEntity { ImageUrl = new Uri(@"C:\Users\Public\Pictures\Sample Pictures\Lighthouse.jpg") });

        }
    }
}
