# SortTV

Short for Sorting Terminal Visualizer. An experimental project to visualize sorting algorithms directly in your terminal, written in C.
 
Currently supports: **Bubble**, **Insertion**, **Selection**, **Merge**, and **Quick** sort.

## Installation

### Linux / WSL

```
git clone https://github.com/eclizs/SortTV.git
cd SortTV
make
./bin/SortTV
```

### Windows
 
Download the latest `SortTV-windows.exe` from the [Releases](https://github.com/eclizs/SortTV/releases) page and run it from a terminal (CMD or PowerShell).
 
## Usage
 
```
./bin/SortTV [OPTIONS]
```
 
| Option | Description |
|---|---|
| `-s, --sort [NAME]` | Run a specific algorithm (bubble, insertion, selection, merge, quick) |
| `-b, --benchmark=[SIZE]` | Benchmark all algorithms, no visualization (default size: 10000) |
| `-w, --width=[WIDTH]` | Set visualization width (default: terminal width) |
| `-e, --height=[HEIGHT]` | Set visualization height (default: terminal height) |
| `-i, --interval=[SECONDS]` | Set delay between steps in seconds (default: 0.01) |
| `-h, --help` | Display help message |
 
### Examples
 
```bash
./bin/SortTV                        # Visualize all algorithms in sequence
./bin/SortTV -s bubble              # Visualize bubble sort only
./bin/SortTV -b=5000                # Benchmark all algorithms on 5000 elements
./bin/SortTV -s quick -i 0.05       # Quick sort with slower animation
```
 
## Adding a New Algorithm
 
Adding a new sorting algorithm is straightforward. The only requirement is calling `VISUALIZE(arr, size, intervalInSeconds)` at each step where you want the array state rendered.
 
## Building from Source (Windows)
 
Install [MSYS2](https://www.msys2.org/), then from the MSYS2 terminal:
 
```bash
pacman -S mingw-w64-x86_64-gcc make
git clone https://github.com/eclizs/SortTV.git
cd SortTV
make
```




