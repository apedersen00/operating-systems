# Operating Systesm - Assignment 1

## Structure

- `applications/` - C applications for questions 2, 4, 6, 7, and 8
- `python/` - Python scripts
- `report/` - LaTeX report

## Report

The report is found [here](Lab1_Group61.pdf).

## Building Applications

To build all C applications:

```bash
cd applications
make
```

Individual applications can be built using:

```bash
make q2    # Question 2 - fork() demonstration
make q6    # Question 6 - threads and processes
make q7    # Question 7 - parallel array summation
make q8_1  # Question 8 - FIFO producer
make q8_2  # Question 8 - FIFO consumer
```

Built executables will be located in `applications/build/`.

## Running Applications

After building, run applications from the `applications` directory:

```bash
./build/q2
./build/q6
./build/q7 1000        # Specify array size N
./build/q8_1 &         # Run producer in background
./build/q8_2           # Run consumer
```

## Building Report

The report is made using LaTeX. To build the PDF:

```bash
cd report
make
```

The generated PDF will be `report/build/main.pdf`.
