
## Operating System Simulation

### Description

This project is an educational simulation of operating system concepts implemented using C and GTK+ 3.0 for the graphical user interface. The simulation provides a visual and interactive way to understand various aspects of operating systems, including process scheduling, memory management, and other key functionalities. Additionally, the simulation includes various utilities and features typically found in an operating system environment.

### Features

- **Clock**: Displays the current system time.
- **Background Music**: Plays background music during the simulation.
- **Calculator**: A simple calculator for basic arithmetic operations.
- **Notepad**: A basic text editor for creating and editing text files.
- **Create File**: Functionality to create new files within the simulated environment.
- **Copy File**: Allows users to copy files from one location to another.
- **Delete File**: Enables users to delete files.
- **Move File**: Functionality to move files between directories.
- **File Info**: Provides detailed information about selected files.
- **Print File**: Simulates printing a file.
- **Game**: Includes a Minesweeper game to demonstrate interactive applications within the OS.
- **Shutdown**: Simulates shutting down the operating system.

### Requirements

- GTK+ 3.0
- GCC (GNU Compiler Collection)

### Installation

1. **Install GTK+ 3.0**:
    - On Ubuntu/Debian:
        ```bash
        sudo apt-get update
        sudo apt-get install libgtk-3-dev
        ```

2. **Clone the Repository**:
    ```bash
    git clone https://github.com/twonum/os-simulation.git
    cd os-simulation
    ```

3. **Compile the Simulation**:
    ```bash
    gcc -o os_simulation os_simulation.c `pkg-config --cflags --libs gtk+-3.0`
    ```

### Usage

1. **Run the Simulation**:
    ```bash
    ./os_simulation
    ```

2. **Start a New Simulation**:
    - Configure the parameters of the simulation such as the number of processes, memory size, scheduling algorithm, etc.
    - The main window will display the simulation with visual representations of processes and memory.

3. **Interactive Controls**:
    - Start, pause, and reset the simulation.
    - Use the provided utilities like the calculator, notepad, and file management tools.
    - Play the Minesweeper game for a fun break.
    - View detailed statistics and logs of the simulation.

### Contributing

Contributions are welcome! Please feel free to submit a Pull Request or open an Issue if you have any suggestions or improvements.

### License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

### Author

- [twonum](https://github.com/twonum)

