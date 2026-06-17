[Opticus-README.md](https://github.com/user-attachments/files/29032188/Opticus-README.md)
# Opticus

*An experimental 2D engine built around a custom non-Euclidean vision system.*

Opticus began as a game, but its real subject is **vision**. The idea: cover the world in grids that can **bend, move, and teleport** in response to sight-rays cast by whatever is looking. The player casts rays and sees a real rendered image of a world whose geometry doesn't have to stay flat or continuous — space can fold back on itself, wrap, or jump. The same machinery generalizes: an NPC could cast the same kind of rays against a *different* grid and "see" objects or creatures instead of pixels.

The vision system was my own design — I worked out the ray casting, the grid deformation, and the rendering largely from scratch. Most of the shader code in the project exists to make that system run: tracing many rays per observer and resolving them into an image.

## How it works (the short version)

- The world is represented as grids laid over space, rather than as a single fixed plane.
- An observer casts rays; those rays sample the grids, which can be transformed — bent, moved, teleported — along the way.
- For the player, the resolved rays become pixels: a rendered view of a non-Euclidean space.
- The design is observer-agnostic. Swap what the rays sample, and the same system answers "what can this entity see?"

## Tech

C++ with SFML, GLSL shaders for the ray/vision work, built as a Visual Studio solution. The repo also carries a copy of my [ACECS](https://github.com/oniontherock/ACECS-Engine) engine layer.

## Status

An unfinished, experimental prototype. The vision system worked, but it was too computationally heavy to build an actual game on top of — which is exactly what led me to rebuild the project, without the non-Euclidean vision, as [Firebrand](https://github.com/oniontherock/Firebrand). I'm keeping Opticus public because the vision experiment is the interesting part.
