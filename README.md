# Ray Tracing in OpenGL

This project is intended for me to learn more about raytracing

# Build
First clone using ```git clone --recursive git@github.com:Brett-Constantinoff/OpenGL-Ray-Tracing.git```

Note: For running the project I recommend using release mode

## MacOS
In the root run ```./premake/premake5 gmake```

This will create a Makefile which you can then use the build
the project using 
```make config=release```

or 

```make config=degub```

Then use ```./Debug/Raytracing``` to run the executable

## Windows
In the root run ```./premake/premake5 vs2022```

This will generate a visual studio solution file which
you can then use the build the project