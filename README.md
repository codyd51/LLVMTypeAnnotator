LLVMTypeAnnotator
-----------------

This is a simple LLVM pass to mangle symbols to retain the type information of arguments and return value.

It was made to familiarize myself a bit with LLVM passes.

Usage
---------------

```
$ which opt
~/llvm/bin/opt
$ opt -load LLVMTypeAnnotator.dylib -typeAnnotator < input.bc > output.bc
```

Or, with clang:
```
$ clang -Xclang -load -Xclang ./lib/LLVMTypeAnnotator.dylib -c input.c -o output.o
```

Format
----------------

The mangled symbol name is delimited by the `@` symbol. 

The first component is the original symbol name.

The second component is the return type identifier.

The third component is a series of type identifiers, representing positionally each argument to the function.

This function:

`float my_func(int arg1, char* arg2);`

Will be compiled to this symbol:

`_my_func@f@ip`

Where `_my_func` is the original symbol, `@f` indicating the function returns a `float`, and `@ip` indicating the function accepts an int and a pointer, in that order.

For the up-to-date mapping of value types to mangled identifiers, see `NameAnnotatorPass::identifierForType`.

License
---------------

MIT license.
