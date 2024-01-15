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
lightPos = network.Pointlightsource.lightPosition.position

InitialLookTo = cam.lookTo
InitialLookFrom = cam.lookFrom
InitialLookUp = cam.lookUp
#<lookFrom x="6.7809315" y="4.820442" z="12.551248" />
#<lookTo x="-0.5992905" y="-0.063792214" z="0.9333389" />
#<lookUp x="0" y="0" z="1" />
#<near content="0.08776415" /><far content="877.64154" />
#<aspectRatio content="1" /><fov content="38" />

InitialDir = InitialLookFrom - InitialLookTo

InitialLightDir = lightPos.value

NumImages = 601
for i in range(NumImages):
    t = i / (NumImages - 1)
    RotateBy = 2 * math.pi * t
    SwingBy = 9/10 + math.cos(4 * math.pi * t) / 10
    SwingDown = 2/3 + math.cos(2 * math.pi * t) / 3

    NewDir = glm.rotate(InitialDir, RotateBy, InitialLookUp)
    NewLightDir = glm.rotate(InitialLightDir, RotateBy, glm.vec3(0.0,0.0,1.0))

    cam.lookFrom = InitialLookTo + glm.vec3(SwingBy * NewDir.x, SwingBy * NewDir.y, SwingDown * SwingBy * NewDir.z)
    lightPos.value = NewLightDir
    ray.render.press()
    inviwo_utils.update() # Needed for canvas to update

    #qt.update()
    #print([i, t])
    network.Canvas.snapshot("D:\\Shot\\Vorlesung\\SphereSubtraction\\Rotation.%05d.png" % i);
