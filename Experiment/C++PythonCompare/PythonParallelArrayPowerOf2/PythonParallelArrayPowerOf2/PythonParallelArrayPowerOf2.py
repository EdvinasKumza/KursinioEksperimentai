import random
import time
import os
import concurrent.futures

def kvadratu(d):
    return [x*x for x in d]

def main():
    n = 10_000_000
    data = [random.randint(1, 10_000) for _ in range(n)]
    num_workers = os.cpu_count()
    iterations = 100
    total_time_ms = 0.0
    chunk = n // num_workers

    with concurrent.futures.ProcessPoolExecutor(max_workers=num_workers) as executor:
        for _ in range(iterations):
            start = time.perf_counter()

            futures = []
            for i in range(num_workers):
                start_idx = i * chunk
                end_idx = n if i == num_workers - 1 else (i + 1) * chunk
                futures.append(executor.submit(kvadratu, data[start_idx:end_idx]))

            results = []
            for f in futures:
                results.extend(f.result())

            end = time.perf_counter()
            total_time_ms += (end - start) * 1000

    avg_time = total_time_ms / iterations
    print(f"Vidutinis lygiagretaus vykdymo laikas: {avg_time:.2f} ms")

if __name__ == "__main__":
    main()
