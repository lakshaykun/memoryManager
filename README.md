
# Memory Management System

## Objective

Develop a memory management system in C/C++ that fulfills the following specifications:

- **Memory Manager**: Manages physical pages, allocating pages upon request.
- **Task**: Each instance has its own page table and requests memory from the memory manager based on a trace file.
- **Performance Module**: Computes memory usage, including page tables, allocated memory, and available memory, and reports execution times and performance metrics.
- **Input/Output Module**: Reads a trace file, invoking tasks based on the file contents.

## Specifications

### Components

1. **Memory Manager**:
   - Maintains records of available physical pages.
   - Allocates physical pages based on requests.

2. **Task**:
   - Multiple instances with their own page tables.
   - Requests memory from the Memory Manager using a trace file.
   
3. **Performance Module**:
   - Computes memory requirements for page tables.
   - Reports allocated memory and available (free) physical memory.
   - Measures page hits and misses for each task.
   
4. **Input/Output Module**:
   - Reads trace files with tasks' memory requests.
   - Processes addresses in hexadecimal format with sizes in KB or MB.

5. **Test Module**:
   - Generates traces based on configurations.
   - Tests memory management performance with generated data.

### Features

- Multiple implementations of page tables:
  - Map: Uses a map with logical page numbers as keys and physical page numbers as values.
  - Single Level Page Table.
  - Multi-Level Page Table: Configurable entries defined in `config.h`.

- Configurable parameters:
  - Page sizes
  - Virtual and physical memory sizes

## File Structure

- `include/`:
  - `config.hpp`: Configuration file defining parameters like page sizes and memory sizes.
  - `memoryManager.hpp`: Header for the memory manager class.
  - `task.hpp`: Header for the task class.
  - `taskManager.hpp`: Header for the task manager class.

- `src/`:
  - `input_output.cpp`: Main source file for input, output, and CSV generation.

- `test/`:
  - `test.cpp`: Test module for generating and testing traces.

- `traces/`:
  - `trace.txt`: Example trace file for testing.

- `output/`:
  - `map.csv`: CSV file for map-based page table implementation.
  - `single_level.csv`: CSV file for single-level page table implementation.
  - `multilevel.csv`: CSV file for multi-level page table implementation.

## Compilation and Running

### Compilation

To compile the project, use the provided `Makefile`. The `Makefile` includes targets to build the application, run tests, and clean up generated files.

```bash
make
```

This command will compile the source files and generate executables in the `./bin/` directory.

### Running

To run the application:

```bash
make run
```

To execute tests:

```bash
make test
make trace
```

To clean up generated files:

```bash
make clean
```

### Example Usage

1. Compile the project:
   ```bash
   make
   ```

2. Run the application:
   ```bash
   make run
   ```

3. Generate and test traces:
   ```bash
   make test
   make trace
   ```

4. Clean up:
   ```bash
   make clean
   ```

## Additional Information

- **Memory Management Details**: The memory manager handles page allocations and maintains records of available pages. Tasks request memory through this manager, and their performance is measured by the performance module.

- **CSV Output**: The generated CSV files contain detailed metrics about the page tables and memory usage for different implementations.

---

Feel free to adjust any part of this `README` file to better fit your project's specifics or additional requirements.