   M S B A M L   `   `   `  ���� F  BImageViewer, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null)clr-namespace:ImageViewerImageViewer  =#clr-namespace:ImageViewer.ViewModelImageViewer.ViewModel  ;"clr-namespace:ImageViewer.CommandsImageViewer.Commands  R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35X;clr-namespace:System.Windows.Threading;assembly=WindowsBaseSystem.Windows.Threading $    ImageViewer.View.ImageListView   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    )localclr-namespace:ImageViewer    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     @d2http://schemas.microsoft.com/expression/blend/2008     2vm#clr-namespace:ImageViewer.ViewModel     7commands"clr-namespace:ImageViewer.Commands     Q	threading;clr-namespace:System.Windows.Threading;assembly=WindowsBase     -     >�Name$  ImageListViewInstance��.�� � 	Resources  
  mytrig �� Property �� Value �� Triggers P� Property P� Value  ImageListDataTemplate � ToolTip �� DecodePixelHeight	 �� CacheOption
 �� CreateOptions �� 	UriSource  ExpanderControlDataTemplate � DataContext��%d  &	        &	 o     &	     �� .���  � ��$ _Fit Screen�� P�  �@d�	IsChecked$
 TrueY�. ���  � ��$ _Real Width���� .R��� $��4q�$	��100��.���� $��Uniform=� �� *
ImageUrl+���� $ 90��$	 OnLoad=�$
 DelayCreation=� �� *
ImageUrl+�� .R�� $
��Auto��# z $
��Auto��$��1,1,1,1q�
���  ��.���  � RowDefinitions �� $��* ��� $
��Auto �.+�� -    �Name$ scrollViewer1��$��0������ .���  � ��
 ����� ��  �� Binding ��  �� ElementName$ BackgroundButton�� �� Path$ 	IsChecked� �� Value True.a���  � ��
 �   � �� $
��Auto ��� $��* �.+��� -    ��Name$ 	ImageName��$��0��
���   �
��� ���$	��Top=�$��Stretch=� J� IsManipulationEnabled .  �� 
IsReadOnly . ���� *SelectedImageEntity.ImageUrl+�� -    ��Name$ ImageScrollViewer��$��1������ .���  � ��$
 Auto=���  � ��$
 Auto=� ��  �� $ WidthButton��$ 	IsChecked� True.a���  � ��$ Disabled=���  � ��$ Disabled=�.��� .+���   DropImageTextBlock#��[ $��50q����	#FFB4B4B4$��Center=�$��Center=�.��" Drop some image files to display�� -    ��Name$ ImageDisplayer��$��Uniform=�$��Center=�$��Center=�}�.  ���� *SelectedImageEntity.ImageUrl+�� -    ��Name$ ControlSection��$
��Auto��$��1�� �� Orientation$ Vertical=�$��Bottom=�.+�9� -    9�Name$ ControlSectionExpander�� 9� 
IsExpanded . #���  $��Bottom=�.���� .+��� -    ��Name$ ImageListBox��
���   �$	��110��  ImageListContextMenu � ContextMenu# [ $��Stretch=���.  #��[  . $ �� IsSynchronizedWithCurrentItem$
 TrueY����� *ImageListCollection+ �� SelectedItem �� *SelectedImageEntity+���� .R��� $ 
Horizontal=��� $��Center=�$ 
Horizontal=�.+��� $��_First��  ButtonStyle1#���  $:��4commands:ApplicationCommands.SelectFirstImageCommand������ �� #���  $=��7commands:ApplicationCommands.SelectPreviousImageCommand��$��	_Previous������ d� $/��)commands:ApplicationCommands.DummyCommand��  d� 	IsChecked  �� *IsClockTicking+���� ���� .���  � ��$ 3q���  � ��$
 Bold"���  � ��$	 100����  � ��$ _Play�� P�  �@d�	IsChecked$
 TrueY�. ���  � ��$ _Pause���� #���  $9��3commands:ApplicationCommands.SelectNextImageCommand��$��_Next������ �� #���  $9��3commands:ApplicationCommands.SelectLastImageCommand��$��_Last������ �� $y�Hidden=�{�.  
�������#���  d� -	   ! d�Name$! WidthButton��$	��100��$/��)commands:ApplicationCommands.DummyCommand��#���   ���� d� -
   $! BackgroundButton��$	��100��$/��)commands:ApplicationCommands.DummyCommand������ ���� .���  � ��$ _Dark Bg�� P�  �@d�	IsChecked$
 TrueY�. ���  � ��$ 	_Light Bg���� -   " ��Name$" SlideshowSpeedButton��$	��100��# � Minimum$# 1Q�$ Slideshow speed��$ � Maximum$$ 30Q�% �� IsSnapToTickEnabled% . & �� TickFrequency$& 1Q�' � Value' �� *SlideShowSpeed+�� ���� $ SlideshowSpeedButton��$ Value�