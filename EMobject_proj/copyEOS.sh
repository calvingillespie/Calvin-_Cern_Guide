#!/bin/bash

# Base EOS directory
EOS_BASE="/store/user/cgillesp/DoubleEG/2016_Dataset_V2_MC"

# Local destination directory on cmslpc
LOCAL_DEST="/store/user/cgillesp/2016_Reduced_Dataset_MC"  # Replace with your desired local path

eosrm -r "$LOCAL_DEST"
eosmkdir "$LOCAL_DEST"

#Name number
NAME_NUM = 0
# Create the destination directory if it doesn't exist
mkdir -p "$LOCAL_DEST"

# List all directories in 2016_Dataset
SUBDIRS=$(eosls $EOS_BASE)

# Loop through each subdirectory
for SUBDIR in $SUBDIRS; do
    # Define the path to the 0000 directory
    EOS_0000_PATH="$EOS_BASE/$SUBDIR/0000"


    #####################################################################
    : << 'EOF'
    # List all .root files in the 0000 directory
    FILE_LIST=$(eosls $EOS_0000_PATH | grep '\.root$')
    
    if [ -z "$FILE_LIST" ]; then
        echo "No .root files found in $EOS_0000_PATH. Skipping."
        continue
    fi
    
    # Loop through and copy each file
    for FILE in $FILE_LIST; do
        # Construct the full EOS path
        EOS_PATH="root://cmseos.fnal.gov/$EOS_0000_PATH/$FILE"
        
        # Copy to the local destination
        #echo "Copying $EOS_PATH to $LOCAL_DEST/"
        #xrdcp "$EOS_PATH" "root://cmseos.fnal.gov/$LOCAL_DEST/"
        
        BASE_NAME=$(basename "$FILE" .root)
        NEW_FILE_NAME="${BASE_NAME}_${NAME_NUM}.root"

        # Increment NAME_NUM for the next file
        NAME_NUM=$((NAME_NUM + 1))

        # Construct the full destination path in EOS with the new name
        DEST_PATH="root://cmseos.fnal.gov/$LOCAL_DEST/$NEW_FILE_NAME"

        # Copy the file to the new destination with the new name
        echo "Copying $EOS_PATH to $DEST_PATH"
        xrdcp "$EOS_PATH" "$DEST_PATH"
        
    done
EOF
    ####################################################################

    FIRST_FILE=$(eosls "$EOS_0000_PATH" | grep '\.root$' | head -n 1)
    if [ -z "$FIRST_FILE" ]; then
        echo "No .root files found in $EOS_0000_PATH. Skipping."
        continue
    fi
    # Construct the full EOS path
    EOS_PATH="root://cmseos.fnal.gov/$EOS_0000_PATH/$FIRST_FILE"

    # Construct the new file name
    BASE_NAME=$(basename "$FIRST_FILE" .root)
    NEW_FILE_NAME="${BASE_NAME}_${NAME_NUM}.root"

    # Increment NAME_NUM for the next file
    NAME_NUM=$((NAME_NUM + 1))

    # Copy the file to the local destination
    echo "Copying $EOS_PATH to $LOCAL_DEST/$NEW_FILE_NAME"
    xrdcp "$EOS_PATH" "root://cmseos.fnal.gov/$LOCAL_DEST/$NEW_FILE_NAME"

done

# Merging the files into one
echo "Merging all .root files into combined_2016_dataset.root..."

echo "All files have been merged into combined_2016_dataset.root."
