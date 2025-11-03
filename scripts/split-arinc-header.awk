#!/usr/bin/env -S awk -f

# not the nicest, should be rather on the comment ..

/^#ifndef APEX_/ {
  # Close the previous output file if open
  if (out) close(out)

    # Extract the name part after APEX_
    match($0, /^#ifndef APEX_([A-Z0-9_]+)/, arr)
    name = tolower(arr[1])
    # Capitalize first letter
    name = toupper(substr(name, 1, 1)) substr(name, 2)

    # Compose output filename
    out = "a653" name ".h"

    # Print the current line to the new output file
    print $0 > out
    next
}

# mark all functions to be importend from the arinc module
/^#define APEX_([A-Z0-9_]+)/ {
  print $0 > out
  print "" > out
  print "#include \"a653Type.h\"" > out
  next
}

{
  # Print all other lines to the current output file
  if (out) print $0 > out
}
