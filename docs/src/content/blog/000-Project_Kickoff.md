---
title: "000 - Project Kickoff"
description: "Project plan and setup; math library beginnings"
pubDate: "2026-06-01"
heroImage: "/src/assets/000_Kickoff.jpg"
---

In the beginning, I don't want to get bogged down by graphics APIs and their associated boilerplate. I do, however, want to understand the algorithms responsible for rendering 3D (or 2D) objects in a scene as pixels on a grid (representing an image). There are two main approaches to handle this 3D &rarr; 2D mapping, namely *object-ordered* and *image-ordered* rendering.   

In *object-ordered* rendering, the objects in a scene are projected onto the viewing plane (determined by the camera). The pixels covered by the objects (or primitives like triangles) are determined, and shaded. This is also known as *rasterized rendering*.

In *image-ordered* rendering, rays are cast that go through each pixel in the viewing plane and into the scene. Ray-Object intersections are checked to determine which pixels should be shaded. This is also known as *ray tracing* or *path tracing*.

Each of these two paradigms can be explored using Software Rendering (running primarily on the CPU) and Hardware Rendering (running on the GPU). I want to get a handle on SW rendering before I go on to experiment with the GPU side of things. The following is the general order I want to go about with my projects:-

**1. Software Rasterized Renderer**  
**2. Software Path Tracer**  
**3. Hardware Accelerated Renderer**  
**4. Advanced Graphics Engine**

The programming language I will use for these projects is *C++*, mainly because I am relatively more familiar with it and it is the industry-standard in graphics programming.

### Setting up a github monorepo

Since there are several potential commonalities between the projects (like a math library for example), and also for the sake of organization, I have opted to create a mono-repo on github ([Graphics-Projects](https://github.com/mohid8/Graphics-Projects)). The initial structure will look something like this (though I'm sure it can change in the future):-
- **Core:** The common components between all the projects like the math library, graphics-related structures etc.
- **CPURasterizer:** The software rasterized renderer.
- **CPUPathTracer:** The software path tracer.
- **Engine:** The GPU accelerated renderer. Probably will start with object-ordered rendering acceleration, then add support for path-tracing and then a hybrid approach. Will add more features with time.
- **ThirdParty:** Collector for third-party libraries that the projects will use. I want to focus more on the rendering algorithms, especially in the beginning, so I'll offload things like displaying the framebuffer in a window or parsing obj files to these libraries.
- **docs:** For collecting any documentation associated with this project, and also the home for this blog's files etc.
- **build:** Collecting build related files, including executables, .lib files etc.

I admit I haven't used github for a project like this before so I'll probably make some mistakes or work in a sub-optimal manner. I'm fine with that as long as I learn and improve.

### Setting up the environment  
The IDE I am using for these projects is [VS Code](https://code.visualstudio.com/) primarily because I am most familiar with it. Additionally, I am using CMake to configure the build environment hoping that it will make it easier to work in this monorepo structure. Again, I haven't really used CMake too much before, but there is a helpful [tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html) at the CMake website that I used to understand the basics.

Simply put, I created *CMakeLists.txt* files in the monorepo root and in the subdirectories (like in Core, CPURasterizer etc.). The root *CMakeLists.txt* sets global parameters that control what standards are used, what libraries are imported, where the build outputs are stored, compiler flags etc. The root *CMakeLists.txt* looks something like this:

```cmake
cmake_minimum_required(VERSION 3.22)
project(GraphicsProjects CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(MSVC)
    add_compile_options(/W4 /WX /permissive- /Oi)
else()
    add_compile_options(-Wall -Wextra -Werror -O3)
endif()

add_subdirectory(Core)
# add_subdirectory(CPURasterizer)       # Will uncomment later
# add_subdirectory(CPUPathTracer)       # Will uncomment later
# add_subdirectory(Engine)              # Will uncomment later
```
*Code Block 1: Root CMakeLists.txt file that sets up the overall project*

I have left the `add_subdirectory()` for the subprojects commented out. For now, the core math library is what i'll begin working on. Here is how I set up the *CMakeLists.txt* for it:-  

```cmake
add_library(Core STATIC
    src/GMath.cpp
    include/GMath.hpp
)

target_include_directories(Core PUBLIC include)

add_library(GMath::core ALIAS Core)
```
*Code Block 2: Core subdirectory CMakeLists.txt file*

I plan to have the math library as header-only, but I've still kept both *GMath.cpp* and *GMath.hpp* files in the project if I see the need to change it in the future. Additionally, `GMath::core` will be used as an *alias* so that CMake interprets it as an existing target, preventing compilation in case of typos. Now that the scaffolding for the project is set up, I can begin work on the core math library.

### Math library beginnings

I'll divide my progress in this section into sub-headings for a bit more clarity.

#### 3D Vectors

Points in a 3D space can be represented mathematically by vectors such as: 
$$
\vec{u} = \begin{pmatrix} x \\ y \\ z \end{pmatrix}
$$ 
where $x, y, z$ represent cartesian coordinates. The same vector can also be represented as $\vec{u} = \begin{pmatrix} x & y & z \end{pmatrix}^T$ which takes a bit less visual space in the text here, being more compact. In any case, the intention here is that the vectors are treated as *column* vectors as opposed to *row* vectors for reasons which will become apparent. Vectors can also be used to store color information as $\vec{c} = \begin{pmatrix} R & G & B \end{pmatrix}^T$, representing red, green and blue channels. 

In my code, I implemented it as a struct containing 3 *floats*. Using operator overloading, I handled addition and subtraction (with other vectors) as well as multiplication and division (by a scalar). 

Vectors can also be multiplied with each other in two ways which are useful in this context. The first of these is the **dot product**, represented as the following (assuming the vectors are in an orthonormal basis spanned by unit vectors in the $x$, $y$ and $z$ directions):-

$$
\vec{u} \cdot \vec{v} = \lVert\vec{u}\rVert \lVert \vec{v} \rVert \cos\theta
$$ 

$$
or
$$

$$
\vec{u} \cdot \vec{v} = u_x v_x + u_y v_y + u_z v_z 
$$ 

, where $\theta$ is the smaller angle between the two vectors. 

As evident, the output is a scalar number. It represents how aligned the two vectors are, with a dot product of $0$ meaning the vectors are orthogonal (at 90° to each other).

The other is the **cross product**, which returns a vector whose magnitude can be represented as:-

$$
\vec{u} \times \vec{v} = \lVert\vec{u}\rVert \lVert \vec{v} \rVert \sin\theta
$$

The direction of the resultant vector is orthogonal to both $\vec{u}$ and $\vec{v}$ (or to the plane spanned by $\vec{u}$ and $\vec{v}$), determined using the right hand rule. It can also be shown as:

$$
\vec{u} \times \vec{v} = \begin{pmatrix} u_y v_z - u_z v_y \\ u_z v_x - u_x v_z \\ u_x v_y - u_y v_x \end{pmatrix}
$$ 

I am not planning to go into the derivations here, but the [Immersive Math](https://immersivemath.com/ila/index.html) website is a great resource for this and I consult it regularly.

Its useful to have separate functions for the length and squared-length of a vector. Often times unit vectors are being dealt with and the square root operation is unnecessary there. Other times, perhaps a length comparison between two vectors is needed, and for that comparing the squared length is also enough. The squared length of a vector $\vec{u}$ is basically its dot product with itself:-

$$
\lVert\vec{u}\rVert^2 = \vec{u} \cdot \vec{u} = u_x^2 + u_y^2 + u_z^2
$$ 

The actual length if needed, can be determined by: $\sqrt{\lVert\vec{u}\rVert^2}$

One more thing I added was a method to clamp the vector components to a desired value which could be useful, for example, to limit *RGB* component values to be within defined bounds.

I've implemented the aforementioned `Vec3` struct in the `root/Core/include/GMath.hpp` as follows:-

```cpp
#pragma once

#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>

namespace GMath
{
    struct Vec3
    {
        union
        {
            struct{float x, y, z;};
            struct{float r, g, b;};
            float e[3];
        };

        Vec3(): x(0.0f), y(0.0f), z(0.0f) {}
        Vec3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}


        Vec3& operator+=(const Vec3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vec3& operator-=(const Vec3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vec3& operator*=(float t)
        {
            x *= t;
            y *= t;
            z *= t;
            return *this;
        }

        Vec3& operator/=(float t)
        {
            return *this *= 1/t;
        }

        float length() const
        {
            return std::sqrt(lengthSquared());
        }

        float lengthSquared() const
        {
            return x * x + y * y + z * z;
        }

        Vec3& clamp(float min, float max)
        {
            x = std::clamp(x, min, max);
            y = std::clamp(y, min, max);
            z = std::clamp(z, min, max);
            return *this;
        }
    };

    inline std::ostream& operator<<(std::ostream& out, const Vec3& u)
    {
        return out << u.x << ' ' << u.y << ' ' << u.z;
    }

    inline Vec3 operator+(const Vec3& u, const Vec3& v)
    {
        return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
    }

    inline Vec3 operator-(const Vec3& u, const Vec3& v)
    {
        return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
    }

    inline Vec3 operator-(const Vec3& u)
    {
        return Vec3(u.x * - 1, u.y * -1, u.z * -1);
    }

    inline Vec3 operator*(const Vec3& u, float t)
    {
        return Vec3(u.x * t, u.y * t, u.z * t);
    }

    inline Vec3 operator*(float t, const Vec3& u)
    {
        return u * t;
    }

    inline Vec3 operator/(const Vec3& u, float t)
    {
        return u * 1/t;
    }

    inline float dot(const Vec3& u, const Vec3& v)
    {
        return u.x*v.x + u.y*v.y + u.z*v.z;
    }

    inline Vec3 cross(const Vec3& u, const Vec3& v)
    {
        return Vec3(
            u.y * v.z - u.z * v.y,
            u.z * v.x - u.x * v.z,
            u.x * v.y - u.y * v.x
        );
    }

    inline Vec3 normalize(const Vec3& u)
    {
        return u/u.length();
    }

    inline Vec3 power(const Vec3& u, float t)
    {
        return Vec3(std::pow(u.x, t), std::pow(u.y, t), std::pow(u.z, t));
    }
}
```
*Code Block 3: Vec3 struct and functions implementation*

Analogous to `Vec3`, I also implemented a `Vec4` struct, with a 4th component so it looks like $\begin{pmatrix} x & y & z & w \end{pmatrix}^T$. These vectors, representing *homogenous coordinates*, enable many cool things which I will get into later when dealing with transformations.

#### Function Inlining
One interesting thing I learned was [Inline Expansion](https://en.wikipedia.org/wiki/Inline_expansion) and how the compiler can replace function calls with the actualy body of the function at the points where it is called. This obviously eliminates function call overhead, but can also help the compiler optimize further on the enlarged code. The straightforward cost is that the size compiled binary/executable becomes larger. 

However, if the function's size is large, inlining can also actually worsen performance by reducing cache-locality, trigerring instruction cache misses and even causing cache eviction (terms I learned about while digging into this topic, but I won't explain them here). 
Inlining is therefore best suited to smaller functions, that are called often (like in a loop), where the function call overhead could end up taking more time than the actual logic of the function. 

For the small-footprint math functions used in `Vec3`, inlining definitely makes sense. Interestingly, I found that (in C++ atleast) the `inline` keyword doesn't actually force the compiler to inline the function, and is apparently more of a suggestion. Whether or not the function is actually inlined depends on the compiler and its own heuristics. Regardless, the `inline` keyword is still necessary because of the *One Definition Rule (ODR)* as it tells the compiler that even though this function is being defined many times (as many times as this header file is included), the definition is identical. The compiler then keeps one copy and discards the rest, and all function calls point to this copy, assuming the function is not *actually* inline substituted (which I find a bit hilarious).

Later, it could be interesting to explore by somehow forcing inlining on and off to see how the graphics program is impacted.

#### Enter the Matrix
A matrix, in this context, mainly acts as a function that takes a vector as its input and spits out a new vector. In other words, it *transforms* the input vector. It can enable operations like *scaling*, *rotation*, *translation*, *shearing*, *projection* and so on. I'll get into that stuff later. For now, the thing to decide is whether the matrices will be *column-major* or *row-major*.

Let's say I want to multiply a 3x3 matrix $A$ and a vector $\vec{u}$:

$$ 
A = 
\begin{bmatrix}
a_{0} & a_{1} & a_{2} \\
a_{3} & a_{4} & a_{5} \\
a_{6} & a_{7} & a_{8} \\
\end{bmatrix},
\vec{u} = \begin{pmatrix} x \\ y \\ z \end{pmatrix}
$$

According of matrix multiplication rules, we can only *right-multiply* these two as the number of columns on the left must match the number of rows on the right. However, if we write out $\vec{u}$ as a row vector, then we can only *left-multiply*. Let's try both:

$$ 
A\vec{u}= 
\begin{bmatrix}
a_{0} & a_{1} & a_{2} \\
a_{3} & a_{4} & a_{5} \\
a_{6} & a_{7} & a_{8} \\
\end{bmatrix}
\begin{pmatrix} x \\ y \\ z \end{pmatrix}=
\begin{pmatrix} a_0x+a_1y+a_1z  \\ a_3x+a_4y+a_5z \\ a_6x+a_7y+a_8z \end{pmatrix}
$$

$$ 
\vec{u}A=
\begin{pmatrix} x & y & z \end{pmatrix} 
\begin{bmatrix}
a_{0} & a_{1} & a_{2} \\
a_{3} & a_{4} & a_{5} \\
a_{6} & a_{7} & a_{8} \\
\end{bmatrix}
=
\begin{pmatrix} a_0x+a_3y+a_6z & a_1x+a_4y+a_7z & a_2x+a_5y+a_8z \end{pmatrix}
$$

It can be seen that by just changing the multiplication order, we get two different results (in terms of the resultant vector coefficients). To avoid this, we can transpose the matrix $A$:

$$ 
A\vec{u}= 
\begin{bmatrix}
a_{0} & a_{3} & a_{6} \\
a_{1} & a_{4} & a_{7} \\
a_{2} & a_{5} & a_{8} \\
\end{bmatrix}
\begin{pmatrix} x \\ y \\ z \end{pmatrix}=
\begin{pmatrix} a_0x+a_3y+a_6z \\ a_1x+a_4y+a_7z \\ a_2x+a_5y+a_8z \end{pmatrix}
$$

Now we get the same coefficients. The point of this example is that we need a different matrix $A$ if we are following a row-vector or a column-vector convention. Since the books that I'm studying from use the column-vector convention, I have adopted that as well.

As for how the matrix is actually stored in memory, that is another topic. In C++, if I create a multi-dimensional array like `a[3][3]`, considering the outer index (left) as *rows* and the inner index (right) as *columns*, each **row** will be stored sequentially in memory. So `a[0][0]`, `a[0][1]`, `a[0][2]`, `a[1][0]`, `a[1][1]` and so on. 

I have decided to go for *column-major* storage for the matrices in this project. This means each column of the matrix will be stored sequentially in memory. Although storing in row or column major doesn't have a direct impact on performance, how I handle operations like multiplication etc. could use knowledge of how the matrix is laid out in memory to allow techniques like [SIMD](https://en.wikipedia.org/wiki/Single_instruction,_multiple_data) vectorization to boost performance. 

For now, I'm **not** planning to have the most efficient code possible, instead opting for clarity/simplicity to get something on the screen faster. However, I will definitely pursue optimization later on. I'll show the matrix implementation and related functions in the next blog post.

### Concluding thoughts
After writing this first blog post, I feel reassured that maintaining this alongside my work on the project is the right move for me. As I hoped, it has forced me to slow down, ponder over my decisions, raise questions, do some more in-depth research into different topics and help solidify concepts I have learned/applied. 

I'm still trying to find a sweet spot for how detailed I want these posts to be. Would it be more helpful for me to have full mathematical derivations? How much code should I display? I'm sure things will become more clear as I go on. The main goal is for me to learn and improve, not to achieve perfection right from the start.

