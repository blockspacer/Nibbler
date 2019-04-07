# Nibbler
Classic snake game, but also online multiplayer first-person 3D survival frogger, in C++ using SFML, SDL2, and OpenGL. (42 Silicon Valley)

<p float="left">
  <img src="https://github.com/ashih42/Nibbler/blob/master/Screenshots/sfml.png" width="280" />
  <img src="https://github.com/ashih42/Nibbler/blob/master/Screenshots/sdl.png" width="280" /> 
  <img src="https://github.com/ashih42/Nibbler/blob/master/Screenshots/opengl.png" width="285" />
</p>

## Prerequisites

You are on macOS with `clang++`, `make`, and `brew` installed.

## Installing

```
./setup.sh
```

## Running

### 1-Player Mode

```
./Nibbler -l 1 boardWidth boardHeight
```

### Local 2-Player Mode

```
./Nibbler -l 2 boardWidth boardHeight
```

### Online 2-Player Mode

#### Server

```
./Nibbler -s port boardWidth boardHeight
```

#### Client

```
./Nibbler -c ipAddress port
```

## Controls

### Gameplay Controls
* **Player 1:** `Left Arrow`, `Right Arrow` to turn.
* **Player 2:** `Numpad 4`, `Numpad 6` to turn.
* `R` to restart.
* `Escape` to terminate.

### Graphics Controls
1. `1` to select **SFML** visualizer.
2. `2` to select **SDL2** visualizer.
    * `G` to toggle grid.
3. `3` to select **OpenGL** visualizer.
    * `W` to toggle wireframe mode.
    * `S` to toggle shading.
    * `V` to toggle first/third person POV.
