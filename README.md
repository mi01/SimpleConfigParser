### Simple config parser
A simple config file parser written in C++11. It is header-only and has no dependencies, just using the C++ standard library.

#### Usage
You can initialize the parser with the constructor. Optional parameters in the constructor let you adjust the delim and comment character, or enable debug output.
```
ConfigParser config("config.cfg");
```

The default config file format is like in this example:
```
# Sample config
foo = 42
bar = Hello World!  # Comment
pi = 3.14159265359
```

Reading entries from the config:
```
int foo = config.getInt("foo").second;
std::string bar = config.getString("bar").second;
std::pair<bool, double> pi = config.getDouble("pi");
```

To check if all entries have successfully been read, simply invoke `config.hasErrors()`. This will return true, if the file is not readable or a mandatory key is not existing. You can also read optional entries from the config or set a fallback value. In this case the error flag is not set.

```
auto notExisting = config.getString("notExisting", true);

if (!notExisting.first)
{
    std::cerr << "Optional key not found in config!" << std::endl;
}

std::string fallback = config.getString("notExisting", std::string("fallback")).second;
```
