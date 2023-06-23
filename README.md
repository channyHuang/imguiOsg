# imguiOsg
imgui + osg implement ui and scene display.

MacBook Pro (13-inch, 2019) do not support glsl130, so use 120 instead.

```c++
ERROR: ImGui_ImplOpenGL3_CreateDeviceObjects: failed to compile vertex shader! With GLSL: #version 130

ERROR: 0:1: '' :  version '130' is not supported
```

# OpenGL 版本
默认cmake和xcode生成osg的lib后，opengl只支持120，即需要在ImGui初始化时指定版本号为120才能正常显示。
```c++
ImGui_ImplOpenGL3_Init("#version 120")
```
而部分Mac最高可以支持410.  
对此，可通过修改cmake的配置解决。
```
OSG_GL3_AVAILABLE 设置为True（勾选上）

OSG_GL_CONTEXT_VERSION 设置具体版本，如设置为4.1
```
为一步到位，直接设置了4.1.然而改为410后又出现warning:
```c++
Warning: detected OpenGL error 'invalid enumerant' after applying GLMode 0xb50
Warning: detected OpenGL error 'invalid enumerant' after applying GLMode 0x4000
Warning: detected OpenGL error 'invalid operation' after applying attribute Material 0x7fbea2411eb0
Warning: detected OpenGL error 'invalid operation' after applying attribute LightModel 0x7fbea24177f0
```

又需要重新修改cmake设置
```
OSG_GL_FIXED_FUNCTION_AVAILABLE 设置为False（去掉勾选）
```

最后才能够正常支持410.

# 其它
Path in cmake refer to project commonClass.