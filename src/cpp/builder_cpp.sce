function build_cpp()
	cppPath = get_absolute_file_path("builder_cpp.sce");

	CFLAGS = ilib_include_flag(cppPath);
	CFLAGS = CFLAGS + " " + ilib_include_flag(fullpath(cppPath + "../../python/include"));
	CFLAGS = CFLAGS + " -DDLL_EXPORTS";

	LDFLAGS = fullfile(cppPath + "../../python/libs/python38.lib");
	src_cpp = ["PyVar.cpp"];

	tbx_build_src("PyVar", src_cpp, "cpp", cppPath, "", LDFLAGS, CFLAGS);
endfunction

build_cpp();
clear build_cpp;