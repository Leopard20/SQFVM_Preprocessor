# SQFVM_Preprocessor
This is a preprocessor for Arma's SQF language, based on SQF-VM's preprocessor.

# Usage
This extension allows you to preprocess SQF files.
Note that there shouldn't be any `#include`s, and if you have any they should be handled in the game.
```sqf
"SQFVM_Preprocessor" callExtension ["p", ["code"]]
```

# Build
Simply clone the repository using Visual Studio 2019's built-in Git feature and build the solution.
If you want to clone using Git, make sure to clone recursively to include the submodules.
