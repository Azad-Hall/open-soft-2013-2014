   M S B A M L   `   `   `  ���� F  BImageViewer, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null;"clr-namespace:ImageViewer.CommandsImageViewer.Commands  \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089   ButtonStyle1  DropImageTextBlock  �� Property �� Value �� 
TargetType �� 
TargetType �� Padding J� SnapsToDevicePixels �� Triggers P� Property P� Value	 �� 
TargetName  ImageListContextMenu
 m� Command m� CommandParameter�� D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    :x,http://schemas.microsoft.com/winfx/2006/xaml     /commands"clr-namespace:ImageViewer.Commands  %  &	        &	      '�� =     '�� �    &	 �    �� �� .���   � ��$ 20(���   � ��$ Italic$��� " ��.���   �@J�SnapsToDevicePixels . ��   �@�OverridesDefaultStyle . ��   � �� �� " ��.R��� $��Border��$ 2q� . 
������ ._���  P�  �@��
IsSelected . . ��� $	 Border��  � ��
 �����P�  �@��
IsSelected .  . ��� $	 Border��  � ��
 �   �P�  � {� .  . ���   � ��
 ������� " ��.���   � �� .  �� .@�m� $��_Open images��$7
 1commands:ApplicationCommands.OpenImageFileCommand�� �� m� $	��Cut��$	
 Cut��m� $
��Copy��$

 Copy��m� $��Paste��$
 Paste��m� $��Delete��$9
 3commands:ApplicationCommands.DeleteImageFileCommand�� �� m� $��
Properties��