# Remember

When you do a Brew Upgrade, you may change versions of cmake, and/or the MaxOSX sdk
```
cmake  ⇒ ../Cellar/cmake/3.24.2/bin/cmake
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk
```

You'll thus want to run this command: 

```zsh
cmake --build . --target clean
```

This will romance cmake into tidying up all your CMakeCache.txt and Makefile version identifiers.
