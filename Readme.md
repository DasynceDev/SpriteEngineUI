# RUN THE RUNME.sh FILE!!!

# 🚀 Welcome To Sprite Engine

Welcome to Sprite Engine, a user-friendly **2D Game Engine** designed for solo developers who want a straightforward yet powerful tool.

## Table of Contents
1. [Welcome To Sprite Engine](#welcome-to-sprite-engine)
2. [Tools Used](#tools-used)
3. [Features](#features)
4. [Enhancements](#enhancements)
5. [Quick overview](#quick-overview)
6. [Getting Started](#getting-started)
7. [The Engine](#the-engine)

## Tools Used
- **SFML**
- **Dear ImGui**
- **CMake**
- **Rene**
- **TheCherno** 

## Features
- **UI Editor**
- **Scene Creation**
- **Box Collision Detection**
- **Parent-Child Collision Stopping**
- **Physics**
- **Animations (supported by the editor)**
- **Scripting with C++** (Probably not the scripting you think, look in [Scripting](#scripting))
- **Project Creator**
- **Automatic Project Builder**
- **Lighting**
- **Prefabs**
- **Physics Simulation in editor**

## Enhancements
- **Sound Manager**
- **Additional C++ Functions**
- **Cleaner Design**
- **Box Collider Rotation**

## How to build
- ❗ **MAKE SURE TO RUN THE RUNME.sh file**
  - This will create a Build folder, run cmake and build the C++ files.
  - It will also download the required libraries using github submodules. 
  - It creates also a special Build folder in Engine/Template/ so the engine does not crash when creating a new project, the **Engine/Template/Build directory is REQUIRED!!!**

- ⚠️ **BUILDING IN RELEASE** If you want to build in **RELEASE**, you need to go into the top CMake file and change the `CMAKE_BUILD_TYPE` to Release, CMake doesn't update it in their cache when you change it in Visual Studio to Release, so it will copy Debug DLL's in the Release folder on the costum POST_BUILD command. It's **DEBUG** by default, you don't need to do anything if you want to build in DEBUG.

## How do i start a game? 
- ❗ Its **REQUIRED** to download the Core+Editor (SpriteEngineUI) and open the game with the editor; this will set the path to the core library in the game project. If you try to build the game without opening it in the Editor once, it won't work! 
  - Build the SpriteEngine with the instruction above. 
  - You can open the game in the editor and click on the top right button to play the game. 
- **COMPILING** the game on the first time will take quite alot of time, the editor freezes while the game runs, and while the game builds. 

## Getting Started

📝 **Note:** This file may be outdated as I am working solo on this engine.

To begin, clone this github repository. And build it with the instruction: (It's really just one command)

(Outdated)
Watch an introduction to the engine on [YouTube](https://www.youtube.com/watch?v=pnCD5dKhpmg).

1. **Create a Project**
   - Click the `create` button to initiate a new project.
   - In the file dialog, choose the project location and press the `+` button.
   - All files from the **template** folder will be copied to the selected project location.
   - I suggest opening the user project that got generated.

   *Tip:* Pressing `build` or `CTRL + B` in the engine will automatically build the project so you can execute it directly in the editor.

2. **How Do I Script My Sprites?**

    The engine calls the `update` and `start` methods of the `Game` class (⚠️The game class is NOT a core file!). If you create a new script, you CAN invoke the `update` and `start` method from the script within the `Game` class. You have complete control over the calling of the Scripts. They are basic C++ classes. If you wish to share instances, simply pass them in the `update` methods or set a pointer to them in the `start` method. To access a sprite, call `this->config.ptr_sprites->GetByName("name");` This method returns a pointer to the sprite in the `SpriteRepository`. You can now perform various operations with the sprite. Please be aware that the engine still has bugs.

   - Open the project that got generated by the editor.  
   - Right-click the hierarchy window and select **New -> Sprite**.
   - In the game engine, right-click on the asset folder and choose **New -> C++ Script** to create a script with **update** and **start** functions.
   - Create a new **Sprite ptr** in your class:

     ```C++
     class Game : public spe::IScript
     {
     private:
         Sprite* m_sprite;
     }
     ```

   - Assign an address to the **Sprite ptr** using:

     ```C++
     void Game::start()
     {
         this->m_sprite = this->config.ptr_sprites->getSpriteWithName("name");
     }
     ```

   - Change sprite properties in the **update** function:

     ```C++
     void Game::update()
     {
         if(Input::onKeyHold(KeyBoardCode::A))
         {
             this->m_sprite.transform.setPosition(spe::Vector(x, y));
         }
     }
     ```

3. **How do the sprites get managed?**
   - All sprites are managed by an object called `SpriteRepository`.
   - Add sprites to the scene using the repository:

     ```C++
     void Game::start()
     {
       Sprite* spr = new Sprite();
       this->config.ptr_sprites.add(spr);
     }
     ```

   - Activate necessary components for basic collision detection:

     ```C++
     void Game::start()
     {
       Sprite* spr = new Sprite();
       spr->collider.exist = true;
     }
     ```

# Scripting

So scripting probably doesn't work as you think. In this game engine you don't "attach" scripts to an Entity, you can create a Script that can be used as you want. When you create a script it automatically creates an `Update()` and `Start()` function, but these functions don't get called. So you probably already read about the `Game` class; the Game class is kind of the Core class of your game. You can theoretically program the whole game in the `Game` class. I would recommend creating `Scripts` though, as you can call these `Scripts` in your Game class. So that's where the difference is. You have control over where to call these `Scripts` and you can do whatever you want. A `Script` will not be destroyed; it's just a basic C++ class. I would highly recommend you to take a look into my own game [DasynceSE](https://github.com/jkatsanis/DasynceSE.git) - on Github. 

**Scripting -> ECS**
📝 Please note that this is *NOT* an ECS. The scripts are not components; they are just basic C++ classes. 

# The Engine

**Picture of the Engine**
![Engine](Github/Game.PNG)

This game is currently being developed. GitHub link: 
[DasynceSE](https://github.com/jkatsanis/DasynceSE.git)

**How does the SpriteRepository work?**
![SpriteRepository](Github/repoplan.png)
