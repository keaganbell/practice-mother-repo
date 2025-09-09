target=webserver
targetexe=$(target).exe
exesrc="src\\windows_$(target).cpp"
htmldir="html"

openssllib=C:\\Program Files\\OpenSSL-Win64\\lib\\VC\\x64
inc=/I"C:\\Program Files\\OpenSSL-Win64\\include"
cflags=-WL /nologo /Foobj\\
linker_flags = /INCREMENTAL:NO ws2_32.lib
defines=-DHTML_DIR="\"$(htmldir)\""

ifeq ($(build), 1)
cflags += -DRELEASE_BUILD
linker_flags += /OUT:"bin\\release\\$(target).exe"
build_dir=bin\\release
linker_flags += "$(openssllib)\\MT\\libssl.lib"
linker_flags += "$(openssllib)\\MT\\libcrypto.lib"
else
cflags += /Zi
linker_flags += /DEBUG /OUT:"bin\\debug\\$(target).exe" /PDB:"pdb\\$(target).pdb"
build_dir=bin\\debug
linker_flags += "$(openssllib)\\MTd\\libssl.lib"
linker_flags += "$(openssllib)\\MTd\\libcrypto.lib"
endif

index=$(build_dir)\\html\\index.html
cert=$(build_dir)\\cert\\server.crt
key=$(build_dir)\\cert\\server.key

ifeq ($(opt), 1)
# small size
cflags += /O1
else ifeq ($(opt), 2)
# speed
cflags += /O2
else ifeq ($(opt), x)
# efficiency (some speed and size)
cflags += /Ox
else
cflags += /Od
endif

.PHONY: all
all: server $(index) $(cert) $(key)

.PHONY: server
server: | obj pdb $(build_dir)
	@cl $(cflags) $(defines) $(inc) /Fd"pdb\\debug.pdb" $(exesrc) /link $(linker_flags)

$(index): html\\index.html | $(build_dir)\\html
	@copy $< $@

$(build_dir)\\html: | $(build_dir)
	@mkdir $@

$(cert):cert\\server.crt | $(build_dir)\\cert
	@copy $< $@

$(key):cert\\server.key | $(build_dir)\\cert
	@copy $< $@

$(build_dir)\\cert: | $(build_dir)
	@mkdir $@

obj:
	@mkdir $@

pdb:
	@mkdir $@

$(build_dir): | bin
	@mkdir $@

bin:
	@mkdir $@

.PHONY: clean
clean:
	@rmdir /s /q "pdb"
	@rmdir /s /q "obj"
	@rmdir /s /q "bin"
