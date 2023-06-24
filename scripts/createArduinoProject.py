import os
import shutil

# Program to copy all .h and .cpp files from current directory to the arduino directory

# Get the path of the file
path = os.path.dirname(os.path.realpath(__file__))
projectRoot = os.path.abspath(os.path.join(path, '..'))

# Get the name of the project
projectName = os.path.basename(projectRoot)

# Get the path of the arduino directory
arduinoPath = os.path.join(projectRoot, 'arduino')

# Check if any directory exists inside the arduino directory with the same name as the project: arduinopath/projectname
if not os.path.isdir(os.path.join(arduinoPath, projectName)):
    os.mkdir(os.path.join(arduinoPath, projectName))

for file in os.listdir(os.path.join(projectRoot, 'src')):
    if file.endswith('.cpp') or file.endswith('.h'):
        shutil.copy(os.path.join(projectRoot, 'src', file), os.path.join(arduinoPath, projectName, file))

for file in os.listdir(os.path.join(projectRoot, 'include')):
    if file.endswith('.cpp') or file.endswith('.h'):
        shutil.copy(os.path.join(projectRoot, 'include', file), os.path.join(arduinoPath, projectName, file))

os.rename(os.path.join(arduinoPath, projectName, 'main.cpp'), os.path.join(arduinoPath, projectName, projectName + '.ino'))
