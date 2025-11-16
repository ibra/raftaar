# RAFTAAR

A responsive, terminal-based typing application built from scratch in C++. Submission for CS100 semester project. The goal was provide a powerful, clean interface, real-time feedback, and multiple ways to measure typing performance. The project mixes UI design, input handling, data management and visualisation into one cohesive application.

## MODES

### **Words Mode**

The main mode. It's a fixed-length accuracy test. Random words are loaded from an external dataset, and the UI highlights progress word by word. Correct and incorrect submissions are color-coded immediately.

### **Timed Mode**

A paced challenge where the interface streams new words as you advance. There's an infinite amount of words and the display scrolls infinitely, and the stats update every time you submit a word. This mode highlights the real-time rendering and state-management part of the project.

### **Equation Mode**

Randomly assembled math expressions: quadratics, derivatives, logs, limits etc. These are formatted in Webwork syntax, so the user has to match the exact structure. It uses parameterized random generation to produce valid mathematical expressions. By varying coefficients, operators, and structures, each test run feels unique while still being predictable enough to type accurately.

### **Stats Dashboard**

A separate graphical window that displays results, session data, and personal bests. Built with SFML to show a shift from terminal-based GUI rendering to an even more proper 2D graphics environment.

## FEATURES

### **Real-time Typing Engine**

Each mode runs on a shared typing-state system that tracks input, correctness, timestamps, and progress. The interface updates instantly on every submission, recalculating WPM, accuracy, and visual state without blocking the UI.

### **Accurate Timer System**

Timing is handled through a dedicated timing routine that runs independently of the rendering loop. We had to create a new thread to make the timer realtime. This keeps elapsed time accurate even when the UI is busy drawing or scrolling. The timer starts on the first SPACE press and updates WPM in real time, so users never see inflated or frozen numbers.

### **Dynamic Highlighting**

Words and equations react immediately to user input. The current item is highlighted as you type, and once submitted, it flips to green or red based on correctness. The color updates happen inside the rendering cycle, so feedback is instant and consistent across all modes.

### **Infinite Word Grid**

Timed mode uses a virtual word buffer with a shifting viewport. As the user approaches the end of the visible set, the display scrolls upward and loads new words behind the scenes. This gives the user the feeling of an infinite test without stuttering or running out of words.

### **Custom UI Components**

Layout, headers, stat boxes, and content windows are built from custom FTXUI components. You can find them in `src/ui`. They keep every mode visually consistent and make it easy to reorganize the interface without rewriting logic.

### **Persistent Test History**

After each test, results are written to a CSV file. The stats dashboard loads this data, aggregates it, and displays averages, personal bests, and a list of recent attempts. This adds persistence to an otherwise terminal-based application.

### **Graphics-Driven Dashboard**

The stats page uses SFML to render a separate graphical window with a card-based layout system, and a themed color palette that we chose. It presents typing performance in a clean, visually readable format that complements the terminal UI.

## BUILDING THE PROJECT

We used CMake as a build system for the project. This not only makes building the project with the libraries extremely easy, but also means that the project is cross-platform. I wrote the code for it and tested it on
both Windows and Linux (Debian).

Create a build folder:

```bash
mkdir build
cd build
```

Generate build files using CMake:

```bash
cmake -G "Visual Studio 17 2022" -A x64 ..
```

Compile:

```bash
cmake --build . --config Release
```
