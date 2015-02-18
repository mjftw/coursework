--
-- When cross building us --os=linux
--
-- A solution contains projects, and defines the available configurations
solution "CW1"
   configurations { "Release" }
      language "C++"
      includedirs{ ".\graphics\include", ".\graphics\GLFW3\include}
      libdirs{".\graphics\GLFW3\lib-mingw"}
      links{"mingw32", "glew32", "glfw3", "opengl32", "gdi32"}
 
   -- A project defines one build target
   project "demo"
      kind "ConsoleApp"
      files {"demo.exe"}
      
