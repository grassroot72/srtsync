# srtsync

This is a very simple program in c aiming to help anyone who wants to sync the 
subtitles with the sound track.

The code is very straight forward. The program only works with unix line 
endings (LF). It is very easy to adapt the code to work with msdos or macos 
line endings. So, before using this program, you need to convert the srt file
to unix line ending format (use dos2unix to convert, or simply open it by any
text editor which can do the conversion and save it).

For efficiency, I haven't added code to skip the UTF-8 BOM, so the srt file
to work with this program shoud be without UTF-8 BOM.

I put it in the public domain so every can use it and modify it.

Try for yourself!

Happy coding ^_^
