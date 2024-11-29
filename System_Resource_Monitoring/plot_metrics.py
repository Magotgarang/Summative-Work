# plot_metrics.py
import matplotlib.pyplot as plt

def read_metrics_from_file():
    timestamps = []
    cpu_usage = []
    memory_usage = []
    network_usage = []

    with open("usage.txt", "r") as file:
        for line in file:
            parts = line.strip().split(", ")
            timestamp = parts[0].split(": ")[1]
            cpu = float(parts[1].split(": ")[1][:-1])  # Remove '%' from CPU usage
            memory = float(parts[2].split(": ")[1][:-1])  # Remove '%' from memory usage
            network = float(parts[3].split(": ")[1].split(" ")[0])  # Network usage in KB

            timestamps.append(timestamp)
            cpu_usage.append(cpu)
            memory_usage.append(memory)
            network_usage.append(network)

    return timestamps, cpu_usage, memory_usage, network_usage

def plot_metrics():
    timestamps, cpu_usage, memory_usage, network_usage = read_metrics_from_file()

    plt.figure(figsize=(10, 6))
    plt.plot(timestamps, cpu_usage, label="CPU Usage (%)", color="blue")
    plt.plot(timestamps, memory_usage, label="Memory Usage (%)", color="green")
    plt.plot(timestamps, network_usage, label="Network Usage (KB)", color="red")

    plt.xlabel("Time (hh:mm:ss)")
    plt.ylabel("Usage")
    plt.title("System Resource Usage Over Time")
    plt.legend()
    plt.xticks(rotation=45)
    plt.tight_layout()

    plt.show()

if __name__ == "__main__":
    plot_metrics()
