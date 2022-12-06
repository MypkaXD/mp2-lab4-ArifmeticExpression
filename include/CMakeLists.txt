set(target ${MP2_CUSTOM_PROJECT})

project(${target})

file(GLOB hdrs "*.h*")

add_custom_target(${target} SOURCES ${hdrs})
