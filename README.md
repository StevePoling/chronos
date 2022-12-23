 # Remember

When you do a Brew Upgrade, you may change versions of cmake, and/or the MaxOSX sdk
```
cmake  ⇒ ../Cellar/cmake/3.24.2/bin/cmake
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk
```

It'll cause make to fail saying it can't find the old version of cmake.

You'll thus want to run this command: 

```zsh
cmake --build . --target clean
```

This will romance cmake into tidying up all your CMakeCache.txt and Makefile version identifiers.

You want to set up the lldb-mi. See https://code.visualstudio.com/docs/cpp/lldb-mi


```zsh
ls -lt ~/.vscode/extensions/ms-vscode.cpptools-1.12.4-darwin-x64/debugAdapters/lldb-mi/bin/lldb-mi
cp ~/.vscode/extensions/ms-vscode.cpptools-1.12.4-darwin-x64/debugAdapters/lldb-mi/bin/lldb-mi \
   /Applications/Xcode.app/Contents/SharedFrameworks/LLDB.framework
```

Edit launch.json to add
```json
"miDebuggerPath": "/Applications/Xcode.app/Contents/SharedFrameworks/LLDB.framework/lldb-mi"
```

