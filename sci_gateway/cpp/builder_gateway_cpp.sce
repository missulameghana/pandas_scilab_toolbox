// This file is released under the 3-clause BSD license. See COPYING-BSD.

function builder_gw_cpp()
    gwPath = get_absolute_file_path("builder_gateway_cpp.sce");
    gw_table =[;
	"pyDf", "sci_pyDf", "cppsci";
    ];

    CFLAGS = ilib_include_flag(gwPath);
    CFLAGS = CFLAGS + " " + ilib_include_flag(fullpath(gwPath + "../../src/cpp"));
    CFLAGS = CFLAGS + " " + ilib_include_flag(fullpath(gwPath + "../../python/include"));
    CFLAGS = CFLAGS + " " + ilib_include_flag(fullpath(SCI + "modules/ast/includes"));
    LDFLAGS = LDFLAGS + " " + fullpath(gwPath + "../../src/cpp/libPyVar.lib");
    LDFLAGS = LDFLAGS + " " + fullpath(gwPath + "../../python/libs/python38.lib");
    gw_src_cpp = [
	"sci_pyDf.cpp";
    ];
    tbx_build_gateway("scithon_cpp", gw_table, gw_src_cpp, gwPath, "", LDFLAGS, CFLAGS);
    includes_src_c = ilib_include_flag(get_absolute_file_path("builder_gateway_cpp.sce") + "../../src/c");

    //adapt to compiler capabilities
    l = ilib_language();
    if ~isempty(grep(l, "cppsci")) then
        tbx_build_gateway("skeleton_cpp", ..
            ["cpp_find","sci_cpp_find", "csci"; ..
            "cpp_error", "sci_cpperror", "cppsci"; ..
            "cpp_foo", "sci_cppfoo", "cppsci"; ..
            "cpp_sum", "sci_cppsum", "cppsci"; ..
            "cpp_sub", "sci_cppsub", "cppsci"; ..
            "cpp_multiplybypi", "sci_cppmultiplybypi", "cppsci"], ..
            ["sci_cpp_find.cxx", "sci_cpperror.cpp", "sci_cppfoo.cpp", "sci_cppsum.cpp", "sci_cppsub.cpp", "sci_cppmultiplybypi.cpp"], ..
            get_absolute_file_path("builder_gateway_cpp.sce"), ..
            ["../../src/c/libcsum"], ..
            "", ..
            includes_src_c);
    else
        tbx_build_gateway("skeleton_cpp", ..
            ["cpp_find","sci_cpp_find", "csci"], ..
            ["sci_cpp_find.cxx"], ..
            get_absolute_file_path("builder_gateway_cpp.sce"), ..
            ["../../src/c/libcsum"], ..
            "", ..
            includes_src_c);
    end
endfunction

builder_gw_cpp();
clear builder_gw_cpp; // remove builder_gw_cpp on stack
