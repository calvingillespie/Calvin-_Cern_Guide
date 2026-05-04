
import subprocess
#############################################################################################
# Assemble Roc Map
head_cmd = (
    'ssh srv-s2b18-31-01 '
    '"sudo -u pixelpro -H zsh -c \''
    'cd /nfspixelraid/nfspixelraid/data1/Pix/Run_10000; '
    'head -n 30000 Run_10009/Readbacker.dat'
    '\'"'
)

result = subprocess.run(head_cmd, shell=True, capture_output=True, text=True)
mapping_output = result.stdout.splitlines()

#Debug
if result.returncode != 0:
    print("SSH command 1 failed!")
    print(result.stderr)
else:
    print("SSH command succeeded")

#############################################################################################
roc_map = {}
for line in mapping_output:
    if line.startswith("# FPix") or line.startswith("# BPix"):
        parts = line.strip().split()
        roc_name = parts[1]  # remove '#'
        roc_id = parts[2]
        roc_map[roc_id] = roc_name
        print(roc_id , " , " , roc_name)

#Debug
print(f"Loaded {len(roc_map)} ROC names.")



#############################################################################################
# Assemble Rawdata files from all Runs
tail_cmd = (
    'ssh srv-s2b18-31-01 '
    '"sudo -u pixelpro -H zsh -c \''
    'cd /nfspixelraid/nfspixelraid/data1/Pix/Run_10000; '
    'for dir in */; do '
    'echo \\"===== CHECKING \$dir =====\\"; '
    'if [ -f \\"\$dir/Readbacker__META__.txt\\" ] && [ -f \\"\$dir/Readbacker.dat\\" ]; then '
     #'echo \\"=== PASS CONDITION ===\\"; '
    'grep \\"Meta file creation timestamp\\" \\"\$dir/Readbacker__META__.txt\\"; '
    'tail -n +29299 \\"\$dir/Readbacker.dat\\"; '
    'fi; '
    'echo \\"---------------------------\\"; '
    'done\'"'
)

result = subprocess.run(tail_cmd, shell=True, capture_output=True, text=True)
#Debug 
#print("Result: ", result);

if result.returncode != 0:
    print("SSH command 2 failed!")
    print(result.stderr)
else:
    print("SSH command 2 succeeded")

#Debug
#print("Return code:", result.returncode)
#print("STDERR:", result.stderr[:500])
#print("STDOUT preview:", result.stdout[:5000])

output_lines = result.stdout.splitlines()
#############################################################################################
#Parse output_lines
final_lines = []

for line in output_lines:
    if line and line[0].isdigit():  # ROC data line
        parts = line.strip().split()
        roc_id = parts[0]
        if roc_id in roc_map:
            parts[0] = roc_map[roc_id]
        final_lines.append(" ".join(parts))
    elif line and line[0] == "#":
        continue
    else:  # timestamp lines, separators
        final_lines.append(line)

# === Step 4: Write to file ===
with open("ParsedRocFile.txt", "w") as f:
    f.write("\n".join(final_lines))

print("Finished writing ParsedRocFile.txt")
