cflags=-WL /Zi /Fdpdb\\ /Foobj\\
inc=/I"C:\Program Files\Khronos Group"
common_link = /INCREMENTAL:NO  opengl32.lib
exe_link = user32.lib gdi32.lib kernel32.lib $(common_link)

.PHONY: executable
executable: | obj pdb
	cl $(cflags) $(inc) src/win64_wgl.cpp /link $(exe_link)

obj:
	mkdir obj

pdb:
	mkdir pdb
