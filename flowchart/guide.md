# RAFTAAR - Technical Documentation

## 1. Project Overview

**RAFTAAR** is a terminal-based typing speed application built in C++ with three typing modes.
Uses FTXUI for terminal GUI and SFML for a full statistics dashboard.

### Build System

- CMake build system
- Libraries: FTXUI, SFML
- Entry Point File: `main.cpp`

### File Structure

- `main.cpp` - Entry point and main menu
- `mode_utils.cpp/.hpp` - Shared utility functions
- `mode_words.cpp/.hpp` - Words Mode (50 words)
- `mode_equations.cpp/.hpp` - Equations Mode (10 equations)
- `mode_timed.cpp/.hpp` - Timed Mode (60 seconds)
- `mode_stats.cpp/.hpp` - Statistics dashboard

---

## 2. Shared Utilities (`mode_utils`)

### `calculate_wpm()`

Calculates Words Per Minute from correct words and elapsed time. Formula: `(correct_words * 60) / elapsed_seconds`

### `get_random_words(int count)`

Loads random words from `assets/words_alpha.txt` and returns requested number of words.

---

## 3. Words Mode

**Description**: Type 50 fixed words as quickly as possible

**Flow**:

1. Generate 50 random words
2. Display all words, highlight current in yellow
3. User types and presses SPACE to submit
4. Word turns green (correct) or red (incorrect)
5. Real-time WPM and accuracy updates
6. Game ends after 50 words

**Key Variables**:

- `FIXED_WORD_COUNT = 50`
- `current_index` - Current word position
- `correct_words` - Correctly typed count
- `word_correctness[]` - Tracks each word's result

---

## 4. Equations Mode

**Description**: Type 10 mathematical equations with LaTeX notation

**Flow**:

1. Generate 10 random equations via `generate_equations(10)`
2. Display equations horizontally
3. User types exact equation and presses SPACE
4. Equation turns green/red based on correctness
5. Calculate WPM and accuracy

### `generate_equations(int count)`

Generates random math equations from 10 types:

- Quadratic equations (e.g., `x^2+5x+3=0`)
- Derivatives (e.g., `\frac{d}{dx}[15x^4]`)
- Limits, logarithms, trigonometric functions
- Uses random constants: a (1-48), b (1-48), c (1-20), n (2-5)

**Key Variables**:

- `FIXED_EQUATION_COUNT = 10`

---

## 5. Timed Mode

**Description**: Type as many words as possible in 60 seconds

**Flow**:

1. Load initial 50 words
2. Display 5 lines × 8 words per line
3. Timer starts on first SPACE press
4. Auto-loads more words when approaching end
5. Words scroll up as user progresses
6. Game ends at 0 seconds
7. Display final statistics

### `create_falling_words()`

Creates 5-line word grid with color coding for past/current/future words. Implements scrolling by adjusting `visible_start` index.

### `get_remaining_time()`

Calculates seconds remaining by comparing current time to start time.

**Key Variables**:

- `DURATION_SECONDS = 60`
- `WORDS_BATCH_SIZE = 50`
- `visible_start` - First visible word index (for scrolling)

**Scrolling Logic**: When `current_index >= visible_start + 30`, shift `visible_start` to `current_index - 10`

---

## 6. Statistics Page

**Description**: SFML window displaying typing statistics

**Current Implementation**:

- Opens 1280×720 window
- Loads font from `assets/arial.ttf`
- Placeholder text "Stats coming soon..."

**Planned**: Performance graphs, historical data, progress tracking

---

## 7. UI Components

### Color Scheme

- **Yellow**: Current word/equation
- **Green**: Correct
- **Red**: Incorrect

### Other Component Things

1. Title bar (mode name)
2. Content display area
3. Stats box (WPM and Accuracy)
4. Input field ("TYPE HERE...")
5. "GO BACK" button

---

## 8. Input Handling

### Event System

Uses FTXUI's `CatchEvent()` to capture SPACE key:

1. First press starts timer
2. Subsequent presses submit input, check correctness, update stats, advance to next item

---

## 9. Statistics Calculation

**WPM**: `(correct_words × 60) / elapsed_seconds`

**Accuracy**: `(correct_words / total_typed) × 100`

Both update in real-time after each submission.

---

## 10. User Guide

### Main Menu

1. Launch application (fullscreen terminal)
2. Use TAB to navigate, ENTER to select
3. Five options: Words Mode, Equation Mode, Timed Mode, Open Dashboard, Quit

### Playing Modes

1. Select mode from menu
2. Type in input field
3. Press SPACE to submit each word/equation
4. Watch color feedback (green/red)
5. Click "GO BACK" when finished
