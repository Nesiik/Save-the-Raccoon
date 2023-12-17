# Save The Raccon
"Simple" 2D platformer using c, [SDL2](https://github.com/libsdl-org/SDL/tree/SDL2), [SDL_image](https://github.com/libsdl-org/SDL_image/tree/SDL2), [SDL_ttf](https://github.com/libsdl-org/SDL_ttf/tree/SDL2) and [list](https://github.com/jamesamcl/list/tree/master) (see [external](external/) )
## Keys
Z(azerty)/W(qwerty) : Jump  
Q/A : Left  
D : right
## Compilation (tested on linux, not windows but should work)
You need git (if you want to clone) and cmake to be able to compile (i recommanded having sdl,sdl_image and sdl_ttf already installed)  
If you want to install sdl2 check [sdl2.sh](sdl2.sh/)  
If you don't cmake and you're running ubuntu (or ubuntu-based distribution) check [cmake.sh](cmake.sh/)
```sh
git clone https://github.com/Nesiik/Save-the-Raccoon &&
cd Save-the-Raccoon &&
mkdir build &&
cd build &&
cmake .. &&
cmake --build . &&
./projet
```
