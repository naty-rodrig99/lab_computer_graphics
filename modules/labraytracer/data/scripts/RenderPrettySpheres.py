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

#Seed for the video:
#network.Raytracer.seedPrettySpheres.value = -511408457
#qt.update()

Speed = 0.1

InitialLookTo = cam.lookTo
InitialLookFrom = cam.lookFrom
InitialLookUp = cam.lookUp
#<lookFrom x="-0.26221034" y="9.364587" z="1.9756403" />
#<lookTo x="0" y="0" z="0" />
#<lookUp x="0" y="0" z="1" />
#<near content="0.2747883" /><far content="1000" />
#<aspectRatio content="1.4285715" /><fov content="45" />

InitialDir = InitialLookFrom - InitialLookTo
d = glm.length(InitialLookFrom - InitialLookTo)

NumImages = 1801
for i in range(NumImages):
    t = i / (NumImages - 1)
    RotateBy = 2 * math.pi * t
    SwingBy = 3/4 + math.cos(4 * math.pi * t) / 4

    x = d * math.sin(RotateBy)
    y = -d * math.cos(RotateBy)

    #EulerAngles = glm.vec3(0, 0, RotateBy)
    #RotQuaternion = glm.quat(EulerAngles)
    NewDir = glm.rotate(InitialDir, RotateBy, InitialLookUp)

    cam.lookFrom = InitialLookTo + glm.vec3(SwingBy * NewDir.x, SwingBy * NewDir.y, SwingBy * NewDir.z)
    ray.render.press()
    inviwo_utils.update() # Needed for canvas to update

    #qt.update()
    #print([i, t])
    network.Canvas.snapshot("D:\\Shot\\Vorlesung\\PrettySpheres\\Rotation.%05d.png" % i);
