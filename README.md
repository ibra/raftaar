# RAFTAAR

Clean, interactive typing tests in the terminal.

## FEATURES

- timed mode: type as many words as you can.
- words mode: type a fixed set of random words.
- equation mode: type a fixed, randomly generated equation.
- stats: view your stats including tests taken and personal bests.

> we're using CMAKE for building the project. all source files are in the src directory.

## BUILDING THE PROJECT

Create the build directory and navigate to it.

```
mkdir build
cd build
```

Then run:

```
cmake -G "Visual Studio 17 2022" -A x64 ..
```

And then:

```
cmake --build . --config Release
```
