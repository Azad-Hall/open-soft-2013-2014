   M S B A M L   `   `   `  ����F  BImageViewer, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null;"clr-namespace:ImageViewer.CommandsImageViewer.Commands  5      \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089   ButtonStyle1  �� Property �� Value  DropImageTextBlock �� 
TargetType �� 
TargetType �� Padding J� SnapsToDevicePixels �� Triggers P� Property P� Value	 �� 
TargetName  ImageListContextMenu
 m� Command m� CommandParameter�� 5      D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    :x,http://schemas.microsoft.com/winfx/2006/xaml     5      /commands"clr-namespace:ImageViewer.Commands  5      %�  &	        &	 �     '��     '�� �    &	 �    �� 5      .�5   
   ��   � ��$ 3q�5      �� 5	   
     � ��$
 Bold"�5
      �� 5   
     � ��$	 100��5      5      �� 5      .�5   
   ��   � ��$ 20(�5      �� 5   
     � ��$ Italic$�5      5      �� 5      " ��6   .�5   
   ��   �@J�SnapsToDevicePixels . 5      �� 5   
     �@�OverridesDefaultStyle . 5      �� 5   
     � �� 5      �� 5      " ��6"   .R�5      �� $��Border��6   $ 2q�5       . 
������ ._�5       �� 5!       5"      P� 5#       �@��
IsSelected . . �5%      �� $	 Border��6%     � ��
 �����5'   %   5(      P� 5)       �@��
IsSelected .  . �5+      �� $	 Border��6%     � ��
 �   �5-   %   5.      P� 5/       � {� .  . �51      ��   � ��
 �����52   %   53      55      57      58      �� 59      " ��6   .�5:   
   ��   � �� .  5;      5<      �� 5=      .@�5>   
   m� $��_Open images��6   $7
 1commands:ApplicationCommands.OpenImageFileCommand��5?       5@      �� m� 5A   
   $	��Cut��6   $	
 Cut��5B      m� 5C   
   $
��Copy��6   $

 Copy��5D      m� 5E   
   $��Paste��6   $
 Paste��5F      m� 5G   
   $��Delete��6   $9
 3commands:ApplicationCommands.DeleteImageFileCommand��5H       5I      �� 5J      m� 5K   
   $��
Properties��6   5L      5M      