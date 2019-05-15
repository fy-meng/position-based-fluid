# Real-time Position-based Fluid Rendering
Project for Berkeley CS 184: Computer Graphics and Imaging, Spring 2019

## Project Proposal

[Project Proposal](docs/proposal.md) 

## Milestone

[Milestone](docs/milestone.md)

## Abstract

The projects aims to implement a real-time rendering of liquid 
simulation using a particle-based method. The fluid is represented by a 
underlying mesh of particles, and in each iterations, we enforce 
incompressibility by introducing by formulating and solving a set of 
positional constraints that enforce constant density [[1]](macklin2013). 

## Technical Approach

The code is modified from 
[project 4 skeleton code of Berkeley CS 184](https://github.com/cal-cs184/p4-clothsim).

The algorithm is mostly based on Macklin and Muller's *Position Based 
Fluids* [[1]](macklin2013) as follows:

![](docs/images/algs-simulation-loop.png)

The original paper contains incompressibility, tensile instability, 
vorticity confinement and viscosity constraints. Due to time limitation, 
we did not implement the vorticity constraint. 

The neighbor searching algorithm is done by creating a position hash of 
all particles before each simulation loop. Each position is mapped to a 
block whose side is equal to the kernel radius. When searching for the 
neighbors of a given position, we iterate through the 3x3x3 blocks 
centered by corresponding block of the input position. 

The constraints are solved using a iterative density solver, trying to 
change the position of each particle so that all particles are close to 
their rest density.

The biggest obstacle during our implementation is the problem of 
particles cramming and loosing momentum after collision. Our original 
collision algorithm is done by moving the colliding particle from its 
previous position to the collision surface with a friction factor, but 
this method does not take the velocity of the particle into position. 
Thus, the particles that fall from a high distance loss most of its 
momentum since we are doing a position-based dynamic system. Our 
solution is to use a velocity-based collision, and each surface will 
reflect a colliding particle back corresponding to its previous velocity 
instead of position. 

<video width="320" height="240" controls>
  <source src="docs/videos/cramming.mp4" type="video/mp4">
  <p>Your browser doesn't support HTML5 video. Here is a 
    <a href="docs/videos/cramming.mp4">link to the video</a> instead.</p>
</video>

Additionally, since we did not implement vorticity constraints, the 
particles suffer from momentum loosing and sticking to the collision 
surfaces. The tensile instability constraints help with the problem but 
not fully solve it. Thus, we added an additional collision detection 
after the constraint solver, and gives colliding particles that are 
nearly still an artificial velocity away from the collision surfaces. 
This allows the particles to have a tendency to move away from cramming 
after the collision.

To speed up the simulation, we resolve to OpenMP but the effect is 
limited except for on Linux.

## Results

<video width="320" height="240" controls>
  <source src="docs/videos/liquid.mp4" type="video/mp4">
  <p>Your browser doesn't support HTML5 video. Here is a 
    <a href="docs/videos/liquid.mp4">link to the video</a> instead.</p>
</video>

A cube of liquid dropping into a slight larger container.

<video width="320" height="240" controls>
  <source src="docs/videos/tap.mp4" type="video/mp4">
  <p>Your browser doesn't support HTML5 video. Here is a 
    <a href="docs/videos/tap.mp4">link to the video</a> instead.</p>
</video>

A column of water dropping into the corner of a container.

<video width="320" height="240" controls>
  <source src="docs/videos/milk-crown.mp4" type="video/mp4">
  <p>Your browser doesn't support HTML5 video. Here is a 
    <a href="docs/videos/milk-crown.mp4">link to the video</a> instead.</p>
</video>

An attempt to simulate the "milk crown" effect.

<video width="320" height="240" controls>
  <source src="docs/videos/no-gravity.mp4" type="video/mp4">
  <p>Your browser doesn't support HTML5 video. Here is a 
    <a href="docs/videos/no-gravity.mp4">link to the video</a> instead.</p>
</video>

Water particles without gravity.

## Future Works

We could reconstruct a triangular mesh from the existing particle mesh 
to get a more realistic water representation. We can also add the ray
tracing functionality to show the reflection of refraction of the liquid. 

The liquid simulation could be rewrite along with GPU acceleration. 
Since in the iterative constraint solver, all calculation on the 
particles are independent, they can all be run in parallel and speed up 
by a large factor. 

Also, it could potentially be possible that the constraint solve can be 
speed up using a machine learning method, the majority of the time spent 
is on the solver. The idea behind the current iterative solver is 
similar to gradient descent, we could potentially run a large number of 
iterations until convergence, feed the data into a neural net, and 
possibly get a fast well-performing solver.

## Reference

<a name="macklin2013"></a>
\[1\] Macklin, Miles, and Matthias Müller. "Position based fluids." ACM 
Transactions on Graphics (TOG) 32.4 (2013): 104.

## Contributions


