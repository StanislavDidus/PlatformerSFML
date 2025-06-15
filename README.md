# PlatformerSFML

My simple platformer where main goal is to collect coins and get to the end of the level
You can pick up special items to change state of the player and unlock new abilities

Resources:
SFML library
Tmxlite by Matt Marchant

Controls:
AD - movement
Space - Jump
S - Crouch(Only in BIG or FIRE form)
E - Shoot fire balls(Only if FIRE form)

Debug buttons:
G - BIG form
F - Fire form

How to use:
- Install the archive
- Unpack it in any folder
- Open SFMLPlatformer.exe file

  This game uses features like:
  - State machine(to manage states of the player and the game itself)
  - QuadTree(for better perfomance of the collision system)
  - TileMapEditor(to easily load and change tilemaps)
  - VertexArray(to render tiles effectively)
  - EventBus(to manage messages and events between game and player)
  - Animator(allows to easily add new animations to sprite, supports frameAnimations and positionAnimations)
  - RenderQueue(renders sprites and tiles in a selected order)
