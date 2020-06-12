
# Lil Engie Logging
`#include <Core/Debug/Log.h>`

Lil Engie features a simple logging system with various verobosities to allow for easy filtering of the console. 

## Macros

| Macro| Use case |
|--|--|
| `LIL_LOG(...)`|Logs with default **Log** verbosity. (Use this for most logs)|
| `LIL_WARN(...)`|Logs with **Warning** verbosity|
| `LIL_ERROR(...)`|Logs with **Error** verbosity|
| `LIL_FATAL(...)`|Logs with **Fatal** verbosity|