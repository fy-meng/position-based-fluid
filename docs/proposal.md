# Project Proposal

Real-time Position-based Fluid Rendering in CUDA.

Project for Berkeley CS 184: Computer Graphics and Imaging, Spring 2019.

We are trying to create a render supporting incompressible liquid 
physics.

## Team Members

Fanyu Meng [fy.meng@berkeley.edu], Collin Cao [collincao@berkeley.edu], Serena Wu [serena.wu@berkeley.edu]

## Description

The idea is to implement liquid as a set of spherical particles, 
incorporating an artificial pressure term to improves particle 
distribution and creates surface tension \[1\].

## Goals and Deliverables

We are trying to create a render that given initial position of the 
liquid particles, the render could simulate the consequent movement 
frame by frame. The deliverables will be as follows:

- Images or videos of animated liquid, rendered as a BRDF material and
also the underlying particles;
- (Potential) a GUI That allow the user to change camera angles and add
liquid;
- Speed up the algorithms using a CUDA GPU, then make a speed comparison between using the project 4 skeleton code and using 
CUDA.

## Schedule

- Week 1: no schedule, left for project 4;
- Week 2: implement the physics using project 4 as skeleton, adding additional features based on project 4;
- Week 3: port to CUDA;
- Week 4: continue to port to CUDA, potentially implement a GUI and 
final rendering.

## Resources

The physic model is based on M. Macklin and M. Müller's paper *Position 
based fluids* \[1\].

The initial attempt will be based on CS 184's 
[project 4](https://github.com/cal-cs184/p4-clothsim) and will be later 
port into CUDA.

## Reference

\[1\] M. Macklin and M. Müller, 
“[Position based fluids](http://mmacklin.com/pbf_sig_preprint.pdf),” 
ACM Trans. Graph., vol. 32, no. 4, p. 1, Jul. 2013. 
