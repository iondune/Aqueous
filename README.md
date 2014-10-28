# Readme

## Setup

### MATLAB Compiler Runtime (MCR)

For interoperability with MATLAB `.mat` files, [MCR](http://www.mathworks.com/products/compiler/mcr/) must be installed. The version current used is [R2014b (8.4)](http://www.mathworks.com/supportfiles/downloads/R2014b/deployment_files/R2014b/installers/win64/MCR_R2014b_win64_installer.exe). After installation, make sure that the
win64 bin directory is in your PATH, e.g.:

```
C:\Program Files\MATLAB\MATLAB Compiler Runtime\v84\bin\win64
```

and that the post-build event in the `.vcxproj` is copying `libmat.dll` from the same correct directory. You may also need to reboot.