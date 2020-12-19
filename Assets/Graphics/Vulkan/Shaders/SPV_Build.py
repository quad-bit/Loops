import os
import subprocess

currentDirectory = os.getcwd()
#print(currentDirectory)

EXE_PATH = os.environ.get('VULKAN_SDK') + '\Bin\glslangValidator.exe'
#print(EXE_PATH)

for file in os.listdir(currentDirectory):
     filename = currentDirectory + "\\" + os.fsdecode(file)
     #print(filename)
     if filename.endswith('.spv'):
        os.remove(filename)


for file in os.listdir(currentDirectory):
     filename = currentDirectory + "\\" + os.fsdecode(file)
     print('###### ' + filename)
     outFilename = 'Test'
     if filename.endswith('.frag'):
        outFilename =   os.path.splitext(filename)[0] + 'Frag.spv'
     elif filename.endswith('.vert'):
        outFilename =  os.path.splitext(filename)[0] + 'Vert.spv'
     else:
        continue
     #print (outFilename)
     cmd = EXE_PATH + ' -V ' + filename + ' -o '+ outFilename

     print(cmd)
     process = subprocess.Popen(cmd) #, stdout=subprocess.PIPE, creationflags=0x08000000)

     if process.stderr:
        print (process.stderr.readlines())
     process.wait()
