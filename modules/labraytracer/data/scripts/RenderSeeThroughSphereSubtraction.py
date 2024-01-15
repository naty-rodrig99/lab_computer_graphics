#Inviwo Python script 
import inviwopy

from inviwopy import qt
import inviwopy.glm as glm
import ivw.utils as inviwo_utils
import math

app = inviwopy.app
network = app.network
ray = network.Raytracer
cam = ray.camera
lightPos = network.Pointlightsource2.lightPosition.position
lightPowerOuter = network.Pointlightsource2.lighting.lightPower
lightPowerInner = network.Pointlightsource.lighting.lightPower

InitialLookTo = cam.lookTo
InitialLookFrom = cam.lookFrom
InitialLookUp = cam.lookUp
#<lookFrom x="12.191141" y="13.39508" z="11.727107" />
#<lookTo x="0" y="0" z="3.616187" />
#<lookUp x="0" y="0" z="1" /><near content="0.1" />
#<far content="100" /><aspectRatio content="1.248848" />
#<fov content="38" />

InitialDir = InitialLookFrom - InitialLookTo
InitialLightDir = lightPos.value
InitialLightPowerOuter = lightPowerOuter.value
InitialLightPowerInner = lightPowerInner.value

NumImages = 1201
for i in range(NumImages):

    t = i / (NumImages - 1)

    #Light Position outer light
    RotateBy = 2 * math.pi * t
    NewLightDir = glm.rotate(InitialLightDir, RotateBy, glm.vec3(0.0,0.0,1.0))
    Dampen = 2/3 + math.cos(2 * math.pi * t) / 3
    lightPos.value = glm.vec3(NewLightDir.x / Dampen, NewLightDir.y / Dampen, Dampen * NewLightDir.z)

    #Light Power outer light
    Modulation = 1 - math.sin(math.pi * t) / 2
    lightPowerOuter.value = InitialLightPowerOuter * Modulation

    #Light Power inner light
    Modulation = 1 + math.sin(math.pi * t)
    lightPowerInner.value = InitialLightPowerInner * Modulation

    #Camera
    RotateBy = math.sin(t * math.pi * 2) * math.pi / 4
    #SwingBy = 9/10 + math.cos(4 * math.pi * t) / 10
    #SwingDown = 2/3 + math.cos(2 * math.pi * t) / 3
    NewDir = glm.rotate(InitialDir, RotateBy, InitialLookUp)
    NewDir2 = glm.rotate(NewDir, RotateBy, glm.normalize(glm.vec3(-1.0,0.0,1.0)))
    #cam.lookFrom = InitialLookTo + glm.vec3(SwingBy * NewDir.x, SwingBy * NewDir.y, SwingDown * SwingBy * NewDir.z)
    cam.lookFrom = InitialLookTo + glm.vec3(NewDir2.x, NewDir2.y, NewDir2.z)

    ray.render.press()
    inviwo_utils.update() # Needed for canvas to update

    network.Canvas.snapshot("D:\\Shot\\Vorlesung\\Raytracer\\SeeThroughSphereSubtraction\\Animation.%04d.png" % i);
