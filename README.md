Cool_ocoo
---

This is Cool_ocoo - an implementation of the [OCOO esoteric programming language](https://esolangs.org/wiki/OCOO).

Check `./src` for the source files. The Cool_ocoo program strictly conforms to the ISO/IEC 9899:2011 (AKA C11) standard. So if you have any C11 implementation, just translate the following C source files: `./src/cool_ocoo.c`, `./src/clean_up_routines.c`, `./src/evaluate.c`, `./src/lexer.c`, `./src/parser.c`, `./src/write_error.c`, and link them.

On most platforms, you can pass a source file to Cool_ocoo like so [^1]:

```
<path_to_cool_ocoo> --input <path_to_source_file>
```

If a flag is passed more than once, the behavior is as if the last instance of that flag overrides all the previous ones, unless it's explicitly documented to behave otherwise. For example, doing

```
<path_to_cool_ocoo> --input a.oc --input b.oc
```

will actually use the source file `b.oc` as the input rather than `a.oc`.

[^1]: Any source file you pass to Cool_ocoo is treated as a _byte-oriented_ stream (ISO/IEC 9899:2011 §7.21.2 4), and the behavior is as if each item in the file is read with a call to the `fgetc` function. So make sure that the representations of those items fit in an `unsigned char`.
