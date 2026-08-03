---
title: "Devlog #X: [Topic Title]"
description: "Brief summary of what was implemented today."
pubDate: "2026-07-30"
heroImage: "/src/assets/blog-placeholder-1.jpg"
---

## Objective
What feature or math component was I trying to build today?

# Devlog: Project Initialization & Architecture Design
**Date:** June 02, 2026
**Current Milestone:** Developing Core Math Library

### 1. Objectives Completed
- Designed a hybrid monorepo repository structure to handle the different projects
- Setting up the CMakeLists and other environment related stuff
- Implemented Vec3, Vec4, and parital Mat4 structs and related functions in the GMath::core library

### 2. Architectural Decisions & Trade-offs
- Column Major Matrix arrangement


### 3. Current Roadblocks & Next Steps
- **Roadblock:**
- **Next Step:** Complete Mat4 and Quat

# Devlog: Projection Math
**Date:** June 09, 2026
**Current Milestone:** Developing Core Math Library

### 1. Objectives Completed
- Implemented 4x4 Matrix functions
- Some basic Quaternion functions
- Implemented Camera Look At and Orthographic/Perspective Projection Matrices

### 2. Architectural Decisions & Trade-offs
- Right handed Viewing direction (towards -z), and left handed NDC (normalized device coordinates)
- Following OpenGL canonical view volume (-1,-1,-1) bottom left to (1,1,1) top right
- Matrice inverse using Cramer's rule, could explore other methods and optimizations later
- Could explore different methods to improve z-buffer accuracy at longer distances

### 3. Current Roadblocks & Next Steps
- **Roadblock:**
- **Next Steps:** Move on to rasterized rendering and add math functionalities as needed. Write up derivation for projection matrices.

# Devlog: Rasterized Rendering beginnings
**Date:** June 22, 2026
**Current Milestone:** Wireframe rendering

### 1. Objectives Completed
- Integrating GLFW and TinyOBJLoader libraries
- Experimented with a bit of profiling, saving the results for a future analysis/blog post
- Implemented Rasterizer Class for handling line/triangle drawing and presenting buffer to GLFW
- Implemented Vertex struct for storing vertex information (for now just Vec4 pos and Vec4 color)
- Implemented Mesh struct for storing array of vertices, and array of vertex indices to avoid duplication
- Implemented Pipeline class for taking a mesh, transforming it to screen space, and handing it to rasterizer for drawing (tried it with Utah Teapot)

### 2. Architectural Decisions & Trade-offs
- Using GLFW for handling the window display, will explore how to do that myself later maybe
- Using tinyobjloader for parsing obj files instead of parsing them manually, want to focus more on the rendering side for now
- Trying to make drawing algorithms incremental, makes the code a bit verbose, maybe will focus on getting thing on screen first and optimizing later
- Having the Mesh and Vertex structs in separate files inside Core library, since they will probably be used for the other projects

### 3. Current Roadblocks & Next Steps
- **Roadblock:**
- **Next Steps:** Handling clipping. Filling triangles instead of just wireframe. 
Updating Vertex struct to have more info. Using a z-buffer. Adjustable realtime camera. Shading basics.


## Implementation Notes
Key details on code structure, algorithm choices, or shaders.

```cpp
// Code snippet goes here