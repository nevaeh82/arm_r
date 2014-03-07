defineReplace( version_parts ) {
	eval( VERSION_PARTS = $$replace(VERSION, \\., " ") )
	return( $$VERSION_PARTS )
}

#
# Check developer version
# 
VERSION_PARTS = $$version_parts()
count( VERSION_PARTS, 2 ) {
	VERSION = $${VERSION}.0
	message( devel version )
} 


defineReplace( version_maj ){
	var = $$version_parts()
        return( $$member(var,0) )
}
defineReplace( version_min ) {
	var = $$version_parts()
        return( $$member(var,1) )
}
defineReplace( version_pat ) {
	var = $$version_parts()
        return( $$member(var,2) )
}

