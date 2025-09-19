# 🌟 miniRT - Ray Tracing Made Simple!

> *"Rendering reality, one pixel at a time"* 🚀

A beautiful **ray tracer** implementation built as part of the 42 School curriculum. Watch as mathematics transforms into stunning photorealistic 3D scenes!

## 🎯 What is miniRT?

miniRT is a **3D rendering engine** that uses ray tracing algorithms to create realistic images from mathematical scene descriptions. Think of it as a virtual photographer that shoots rays of light through every pixel of your screen to calculate what color that pixel should be.

### ✨ The Magic Behind Ray Tracing

Imagine you're looking through a window at a beautiful landscape. Your eyes receive light rays that have bounced off objects in the scene. Ray tracing simulates this process in reverse:

1. **Cast rays** from the camera through each pixel
2. **Calculate intersections** with 3D objects (spheres, planes, cylinders)
3. **Simulate lighting** effects (shadows, reflections, ambient lighting)
4. **Render the final image** pixel by pixel

## 🎨 What Can miniRT Do?

Our ray tracer brings 3D worlds to life with support for:

### 🎯 Geometric Objects
- **Spheres** - Perfect for planets, balls, or bubbles
- **Planes** - Great for floors, walls, and infinite surfaces  
- **Cylinders** - Ideal for columns, tubes, and curved surfaces

### 💡 Lighting System
- **Ambient lighting** - Soft, overall illumination
- **Directional lights** - Like sunlight streaming through a scene
- **Point lights** - Bright spots that illuminate nearby objects

### 🎭 Visual Effects
- **Realistic shadows** - Objects cast shadows based on light sources
- **Material properties** - Different colors and surface characteristics
- **Multiple light sources** - Complex lighting setups for dramatic scenes

## 🏗️ Built With

- **C Programming Language** - For performance and control
- **MiniLibX** - 42's graphics library for window management and pixel manipulation
- **Mathematical precision** - Vector algebra and geometric calculations
- **Pure dedication** - And maybe a few cups of coffee ☕

## 📖 42 School Curriculum Context

This project is part of the 42 School's computer graphics curriculum, designed to:

- **Master 3D mathematics** - Vectors, matrices, and geometric transformations
- **Understand computer graphics** - How images are generated from mathematical descriptions
- **Practice C programming** - Memory management, optimization, and clean code
- **Learn ray tracing** - One of the most elegant rendering algorithms in computer graphics

### 🎓 Educational Goals

- Implement ray-object intersection algorithms
- Handle coordinate transformations and camera positioning  
- Manage lighting calculations and color blending
- Parse scene description files and validate input
- Create an interactive graphics application

## 🚀 Getting Started

### Prerequisites

```bash
# On macOS (required for MiniLibX)
xcode-select --install

# On Linux (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install gcc make xorg libxext-dev libbsd-dev
```

### Installation

```bash
# Clone the repository
git clone https://github.com/rach3bartmoss/miniRT.git
cd miniRT

# Compile the project
make

# Run with a scene file
./miniRT scenes/example.rt
```

### 🎬 Scene File Format

Create your own 3D scenes using our simple scene description language:

```
# Camera settings
C 0,0,0 0,0,1 70

# Ambient lighting  
A 0.2 255,255,255

# Light source
L 10,10,10 0.7 255,255,255

# A red sphere
sp 0,0,5 2 255,0,0

# A blue plane as ground
pl 0,-2,0 0,1,0 0,0,255
```

## 🎮 Controls

- **ESC** - Exit the program
- **Mouse** - Explore the rendered scene
- **Window close button** - Alternative exit method

## 🎨 Example Renders

*"A picture is worth a thousand rays!"* 

Here are some example scenes you can create:

### Simple Scene
```
# Basic three-object scene
C 0,0,0 0,0,1 70
A 0.1 255,255,255  
L 5,5,0 0.6 255,255,255

sp 0,0,5 1 255,100,100    # Red sphere
sp -2,0,5 1 100,255,100   # Green sphere  
sp 2,0,5 1 100,100,255    # Blue sphere
pl 0,-1,0 0,1,0 200,200,200  # Gray floor
```

### Dramatic Lighting
```
# Moody scene with multiple lights
C 0,2,-5 0,-0.3,1 60
A 0.05 100,100,150

L 5,10,-2 0.8 255,200,150    # Warm key light
L -3,5,2 0.4 150,200,255     # Cool fill light

sp 0,0,0 1.5 150,75,0        # Orange sphere
cy 2,0,0 0,1,0 1 3 200,200,200  # Tall cylinder
pl 0,-2,0 0,1,0 50,50,50        # Dark floor
```

## 🛠️ Features in Detail

### Ray-Object Intersection
- **Sphere intersection** - Solving quadratic equations in 3D space
- **Plane intersection** - Linear algebra for infinite surfaces  
- **Cylinder intersection** - Complex geometric calculations

### Lighting Model
- **Phong lighting** - Ambient + Diffuse + Specular components
- **Shadow calculation** - Ray casting to light sources
- **Multiple light support** - Realistic multi-light scenes

### Camera System
- **Position and orientation** - Full 6DOF camera control
- **Field of view** - Adjustable viewing angles
- **Resolution handling** - Aspect ratio and pixel mapping

## 🤝 Contributing

Found a bug or want to add a feature? Contributions are welcome!

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📚 Learning Resources

Want to dive deeper into ray tracing? Check out these resources:

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) - Excellent introduction
- [Scratchapixel](https://www.scratchapixel.com/) - Computer graphics fundamentals  
- [PBRT](https://www.pbr-book.org/) - The definitive ray tracing reference

## 🏆 42 School Projects

Part of the journey at 42 School? Check out other graphics projects:

- **FdF** - Wire-frame 3D projections
- **Cub3D** - Raycasting game engine (like Wolfenstein 3D)
- **miniRT** - Full ray tracing (you are here! 📍)

## 📄 License

This project is part of the 42 School curriculum. Use it for learning and educational purposes!

---

*Made with ❤️ at 42 School*

**"The only way to learn ray tracing is to implement it yourself"** - Ancient 42 Proverb (probably) 😄
