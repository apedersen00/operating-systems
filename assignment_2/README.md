# Operating Systesm - Assignment 1

## Structure

- `applications/` - C applications for questions 3, 4, and 5
- `python/` - Python scripts
- `report/` - LaTeX report

## Report

The report is found [here](Assignment2_Group61.pdf).

## Building Applications

Individual applications can be built using:

```bash
applications> make q3
applications> make q4
applications> make q5
```

Built executables will be located in `applications/build/`.

## Building Report

The report is made using LaTeX. To build the PDF:

```bash
cd report
make
```

The generated PDF will be `report/build/main.pdf`.
