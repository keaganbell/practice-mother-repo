targetname := wulkan
targetsrc := src\\windows_$(targetname).cpp
targetexe := $(targetname).exe

vulkan_libdir ?= $(wildcard C:/VulkanSDK/*/Lib)
ifeq ($(vulkan_libdir),)
	$(error Vulkan libs not found in default directory. Please specify vulkan_libdir=path/to/Lib)
endif

vulkan_incdir ?= $(wildcard C:/VulkanSDK/*/Include)
ifeq ($(vulkan_incdir),)
	$(error Vulkan includes not found in default directory. Please specify vulkan_incdir=path/to/Include)
endif

compile_flags := /WL /nologo /Foobj\\ /Zi /Od /Fdpdb\\compiler_$(targetname).pdb
compile_flags += /I$(vulkan_incdir)

linker_flags := /INCREMENTAL:NO /DEBUG
linker_flags += kernel32.lib user32.lib gdi32.lib
linker_flags += $(vulkan_libdir)\\vulkan-1.lib

builddir := bin\\debug
output := /OUT:$(builddir)\\$(targetexe) /PDB:pdb\\linker_$(targetname).pdb

.PHONY: $(targetname)
$(targetname): | obj pdb $(builddir)
	@cl $(compile_flags) $(targetsrc) /link $(linker_flags) $(output)

pdb:
	@mkdir $@

obj:
	@mkdir $@

$(builddir): | bin
	@mkdir $@

bin:
	@mkdir $@

.PHONY: clean
clean:
	@del /Q pdb\\*
	@del /Q obj\\*
	@del /Q $(builddir)\\*
