# ODT - Dungeon
ODT-Dungeon is the Summer Week 2023 practice project aimed at extending the real-time raytracer from the previous exercise. The goals include separating the renderer and raytracer, optimizing and cleaning up the raytracer, and splitting the single thread into Main, Scene, and Render threads. Above all, the main focus is to have fun and enjoy the project.

ODT-Dungeon is the practice project of Summer week 2023. 
Goals: 
- have fun and enjoy. 
- extend the real-time-raytracer of the previous practice project
- isolate renderer and raytracer
- cleanup the raytracer 
- splitt the single thread to each one for Main, Scene and Render 



+ github
+ main
+ qtc
+ module
+ window
+ scene
+ renderer
+ vulkan
+ triangle
+ input
+ rock

- char
- bones




- rm VulkanMeshPool
- rm VulkanRenderableArray
- rm VulkanLightArray
- rm VulkanMaterialArray
- globals.data mv private
- rm dependencies of mesh to vulkan
- VulkanMesh extends Mesh
- render-lights reorg
- scenenode.childs as array 
- rm Physics::Global, PhysicsWorld as Property of Scene, as a own Thread
