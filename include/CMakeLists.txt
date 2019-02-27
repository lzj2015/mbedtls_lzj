option(INSTALL_SSL_HEADERS "Install AisinoSSL headers." ON)

if(INSTALL_SSL_HEADERS)

    install(DIRECTORY . DESTINATION include/ 
        PATTERN "CMakeLists.txt" EXCLUDE
        PATTERN ".gitignore" EXCLUDE)

endif(INSTALL_SSL_HEADERS)
