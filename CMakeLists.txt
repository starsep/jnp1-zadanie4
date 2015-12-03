cmake_minimum_required(VERSION 2.8)
project(jnp_zadanie4)

add_custom_target(pizza COMMAND make -C ${jnp_zadanie4_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})
