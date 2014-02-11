DEFINES += OFX_EXTENSIONS_NUKE OFX_EXTENSIONS_TUTTLE OFX_EXTENSIONS_VEGAS OFX_SUPPORTS_PARAMETRIC OFX_EXTENSIONS_NATRON OFX_EXTENSIONS_TUTTLE
#DEFINES += OFX_SUPPORTS_MULTITHREAD

trace_ofx_actions{
    DEFINES += OFX_DEBUG_ACTIONS
}

trace_ofx_params{
    DEFINES += OFX_DEBUG_PARAMETERS
}

trace_ofx_properties{
    DEFINES += OFX_DEBUG_PROPERTIES
}

log{
    DEFINES += NATRON_LOG
}

CONFIG(debug, debug|release){
    message("Compiling in DEBUG mode.")
    DEFINES += NATRON_DEBUG
}

*g++* {
  QMAKE_CXXFLAGS += -ftemplate-depth-1024
  QMAKE_CXXFLAGS_WARN_ON += -Wextra
  c++11 {
    # check for at least version 4.7
    GCCVer = $$system($$QMAKE_CXX --version)
    contains(GCCVer,[0-3]\\.[0-9]+.*) {
      error("At least GCC 4.6 is required.")
    } else {
      contains(GCCVer,4\\.[0-5].*) {
        error("At least GCC 4.6 is required.")
      } else {
        contains(GCCVer,4\\.6.*) {
          QMAKE_CXXFLAGS += -std=c++0x
        } else {
          QMAKE_CXXFLAGS += -std=c++11
        }
      }
    }
  }
}

macx{
  # Set the pbuilder version to 46, which corresponds to Xcode >= 3.x
  # (else qmake generates an old pbproj on Snow Leopard)
  QMAKE_PBUILDER_VERSION = 46
  universal {
    CONFIG += x86 x86_64
  }	    
}

!macx|!universal {
  # precompiled headers don't work with multiple archs
  CONFIG += precompile_header
}

!macx {
  # c++11 build fails on Snow Leopard 10.6 (see the macx section below)
  CONFIG += c++11
}

win32{
  #ofx needs WINDOWS def
  #microsoft compiler needs _MBCS to compile with the multi-byte character set.
  DEFINES += WINDOWS _MBCS COMPILED_FROM_DSP XML_STATIC  NOMINMAX
  DEFINES -= _UNICODE UNICODE
  RC_FILE += Natron.rc
}

unix {
     #  on Unix systems, only the "boost" option needs to be defined in config.pri
     QT_CONFIG -= no-pkg-config
     CONFIG += link_pkgconfig
     glew:      PKGCONFIG += glew
     expat:     PKGCONFIG += expat
     !macx {
         LIBS +=  -lGLU -ldl
     }
} #unix

*-xcode {
  # redefine cxx flags as qmake tends to automatically add -O2 to xcode projects
  QMAKE_CFLAGS -= -O2
  QMAKE_CXXFLAGS -= -O2
  QMAKE_CXXFLAGS += -ftemplate-depth-1024

  # Qt 4.8.5's XCode generator has a bug and places moc_*.cpp files next to the sources instead of inside the build dir
  # However, setting the MOC_DIR doesn't fix that (Xcode build fails)
  # Simple rtule: don't use Xcode
  #MOC_DIR = $$OUT_PWD
  warning("Xcode generator wrongly places the moc files in the source directory. You thus cannot compile with different Qt versions using Xcode.")
}

*clang* {
  QMAKE_CXXFLAGS += -ftemplate-depth-1024
  QMAKE_CXXFLAGS_WARN_ON += -Wextra -Wno-c++11-extensions
  c++11 {
    QMAKE_CXXFLAGS += -std=c++11
  }
}

# see http://clang.llvm.org/docs/AddressSanitizer.html and http://blog.qt.digia.com/blog/2013/04/17/using-gccs-4-8-0-address-sanitizer-with-qt/
addresssanitizer {
  message("Compiling with AddressSanitizer (for gcc >= 4.8 and clang). Set the ASAN_SYMBOLIZER_PATH environment variable to point to the llvm-symbolizer binary, or make sure llvm-symbolizer in in your PATH.")
  message("To compile with clang, use a clang-specific spec, such as unsupported/linux-clang, unsupported/macx-clang, linux-clang or macx-clang.")
  message("For example, with Qt4 on OS X:")
  message("  sudo port install clang-3.4")
  message("  sudo port select clang mp-clang-3.4")
  message("  export ASAN_SYMBOLIZER_PATH=/opt/local/bin/llvm-symbolizer-mp-3.4")
  message("  qmake -spec unsupported/macx-clang CONFIG+=addresssanitizer ...")
  message("see http://clang.llvm.org/docs/AddressSanitizer.html")
  CONFIG += debug
  QMAKE_CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -O1
  QMAKE_CFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -O1
  QMAKE_LFLAGS += -fsanitize=address -g
}

# see http://clang.llvm.org/docs/ThreadSanitizer.html
threadsanitizer {
  message("Compiling with ThreadSanitizer (for clang).")
  message("see http://clang.llvm.org/docs/ThreadSanitizer.html")
  CONFIG += debug
  QMAKE_CXXFLAGS += -fsanitize=thread -O1
  QMAKE_CFLAGS += -fsanitize=thread -O1
  QMAKE_LFLAGS += -fsanitize=thread -g
}

coverage {
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage -O0
  QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage
  QMAKE_CLEAN += $(OBJECTS_DIR)/*.gcda $(OBJECTS_DIR)/*.gcno
}
