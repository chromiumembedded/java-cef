#! /bin/csh -f

set JDK_BIN = "/System/Library/Frameworks/JavaVM.framework/Home/bin"
set CLS_PATH = "build/classes"
set OUT_PATH = "src/native/src"

echo "class path: $CLS_PATH"

$JDK_BIN/javah -force -classpath $CLS_PATH -d $OUT_PATH org.limewire.cef.$1
#rm $OUT_PATH/$1.h
mv $OUT_PATH/org_limewire_cef_$1.h $OUT_PATH/$1.h
