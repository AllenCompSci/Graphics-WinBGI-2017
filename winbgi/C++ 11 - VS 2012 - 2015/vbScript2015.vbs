Const DestinationFile = "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include\*.*"
Const SourceFile = "N:\Graphics\C++ 11 - VS 2012 - 2015\*.*"

Set fso = CreateObject("Scripting.FileSystemObject")
    'Check to see if the file already exists in the destination folder
    If fso.FileExists(DestinationFile) Then
        'Check to see if the file is read-only
        If Not fso.GetFile(DestinationFile).Attributes And 1 Then 
            'The file exists and is not read-only.  Safe to replace the file.
            fso.CopyFile SourceFile, "N:\Graphics\C++ 11 - VS 2012 - 2015\", True
        Else 
            'The file exists and is read-only.
            'Remove the read-only attribute
            fso.GetFile(DestinationFile).Attributes = fso.GetFile(DestinationFile).Attributes - 1
            'Replace the file
            fso.CopyFile SourceFile, "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include\", True
            'Reapply the read-only attribute
            fso.GetFile(DestinationFile).Attributes = fso.GetFile(DestinationFile).Attributes + 1
        End If
    Else
        'The file does not exist in the destination folder.  Safe to copy file to this folder.
        fso.CopyFile SourceFile, "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include\", True
    End If
Set fso = Nothing