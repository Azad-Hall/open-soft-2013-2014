using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ImageViewer.View
{
    /// <summary>
    /// Logique d'interaction pour ImageListView.xaml
    /// </summary>
    public partial class ImageListView : UserControl
    {
        public ImageListView()
        {
            InitializeComponent();
        }

				private void ImageListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
				{
					var listBox = sender as ListBox;
					listBox.ScrollIntoView(listBox.SelectedItem);

				}
    }
}
