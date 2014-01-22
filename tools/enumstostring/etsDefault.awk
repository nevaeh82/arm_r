function getScopeWithLimitRight(lr) {
  if( ! scope_cnt) return;
  ret = "";
  for(i = 0; i < scope_cnt - lr; i++) {
    ret = ret scope[i] "::";
  }
  return ret;
}

function getScope() {
  return getScopeWithLimitRight(0);
}

function addScope(newscope) {
  scope[scope_cnt++] = newscope;
}

function addNamespace(newnamespace) {
  namespace[namespace_cnt++] = newnamespace;
}

function getNamespace() {
  return namespace[--namespace_cnt];
}

BEGIN {
	name = "";
	print "// ****************************"
	print "// *"
	print "// * Auto-generated don't edit"
	print "// *"
	print "// ****************************"
	print "#ifndef " toupper(classPrefix) "NUMSTOSTRING_H"
	print "#define " toupper(classPrefix) "NUMSTOSTRING_H\n"
	print "#include \"" includeFile "\"\n"
	scope_cnt = 0;
	namespace_cnt = 0;
	brackets = 0;
}

/^[/][/]/ { 
  next; 
}

/(namespace)/ {
	if ($1 == "namespace"){
	  print "namespace " $2 "\n{";
	  addNamespace($2);
	}
	next;
}

/(struct)|(class)/ {
	if (scope_cnt ==0){
		print "class " classPrefix "EnumsToString {"
		print "public:"
	}
	
	sub(/\r/, "");
	if ( $1 == "typedef" )
		addScope($3);
	else 
		addScope($2);
	next;
}

/enum/ {  
  if ( $1 == "typedef" ) name = $3;
   else name = $2;

  delete indexes;

  print "\tstatic QString enumToString( "getScope() name" val)\n\t{"
  print "\t\tswitch(val) {"
  addScope(name);
  next;
}

/{/ { 
  brackets++; 
  next; 
}

/}/ {
  if ( name ) {
    print "\t\t}"
    print "\t\treturn QString(\"undefined (%1)\").arg(val);"
    print "\t} // end of " name
  }
  if(scope_cnt) {
    delete scope[--scope_cnt];
  }
  name ="";
  brackets--;

  next;
}

{
  if ( $1 == "//" ) next;
  if ( index( $1, "//") == 1 ) next;

  if ( $1 != "" && name != "") {
    if($2 == "=") {
      new_index = $3;
    }
    else {
      new_index = $2;
    }
    match(new_index, /-?[0-9]+/);
    new_index = substr(new_index, RSTART, RLENGTH);
    if(new_index in indexes) {
      print "//\t\t\tcase " getScopeWithLimitRight(1) $1 ": return \"" getScopeWithLimitRight(1) $1 "\"; // " new_index
      next;
    }
    indexes[new_index] = 1;
    print "\t\t\tcase " getScopeWithLimitRight(1) $1 ": return \"" getScopeWithLimitRight(1) $1 "\"; // " new_index
  }
}

END {
	print "};";
	for (nm = 0; nm <= namespace_cnt - 1; nm++)
		print "} // namespace "  namespace[nm];
	print "\n#endif"
}
