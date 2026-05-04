import re

# Define a function to extract the module name and coordinates
def extract_module_coordinates():
	
	# Path to the text file containing the module data
	file_path = "/cmsnfshome0/nfshome0/cgillesp/phase1_det_data_with_alignment_2018.out"
	# Read the text file
	with open(file_path, 'r') as file:
		text = file.read()
	# Regular expression to find the module name and coordinates
	pattern = r"(BPix_\S+).*?r/phi/z = ([\d\.\-]+)/([\d\.\-]+)/([\d\.\-]+)"
	# Find all matches in the text
	matches = re.findall(pattern, text, re.DOTALL)
	# Create a list of dictionaries with the extracted data
	extracted_coordinates = [{'module': match[0], 'r': float(match[1]), 'phi': float(match[2]), 'z': float(match[3])} for match in matches]
	
	for entry in extracted_coordinates:
		print(entry)


	return extracted_coordinates

extract_module_coordinates()
