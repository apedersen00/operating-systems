import subprocess
import statistics
import matplotlib.pyplot as plt
import numpy as np

def run_and_parse(N):
    result = subprocess.run(
        ['../applications/build/q7', str(N)],
        capture_output=True,
        text=True,
        check=True
    )
    res = result.stdout
    res = res.split('Total time taken:')[-1]
    res = res.replace(' ', '')
    res = res.replace('us', '')
    res = res.strip()
    return float(res)

def main():
    N_LIST      = [100, 1000, 10000, 100000, 1000000]
    NUM_TEST    = 1000

    avg_times       = []
    stddev_times    = []

    for N in N_LIST:
        times = []
        for _ in range(NUM_TEST):
            times.append(run_and_parse(N))

        average_time    = statistics.mean(times)
        stdev_time      = statistics.stdev(times)

        avg_times.append(average_time)
        stddev_times.append(stdev_time)

        print(f"\nResults for N = {N}:")
        print(f"Avg. Time: {average_time:.2f} µs")
        print(f"StdDev: {stdev_time:.2f} µs")

    x = np.arange(len(N_LIST))
    plt.bar(x, avg_times, yerr=stddev_times, capsize=5, alpha=0.7)
    plt.xticks(x, [str(N) for N in N_LIST])
    plt.xlabel("N")
    plt.ylabel("Execution Time (µs)")
    plt.title("Execution Time vs. N")
    plt.grid(axis="y", linestyle="--", alpha=0.6)

    plt.tight_layout()
    plt.savefig("execution_times.png", dpi=300)
    plt.show()

if __name__ == "__main__":
    main()