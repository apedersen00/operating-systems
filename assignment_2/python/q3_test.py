import subprocess
import statistics
import matplotlib.pyplot as plt
import numpy as np

def run_and_parse(N):
    result = subprocess.run(
        ['../applications/build/q3', str(N)],
        capture_output=True,
        text=True,
        check=True
    )
    stdout = result.stdout

    ser_line = [line for line in stdout.splitlines() if 'Serial sum' in line][0]
    par_line = [line for line in stdout.splitlines() if 'Parallel Sum' in line][0]

    ser_time = float(ser_line.split('time:')[-1].replace('us','').strip())
    par_time = float(par_line.split('time:')[-1].replace('us','').strip())

    return ser_time, par_time

def main():
    N_LIST      = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
    NUM_TEST    = 100

    avg_ser_times = []
    stdev_ser     = []
    avg_par_times = []

    for N in N_LIST:
        ser_times = []
        par_times = []
        for _ in range(NUM_TEST):
            ser, par = run_and_parse(N)
            ser_times.append(ser)
            par_times.append(par)

        avg_ser_times.append(statistics.mean(ser_times))
        stdev_ser.append(statistics.stdev(ser_times))
        avg_par_times.append(statistics.mean(par_times))

        print(f"N={N} -> Serial Avg: {avg_ser_times[-1]:.2f} µs, Parallel Avg: {avg_par_times[-1]:.2f} µs")

    x = np.arange(len(N_LIST))
    plt.bar(x, avg_par_times, yerr=stdev_ser, capsize=5, alpha=0.7, label="Parallel")

    serial_time = avg_ser_times[0]
    plt.axhline(y=serial_time, color='orange', linestyle='--', linewidth=2, label='Serial')

    plt.xticks(x, [str(N) for N in N_LIST])
    plt.xlabel("Number of Threads")
    plt.ylabel("Execution Time (µs)")
    plt.title("Parallel Execution vs. Serial")
    plt.legend()
    plt.grid(axis="y", linestyle="--", alpha=0.6)

    plt.tight_layout()
    plt.savefig("execution_times.png", dpi=300)
    plt.show()

if __name__ == "__main__":
    main()