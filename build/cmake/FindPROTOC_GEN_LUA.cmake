FIND_PROGRAM(PROTOC_GEN_LUA_BIN
	NAMES
		protoc-gen-lua.cmd
		protoc-gen-lua
	PATHS 
		${PROTOC_GEN_LUA}
		/usr/local/bin/
		/usr/bin/
		${PYTHON_ROOT}/Scripts
)
IF(PROTOC_GEN_LUA_BIN)
	SET(PROTOC_GEN_LUA_FOUND TRUE)
ELSE(PROTOC_GEN_LUA_BIN)
	SET(PROTOC_GEN_LUA_FOUND FALSE)
ENDIF(PROTOC_GEN_LUA_BIN)