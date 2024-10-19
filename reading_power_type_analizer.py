import numpy as np

def compute_stats(values):
    try:
        # Convert the string of comma-delimited values into a list of floats
        values = [float(x) for x in values.split(",") if x.strip()]
        
        if not values:
            print("Error: No valid numeric data found.")
            return

        # Compute basic statistics
        mean_val = np.mean(values)
        min_val = np.min(values)
        max_val = np.max(values)
        
        # Compute errors from the mean
        min_error_from_mean = mean_val - min_val
        max_error_from_mean = max_val - mean_val
        
        # Compute the average error (mean absolute error)
        abs_errors = [abs(val - mean_val) for val in values]
        avg_error = np.mean(abs_errors)

        # Compute the distribution (histogram)
        hist, bin_edges = np.histogram(values, bins='auto')
        
        # Print the results
        print(f"Mean: {mean_val}")
        print(f"Min: {min_val}")
        print(f"Max: {max_val}")
        print(f"Min Error from Mean: {min_error_from_mean}")
        print(f"Max Error from Mean: {max_error_from_mean}")
        print(f"Average Error from Mean: {avg_error}")
        print('-' * 50)
    
    except ValueError as e:
        print(f"Error: {e}")

def process_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    dataset_name = None

    for line in lines:
        line = line.strip()

        # Skip empty lines and separator lines
        if not line or line.startswith('----------------'):
            continue

        # If it's the dataset name
        if not dataset_name:
            dataset_name = line
        else:
            # Process data when data line is found
            print(f"\nDataset: {dataset_name}")
            compute_stats(line)
            dataset_name = None  # Reset to look for the next dataset

if __name__ == "__main__":
    # Specify the file path
    file_path = input("Enter the path to the file: ")
    
    # Process the file and compute statistics for each dataset
    process_file(file_path)
