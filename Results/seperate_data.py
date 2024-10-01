import pandas as pd

pd.set_option('display.max_rows', None)

def output_single_column_without_index(file_path, column_index):
    try:
        # Read the CSV file
        df = pd.read_csv(file_path, header=None)
        
        # Check if the specified column exists
        if column_index >= len(df.columns):
            raise ValueError(f"Invalid column index. Number of columns: {len(df.columns)}")
        
        # Get the selected column
        selected_column = df.iloc[:, column_index]
        
        # Display all rows without index
        print(selected_column.to_string(index=False))
        
    except FileNotFoundError:
        print(f"Error: File '{file_path}' not found.")
    except Exception as e:
        print(f"An error occurred: {str(e)}")

# Example usage
file_path = 'data.csv'

output_single_column_without_index(file_path, 0)

print()
print()
print()

output_single_column_without_index(file_path, 1)

print()
print()
print()

output_single_column_without_index(file_path, 2)

print()
print()
print()

output_single_column_without_index(file_path, 3)