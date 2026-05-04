import os
import re

# Run this program to go over all list and to enact submission

# Input file containing dataset names
datafile = "MClist.list"

os.system("rm -r crabproj/")

# Open the datafile and read lines
with open(datafile, 'r') as f:
    dataset_names = [line.strip() for line in f if line.strip()]  # Remove whitespace and skip empty lines

# Iterate over each dataset name
for dataset_name in dataset_names:
    # Replace spaces (if any) in the dataset name with underscores for safer usage


    if dataset_name.endswith(".root"):
        dataset_name = dataset_name[:-5]

    safe_name = dataset_name.replace(" ", "_")
    safe_name = re.sub(r'[^a-zA-Z0-9_\-]', '', safe_name)  # Keep only valid characters
    
    # Create a request name
    request_name = f"SubDataset_{safe_name}"
    

    # Generate a CRAB config 
    with open('CRABconfig_template.py', 'r') as template:
        crab_config = template.read()
        crab_config = crab_config.replace("__REQUEST_NAME__",f"'{request_name}'")
        crab_config = crab_config.replace("__SCRIPT_ARGS__", f"['param={dataset_name}']")

    # Write the modified CRAB config to a file
    with open('TEMP_crabconfig.py', 'w') as crab_file:
        crab_file.write(crab_config)

    # Submit the CRAB job
    os.system("crab submit -c TEMP_crabconfig.py")
    print(f"Submitted CRAB job for dataset: {dataset_name}")

