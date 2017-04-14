# Contributing

The following are few guidelines and answers intended for contributors

## What can i contribute ?

you can contribute by making one of those:
* Report a bug ([jump](#bug))
* Suggest a feature ([jump](#feature))
* Code a change (this should be first suggested) ([jump](#code-change))

## How can i contribute ?

### <a id='bug'></a>Reporting a bug

Bugs are reported at the [issue tracker]

If you think you found a bug:
* Check the [issue tracker]
    * If you did find a relevant post about the problem: bring details or an upvote (even in closed issue)
    * if you didn't find a relevant post then you can create a new issue
        * The title should be relevant, explaining the problem in few words, avoid uppercases letters
        * The issue should be very detailed, explaining how to reproduce the bug, containing anything that you think useful in order to fix it

### <a id='feature'></a>Suggesting a feature

Features are also welcome but there are few things to take into account.

* Features are also tracked [here](https://github.com/tonykero/Moe/issues), check it before suggesting a feature in order to avoid duplicates.
* Features should be relevant to the Moe library, you don't want to see a machine-learning library with a built-in currency converter
* Feature suggestions are discussions, but don't depend on a general agreement

### <a id='code-change'></a>Making changes

Code contributions are more than welcome!

* Check for duplicates [here](https://github.com/tonykero/Moe/pulls)
* Use [pull requests] in order to submit a change
* Follow [code style guidelines](#coding-style)
* Dependencies should be optional (#define / CMake Option)
* Commit on a disctinct branch
* If your contribution fix an issue, it should preferably only fix one or closely related ones
* Explain and comment your code

## <a id='coding-style'></a>Coding Style

The following is the coding style used in Moe

### Naming

---

Variables naming is in lowerCamelCase
```cpp
unsigned int moesCount;
```
> we need a basic style to start with

---

Members variables are also written in lowerCamelCase, but have a 'm_' prefix
```cpp
std::string m_id;
```
> to distinguish them from function-scoped variables

---

Arguments have a '_' prefix
```cpp
void run( unsigned int _generations );
```
> to distinguish them from members, notably in setters

---

Then classes are written in UpperCamelCase, with first public members then private members
```cpp
class UltraPowerfulAlgorithm : public PowerfulAlgorithm
{
    public:
        void run( unsigned int _generations );
    
    private:
        unsigned int m_generations;
};
```
> note braces placement

### Indentation

Indentations should be 4-space long

also if you have multiple variables declarations:
```cpp
unsigned int m_moesPerGen;
unsigned int m_generations;
```
should be
```cpp
unsigned int    m_moesPerGen,
                m_generations;
```

### Other points:
* Keep naming simple: 3 words max
* There is no file naming rule
* No forward-declaration
* Keep header-only
* Keep dependency-free (or optional dependencies)
* Keep platform independent

Parsing the source in order to be confident with the coding style is recommended.

[pull requests]: https://github.com/tonykero/Moe/pulls
[issue tracker]: https://github.com/tonykero/Moe/issues
