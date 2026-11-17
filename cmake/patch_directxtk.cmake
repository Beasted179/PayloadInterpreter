# Patch applied to DirectXTK's CMakeLists.txt via FetchContent PATCH_COMMAND.
#
# DirectXTK compiles its HLSL shaders by running Src/Shaders/CompileShaders.cmd
# through `cmake -E env ... CompileShaders.cmd` (a bare script name, relying on
# the custom command's WORKING_DIRECTORY). Modern CMake no longer resolves a
# bare command name against the working directory, so the step fails with
# "no such file or directory" and the whole build stops.
#
# Rewrite that single invocation to an absolute path. WORKING_DIRECTORY is left
# untouched so the script still runs with Src/Shaders as its CWD. The replace is
# idempotent: after patching, the original "CompileShaders.cmd ARGS" substring is
# gone, so re-running the patch is a no-op. It also targets only the COMMAND line
# (the MAIN_DEPENDENCY reference already uses a full path and is not matched).
set(_dxtk_cmakelists "CMakeLists.txt")

file(READ "${_dxtk_cmakelists}" _dxtk_content)
string(REPLACE
    "CompileShaders.cmd ARGS"
    "\"\${PROJECT_SOURCE_DIR}/Src/Shaders/CompileShaders.cmd\" ARGS"
    _dxtk_content "${_dxtk_content}")
file(WRITE "${_dxtk_cmakelists}" "${_dxtk_content}")

message(STATUS "Patched DirectXTK CompileShaders.cmd invocation to use an absolute path.")
