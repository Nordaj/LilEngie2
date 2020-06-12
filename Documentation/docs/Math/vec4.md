# Lil Math - vec4
`#include <Core/Math/LilMath.h>`

The 4D vector class for Lil Math

---

## Constructors
```cpp
vec4()
```
Empty constructor initializes all components to 0
<br /><br />

```cpp
vec4(float x, float y, float z, float w)
```
Initializes corresponding elements in XYZW order
<br /><br />

```cpp
vec4(vec3 v3, float w)
```
Initializes XYZ components to given vec3 with trailing scalar for W
<br /><br />

---

## Functions
```cpp
float dot(vec4 a, vec4 b)
```
Returns scalar result of dot product. (Sum of product of each component)
<br /><br />

```cpp
foat len(vec4 vec)
```
Calculates the real length of the given vector
<br /><br />

```cpp
float len2(vec4 vec)
```
Calculates the squared length of the given vector. This is a much faster operation. Is the equivilent of dot(v, v)
<br /><br />

```cpp
void normalize(vec4& vec)
```
Normalizes given vector to make vector length of 1
<br /><br />

---

## Union
|||
|--|--|
`x, y, z, w`|Each individual component as 4 byte floats
`r, g, b, a`|Each individual component using color variables as 4 byte floats
`v3`|XYZ component vec3
`s[4]`|Each component stored as a 4 element array