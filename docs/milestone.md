# Milestone Report

## Status Report
Our project is focusing on Real-time Position-based Fluid Rendering. In the past week, we are working on implementing the features based on the project 4. The benefit of using project 4 is that the GUI is already very helpful, users can change the gravity, rendering method, and all other parameters in realtime.
We have modified the project 4 program so that it support multiple objects/particles instead of single cloth now. We model the water as set of spherical particles, and they are incompressible just like the solid spheres. Implementing the collision algorithms are the key to the realistic simulation, we have implemented efficient neighbor finding algorithms so that we are able to calculate each particles new position and velocity in real time.

The video and presentation below are simple demonstration of what we have accomplished during the last few weeks. The model is working very well and we will keep improving it by finding better algorithms and more parallelization.

For rendering, we are still currently focusing on the physical model, once the model is finished, we will add a new shader so that it would only show the rendered, transparent water textures for the model.

Our plan for this week is to continue improve the speed of the simulation and possibly starting to use CUDA or OpenGL library so we can fully take the advantage of the GPU.

## Video

## Presentation slides
