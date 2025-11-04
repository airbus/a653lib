#!/usr/bin/env -S awk -f

BEGIN {
  # retain information whether the current section of the header is portable or not
  is_impl_dependent = -1;

  # use linux style linendings (just \n, not \r\n)
  # ORS = "\n";

  BINMODE = 0;
}

# detect that we are in an implementation dependent section of the header
"/*" == $1 && "Implementation" == $2 && "Dependent" == $3 {
  is_impl_dependent = 1;
}

# detect that we are in an implementation portable section of the header
"/*" == $1 && "Implementation" == $2 && "Portable" == $3 {
  is_impl_dependent = 0;
}

# mark all functions to be importend from the arinc module
"extern" == $1 && "void" == $2 && $4 ~/^\(/ {
  print "WASM_IMPORT_MODULE(\"arinc653\")"
}


# make all implementation dependent defines ifndef based
"#define" == $1 && $2 ~ /^SYSTEM_LIMIT_/ && 1 == is_impl_dependent {
  define_ident = $2;
  define_value = $3;

  # print the comment
  for (i = 4; i <= NF; i++) {
    if (i != NF) {
      printf("%s ", $i);
    } else {
      print $i;
    }
  }

  printf("%-8s %s\n", "#ifndef", define_ident);
  printf("%-8s %-38s %s\n", $1, $2, $3);
  print "#endif";
  next;
}


# always pick `APEX_LONG_INTEGER` (which ist `int64_t`) where there is a choice
{
  gsub("<an APEX integer type>", "A653_INTEGER     ");

  # to fit to Airbus a653lib
  gsub("APEX_BYTE", "A653_BYTE");
  gsub("APEX_WORD", "A653_WORD");
  gsub("APEX_INTEGER", "A653_INTEGER");
  gsub("APEX_UNSIGNED", "A653_UNSIGNED");
  gsub("APEX_LONG_INTEGER", "A653_LONG_INTEGER");
  print;
}


