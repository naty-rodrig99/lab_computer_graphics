#Inviwo Python script 
import inviwopy

from inviwopy import qt
import inviwopy.glm as glm
import ivw.utils as inviwo_utils
import math

app = inviwopy.app
network = app.network
ray = network.Raytracer
lightPos = network.Pointlightsource2.lightPosition.position

InitialLightDir = lightPos.value

NumImages = 301
for i in range(NumImages):
    t = i / (NumImages - 1)
    RotateBy = 2 * math.pi * t
    SwingBy = 9/10 + math.cos(4 * math.pi * t) / 10
    SwingDown = 2/3 + math.cos(2 * math.pi * t) / 3

    NewLightDir = glm.rotate(InitialLightDir, RotateBy, glm.vec3(0.0,0.0,1.0))

    lightPos.value = NewLightDir
    ray.render.press()
    inviwo_utils.update() # Needed for canvas to update

    network.Canvas.snapshot("D:\\Shot\\Vorlesung\\Raytracer\\SpikedSphereSubtraction\\LightRotation.%04d.png" % i);
