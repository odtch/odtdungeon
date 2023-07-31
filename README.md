# ODT - Dungeon
ODT-Dungeon is the Summer Week 2023 practice project aimed at extending the real-time raytracer from the previous exercise. The goals include separating the renderer and raytracer, optimizing and cleaning up the raytracer, and splitting the single thread into Main, Scene, and Render threads. Above all, the main focus is to have fun and enjoy the project.

## Goals
- have fun and enjoy. 
- extend the real-time-raytracer of the previous practice project
- isolate renderer and raytracer
- cleanup the raytracer 
- splitt the single thread to each one for Main, Scene and Render 

## Erkenntnisse
Die Verschiebung der Erstellung von Resourcen (Material, Texture, Mesh) in das Renderer-Interface hat nicht die erhoffte Vereinfachung gebracht. Da die Resourcen eigentlich ein Teil der Collection sind, muss der Renderer resp Raytracer weiterhin dynamisch mit neuen Resourcen zurecht kommen. Ungelöst ist daher immer noch das Resourcen-Freigeben bei einem Level-Wechsel. 

## Steps
- [x] github
- [x] main
- [x] qtc
- [x] module
- [x] window
- [x] scene
- [x] renderer
- [x] vulkan
- [x] triangle
- [x] input
- [x] rock
- [x] collection
- [x] char
- [ ] mesh in collection
- [ ] mocapcollection
- [ ] char in dungeon
- [ ] animation
- [ ] bones

## Todoes
- [ ] rm VulkanMeshPool
- [ ] rm VulkanRenderableArray
- [ ] rm VulkanLightArray
- [ ] rm VulkanMaterialArray
- [ ] globals.data mv private
- [ ] rm dependencies of mesh to vulkan
- [ ] VulkanMesh extends Mesh or
- [ ] rm Mesh.vulkanMesh
- [ ] render-lights reorg
- [ ] scenenode.childs as array 
- [ ] rm Physics::Global, PhysicsWorld as Property of Scene, as a own Thread
- [ ] reintroduce Renderable (prevent diamond of instance und vulkaninstance)
- [ ] mv Skin to Resource
- [ ] SkinMesher GPU

