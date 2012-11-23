file(GLOB optimizer_src *.cpp)
file(GLOB installers_src Installers/*.cpp)

add_library(mroptimizer ${optimizer_src} ${installers_src})
