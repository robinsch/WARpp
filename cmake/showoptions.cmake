# output generic information about the core and buildtype chosen
message("")
message("* WARpp Warhammer Online Server")
message("")

# Show infomation about the options selected during configuration

if( SERVERS )
  message("* Build world/loginserver : Yes (default)")
else()
  message("* Build world/loginserver : No")
endif()

if( USE_COREPCH )
  message("* Build core w/PCH       : Yes (default)")
else()
  message("* Build core w/PCH       : No")
endif()

if( WITH_COREDEBUG )
  message("")
  message(" *** WITH_COREDEBUG - WARNING!")
  message(" *** additional core debug logs have been enabled!")
  message(" *** this setting doesn't help to get better crash logs!")
  message(" *** in case you are searching for better crash logs use")
  message(" *** -DCMAKE_BUILD_TYPE=RelWithDebug")
  message(" *** DO NOT ENABLE IT UNLESS YOU KNOW WHAT YOU'RE DOING!")
  message("* Use coreside debug     : Yes")
  add_definitions(-DTRINITY_DEBUG)
else()
  message("* Use coreside debug     : No  (default)")
endif()

message("")
