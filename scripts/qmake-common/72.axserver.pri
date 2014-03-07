
contains(CONFIG,qaxserver){
  win32-msvc2010 {
   QMAKE_POST_LINK  += &amp;&amp; cd "$$DESTDIR"
  }
}