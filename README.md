# MNCA Render Tool
GPU-Accelerated [Multiple Neighborhood Cellular Automata (MNCA)](https://slackermanz.com/understanding-multiple-neighborhood-cellular-automata) Render Tool Based on Computer Shaders

---
<img src="Screens/001.png" alt="screen" width="256"/><img src="Screens/002.png" alt="screen" width="256"/><img src="Screens/003.png" alt="screen" width="256"/><img src="Screens/004.png" alt="screen" width="256"/><img src="Screens/005.png" alt="screen" width="256"/><img src="Screens/006.png" alt="screen" width="256"/><img src="Screens/007.png" alt="screen" width="256"/><img src="Screens/008.png" alt="screen" width="256"/><img src="Screens/009.png" alt="screen" width="256"/><img src="Screens/010.png" alt="screen" width="256"/><img src="Screens/011.png" alt="screen" width="256"/><img src="Screens/012.png" alt="screen" width="256"/>

# Cloning the Repository

To get the repository and all submodules, use the following command:

```
git clone --recursive https://github.com/CyberPoincare/MNCARender.git
```

When updating existing repository, don't forget to update all submodules:

```
git pull
git submodule update --recursive
```

It is also a good idea to re-run CMake and perform clean rebuild after getting the latest version.

# How to

On "Settings" panel:

use "`Random All & Respawn`", "`Random Neighbour Def & Respawn`" and "`Random Neighbour Order & Respawn`" to generate new patterns.

use "`Respawn`" to reinit state.

push `Space` key to pause/resume.

---

The tool source code is in `./CyberSpace/`. Since it is coded under DiligentEngine demo code framework, you can easily build and runing it as [DiligentEngine's Tutorials](https://github.com/DiligentGraphics/DiligentEngine#tutorials)
 
Shaders are compiled from `./CyberSpace/MainApp/assets/`

The DiligentEngine can be found at: https://github.com/DiligentGraphics/DiligentEngine

---

Contacts & Networking:

 - Github: https://github.com/CyberPoincare

 - Twitter: https://twitter.com/scorpionpang

