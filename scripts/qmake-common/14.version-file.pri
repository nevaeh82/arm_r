
!isEmpty( VERSION_FILE ) {
	exists( $${VERSION_FILE}.build ) {
		include( $${VERSION_FILE}.build  )
	}
	!exists( $${VERSION_FILE}.build ) {
		include( $${VERSION_FILE} )
	}
}
