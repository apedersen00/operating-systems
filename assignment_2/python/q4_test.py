import subprocess
import statistics
import matplotlib.pyplot as plt
import numpy as np

def run_and_parse(T, N):
    result = subprocess.run(
        ['../applications/build/q4', str(T), str(N)],
        capture_output=True,
        text=True,
        check=True
    )
    stdout = result.stdout

    ser_line = [line for line in stdout.splitlines() if 'Serial time' in line][0]
    par_line = [line for line in stdout.splitlines() if 'Parallel time' in line][0]

    ser_time = float(ser_line.split('time:')[-1].replace('us','').strip())
    par_time = float(par_line.split('time:')[-1].replace('us','').strip())

    return ser_time, par_time

def main():
    THREADS     = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
    ARR_SIZES   = [10000, 100000, 1000000]
    NUM_TEST    = 100

    avg_par_times = {N: [] for N in ARR_SIZES}
    stdev_par_times = {N: [] for N in ARR_SIZES}
    serial_times  = {}

    for N in ARR_SIZES:
        for T in THREADS:
            par_runs = []
            for _ in range(NUM_TEST):
                ser, par = run_and_parse(T, N)
                par_runs.append(par)
                if N not in serial_times:
                    serial_times[N] = ser
            avg_par_times[N].append(statistics.mean(par_runs))
            stdev_par_times[N].append(statistics.stdev(par_runs))
            print(f"T={T}, N={N} -> Serial: {serial_times[N]:.2f} µs, Parallel Avg: {avg_par_times[N][-1]:.2f} µs, Stdev: {stdev_par_times[N][-1]:.2f} µs")

    # Plotting
    x = np.arange(len(THREADS))
    width = 0.2
    colors = ['tab:blue', 'tab:orange', 'tab:green']

    plt.figure(figsize=(12, 6))

    for i, N in enumerate(ARR_SIZES):
        plt.bar(x + i*width - width, avg_par_times[N], yerr=stdev_par_times[N],
                width=width, color=colors[i], alpha=0.8, capsize=5, label=f"Parallel N={N}")

    for i, N in enumerate(ARR_SIZES):
        plt.axhline(y=serial_times[N], color=colors[i], linestyle='--', linewidth=2, label=f"Serial N={N}")

    plt.xticks(x, [str(T) for T in THREADS])
    plt.xlabel("Number of Threads")
    plt.ylabel("Execution Time (µs)")
    plt.title("Parallel Execution vs. Serial for Histogram Binning")
    plt.legend()
    plt.grid(axis="y", linestyle="--", alpha=0.5)

    plt.tight_layout()
    plt.savefig("q4_execution_times.png", dpi=300)

if __name__ == "__main__":
    main()